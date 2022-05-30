/*
* \file sdrxcvr.c
*
* Mods: Ryan Hiser: FMComms4 is a 1x1 and FMComms2 is a 2x2 so 
*       I am modifiying accoridingly. Removed all rx2.
*
*       Also Adding Stuff for TX.
*
* Based off code from Noel Martinez noelmartinez38@gmail 
* and Javier Arribas, jarribas(at)cttc.es
*/

// Includes (Standard Libraries)
#include <unistd.h>

// Includes (SDR)
#include "v2x_sdr.h"
#include "v2x_sdr_uio.h"
#include "v2x_sdr_vio.h"

// Includes (Simulink)
#include "rtwtypes.h"
#include "v2x_constants.h"
#include "v2x_tx_bb_wrapper.h"
#include "v2x_tx_mod_wrapper.h"
#include "v2x_rx_bb_wrapper.h"

// Helper macros
#define ASSERT(expr) { \
    if (!(expr)) { \
        (void) fprintf(stderr, "assertion failed (%s:%d)\n", __FILE__, __LINE__); \
        (void) abort(); \
    } \
}

// RX is input, TX is output
enum iodev
{
    RX,
    TX
};

// Common RX and TX streaming params
struct stream_cfg {
    long long bw_hz; // Analog banwidth in Hz
    long long fs_hz; // Baseband sample rate in Hz
    long long lo_hz; // Local oscillator frequency in Hz
    const char* rfport; // Port name
    double rf_gain_rx1_;
    double rf_gain_tx1_;
    //double rf_gain_rx2_;
    const char* gain_mode_rx1_;
    //const char* gain_mode_rx2_;
    int bb_dc_offset_tracking_;
    int quadrature_tracking_;
    int rf_dc_offset_tracking_;
    int ref_clock_;
};

// ---------------------- Globals ----------------------
// Static scratch mem for strings
static char tmpstr[64];

// IIO structs required for streaming
static struct iio_context *ctx   = NULL;
static struct iio_channel *rx0_i = NULL;
static struct iio_channel *rx0_q = NULL;
static struct iio_buffer  *rxbuf = NULL;

// Addifng for TX
static struct iio_channel *tx0_i = NULL;
static struct iio_channel *tx0_q = NULL;
static struct iio_buffer *txbuf = NULL;

static bool stop = false;
static bool app_running = true;

typedef struct xflow_pthread_data
{
    struct iio_device *dev;
} xflow_pthread_data;

uint32_t vio_start_agc = 0;
uint32_t thread_period = 10000; // 0.01 seconds
uint32_t uio_poll_period = 1000; // 0.001 seconds

// ---------------------- Cleanup and exit ----------------------
static void program_shutdown()
{
    printf("* Destroying buffers\n");
    if (rxbuf) { iio_buffer_destroy(rxbuf); }
    if (txbuf) { iio_buffer_destroy(txbuf); }

    printf("* Disabling streaming channels\n");
    if (rx0_i) { iio_channel_disable(rx0_i); }
    if (rx0_q) { iio_channel_disable(rx0_q); }
    if (tx0_i) { iio_channel_disable(tx0_i); }
    if (tx0_q) { iio_channel_disable(tx0_q); }

    printf("* Destroying context\n");
    if (ctx) { iio_context_destroy(ctx); }
    app_running = false;
    exit(0);
}

static void handle_sig(int sig)
{
    printf("Waiting for process to finish...\n");
    stop = true;
}

// ---------------------- IIO functions ----------------------
// Check return value of attr_write function
static void errchk(int v, const char* what) {
    if (v < 0)
    {
        fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n",
                v, what);
        program_shutdown();
    }
}

// Write attribute: long long int
static void wr_ch_lli(struct iio_channel *chn, const char* what, long long val)
{
    errchk(iio_channel_attr_write_longlong(chn, what, val), what);
}

// Write attribute: string
static void wr_ch_str(struct iio_channel *chn, const char* what, const char* str)
{
    errchk(iio_channel_attr_write(chn, what, str), what);
}

// Helper function generating channel names
static char* get_ch_name(const char* type, int id)
{
    snprintf(tmpstr, sizeof(tmpstr), "%s%d", type, id);
    return tmpstr;
}

// ---------------------- AD helper functions ----------------------
// Returns ad9361 phy device
static struct iio_device* get_ad9361_phy(struct iio_context *ctx)
{
    struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
    ASSERT(dev && "No ad9361-phy found");
    return dev;
}

// Finds AD9361 streaming IIO devices
static bool get_ad9361_stream_dev(struct iio_context *ctx, enum iodev d, struct iio_device **dev)
{
    switch (d)
    {
        case TX:
            *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");
            return *dev != NULL;
        case RX: 
            *dev = iio_context_find_device(ctx, "cf-ad9361-lpc");  
            return *dev != NULL;
        default: ASSERT(0); 
            return false;
    }
}

// Finds AD9361 streaming IIO channels
static bool get_ad9361_stream_ch(struct iio_context *ctx, enum iodev d, struct iio_device *dev,
                                 int chid, struct iio_channel **chn)
{
    *chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
    if (!*chn)
        *chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
    return *chn != NULL;
}

// Finds AD9361 phy IIO configuration channel with id chid
static bool get_phy_chan(struct iio_context *ctx, enum iodev d, int chid, struct iio_channel **chn)
{
    switch (d)
    {
        case TX: 
            *chn = iio_device_find_channel(get_ad9361_phy(ctx),
                                           get_ch_name("voltage", chid), false); 
            return *chn != NULL;
        case RX: 
            *chn = iio_device_find_channel(get_ad9361_phy(ctx),
                                           get_ch_name("voltage", chid), false); 
            return *chn != NULL;
        default: ASSERT(0); 
            return false;
    }
}

// Finds AD9361 local oscillator IIO configuration channels
static bool get_lo_chan(struct iio_context *ctx, enum iodev d, struct iio_channel **chn)
{
    switch (d)
    {
        // LO chan is always output, i.e. true
        case RX:
            *chn = iio_device_find_channel(get_ad9361_phy(ctx), "altvoltage0", true);
            return *chn != NULL;
        case TX:
            *chn = iio_device_find_channel(get_ad9361_phy(ctx), "altvoltage1", true);
            return *chn != NULL;
        default:
            return false;
    }
}

// Applies streaming configuration through IIO
static bool cfg_ad9361_streaming_ch(struct iio_context *ctx, struct stream_cfg *cfg, 
                                    enum iodev type, int chid)
{
    struct iio_channel *chn = NULL;

    // Configure phy and lo channels
    printf("    + Acquiring AD9361 phy channel %d\n", chid);
    if (!get_phy_chan(ctx, type, chid, &chn)) {	return false; }
    wr_ch_str(chn, "rf_port_select",     cfg->rfport);
    wr_ch_lli(chn, "rf_bandwidth",       cfg->bw_hz);
    wr_ch_lli(chn, "sampling_frequency", cfg->fs_hz);

    // Configure LO channel
    printf("  ~ Acquiring AD9361 %s lo channel\n", type == TX ? "TX" : "RX");
    if (!get_lo_chan(ctx, type, &chn)) { 
        return false; 
    }
    wr_ch_lli(chn, "frequency", cfg->lo_hz);
    return true;
}

// ---------------------- RX setup functions ----------------------
// https://analogdevicesinc.github.io/libiio/v0.16/group__Device
static bool config_ad9361_rx_local(struct stream_cfg *cfg, struct iio_context *ctx)
{
    // Local variables
    struct iio_device *ad9361_phy;
    int ret;

    // Print values to write
    printf("* RX hardware gain control mode to be set: %s\n", cfg->gain_mode_rx1_);
    printf("* RX hardware gain to be set: %f dB\n", cfg->rf_gain_rx1_);
    ad9361_phy = iio_context_find_device(ctx, "ad9361-phy");

    // Gain control mode
    ret = iio_device_attr_write(ad9361_phy, "in_voltage0_gain_control_mode",
                                cfg->gain_mode_rx1_);
    if (ret < 0)
    {
        fprintf(stderr, "Failed to set in_voltage0_gain_control_mode: %d\n", ret);
    }

    // Hardware gain
    ret = iio_device_attr_write_double(ad9361_phy, "in_voltage0_hardwaregain",
                                       cfg->rf_gain_rx1_);
    if (ret < 0)
    {
        fprintf(stderr, "Failed to set in_voltage0_hardwaregain: %d\n", ret);
    }

    // Return
    printf("--- END OF FMCOMMS4 RX CONFIGURATION ---\n\n");
    return true;
}

// ---------------------- TX setup functions ----------------------
// https://analogdevicesinc.github.io/libiio/v0.16/group__Device
// https://wiki.analog.com/resources/tools-software/linux-drivers/iio-transceiver/ad9361#tx_attenuation_control
static bool config_ad9361_tx_local(struct stream_cfg *cfg, struct iio_context *ctx)
{
    // Set harware gain
    struct iio_device *ad9361_phy;
    int ret;
    printf("* TX hardware gain to be set: %f dB\n", cfg->rf_gain_tx1_);
    ad9361_phy = iio_context_find_device(ctx, "ad9361-phy");
    ret = iio_device_attr_write_double(ad9361_phy, "out_voltage0_hardwaregain", cfg->rf_gain_tx1_);
    if (ret < 0)
    {
        fprintf(stderr, "Failed to set in_voltage0_hardwaregain: %d\n", ret);
    }

    // Return
    printf("--- END OF FMCOMMS4 TX CONFIGURATION ---\n\n");
    return true;
}

// ---------------------- Initialize XCVR ----------------------
static int init_xcvr(const sdrini_t *ini, sdrstat_t **stat, 
                    struct iio_device **tx, struct iio_device **rx,
                    struct stream_cfg *txcfg, struct stream_cfg *rxcfg)
{
    // RX stream config
    printf("RX Configuration:\n");
    rxcfg->bw_hz = (long long)ini->f_bw;   
    rxcfg->fs_hz = (long long)ini->f_sf;   // max sample rate
    rxcfg->lo_hz = (long long)ini->f_cf; 
    printf(" - SAMPLING RATE: %lli Hz\n", rxcfg->fs_hz);
    printf(" - Center Freq: %lli Hz\n", rxcfg->lo_hz);
    printf(" - Rf Bandwidth: %lli Hz\n", rxcfg->bw_hz);
    rxcfg->rfport = "A_BALANCED"; // port A (select for rf freq.)
    rxcfg->rf_gain_rx1_=ini->rx_hardware_gain;
    rxcfg->gain_mode_rx1_=ini->rx_gain_control_mode;
    rxcfg->bb_dc_offset_tracking_=ini->bb_dc_offset_tracking;
    rxcfg->quadrature_tracking_=ini->quadrature_tracking;
    rxcfg->rf_dc_offset_tracking_=ini->rf_dc_offset_tracking;
    rxcfg->ref_clock_=ini->ref_clock;

    // Tx stream config
    printf("TX Configuration:\n");
    txcfg->bw_hz = (long long)ini->f_bw;
    txcfg->fs_hz = (long long)ini->f_sf;
    txcfg->lo_hz = (long long)ini->f_cf;
    txcfg->rf_gain_tx1_ = ini->tx_hardware_gain;
    txcfg->rfport = "A_BALANCED"; // port A (select for rf freq.)
    printf(" - SAMPLING RATE: %lli Hz\n", txcfg->fs_hz);
    printf(" - Center Freq: %lli Hz\n", txcfg->lo_hz);
    printf(" - Rf Bandwidth: %lli Hz\n", txcfg->bw_hz);
    printf("\n\n");
    
    printf("* Acquiring IIO context\n");
    ASSERT((ctx = iio_create_default_context()) && "No context");
    ASSERT(iio_context_get_devices_count(ctx) > 0 && "No devices");

    printf("* Acquiring AD9361 streaming devices\n");
    printf("  ~ RX\n");
    ASSERT(get_ad9361_stream_dev(ctx, RX, rx) && "No rx dev found");
    printf("  ~ TX\n");
    ASSERT(get_ad9361_stream_dev(ctx, TX, tx) && "No tx dev found");

    printf("* Configuring AD9361 for streaming\n");
    printf("  ~ RX\n");
    ASSERT(cfg_ad9361_streaming_ch(ctx, rxcfg, RX, 0) && "RX port 0 not found");
    printf("  ~ TX\n");
    ASSERT(cfg_ad9361_streaming_ch(ctx, txcfg, TX, 0) && "TX port 0 not found");

    printf("* Initializing AD9361 IIO streaming channels\n");
    if(ini->dtype==DTYPEIQ)
    {
        printf("* Enableling i and q channels.\n");
        ASSERT(get_ad9361_stream_ch(ctx, TX, *tx, 0, &tx0_i) && "TX chan i not found");
        ASSERT(get_ad9361_stream_ch(ctx, TX, *tx, 1, &tx0_q) && "TX chan q not found");
        ASSERT(get_ad9361_stream_ch(ctx, RX, *rx, 0, &rx0_i) && "RX chan i not found");
        ASSERT(get_ad9361_stream_ch(ctx, RX, *rx, 1, &rx0_q) && "RX chan q not found");

        printf("* Enabling IIO streaming channels\n");
        iio_channel_enable(tx0_i);
        iio_channel_enable(tx0_q);
        iio_channel_enable(rx0_i);
        iio_channel_enable(rx0_q);
    }
    else
    {
        printf("* Enableling i channel only.\n");
        ASSERT(get_ad9361_stream_ch(ctx, TX, *tx, 0, &tx0_i) && "TX chan i not found");
        ASSERT(get_ad9361_stream_ch(ctx, RX, *rx, 0, &rx0_i) && "RX chan i not found");

        printf("* Enabling IIO streaming channels\n");
        iio_channel_enable(tx0_i);
        iio_channel_enable(rx0_i);
    }

    config_ad9361_rx_local(rxcfg, ctx);
    config_ad9361_tx_local(txcfg, ctx);
    
    printf("* Center Freq: %f Hz\n", ini->f_cf);
    printf("* Sampling Freq: %f Hz\n", ini->f_sf);
    printf("* Creating non-cyclic buffer\n");
    
    int16_t sampletime = ini->msToProcess/1E3;
    
    (*stat)->buffer_size = ini->buffer_size*4; // 2^22, cant go higher, x4 for 2 bytes per IQ
    printf("BUFFERSIZE = %llu\n", (*stat)->buffer_size);
    printf("stat->buffer_size = %llu, ini->buffer_size = %u\n",
           (*stat)->buffer_size, ini->buffer_size);

    // numbuf is the number of kernel buffers that we set, 
    // and also the number of times we write to file, 
    // total bytes written to file = numbuf*buffersize*datasize*datatype 
    int numbuf = ceil((ini->f_sf * ini->dtype * ini->datasize * sampletime)/
                      ((*stat)->buffer_size * ini->dtype * ini->datasize));

    // One kernel buffer per loop iteration, not sure how kernel buffers count does ?
    int i = iio_device_set_kernel_buffers_count(*rx, numbuf);
    
    if(i != 0)
    { 
        printf("failed to set kernel buffers count.\n");
    }
    printf("Number of kernel buffers set is: %d\n", numbuf);

    rxbuf = iio_device_create_buffer(*rx, (*stat)->buffer_size, false); 
    if (!rxbuf){
        perror("Could not create RX buffer");
        program_shutdown();
    }
    
    txbuf = iio_device_create_buffer(*tx, (*stat)->buffer_size, false);
    if (!txbuf) {
        perror("Could not create TX buffer");
        program_shutdown();
    }

    // Return number of buffers
    return numbuf;
}

// ---------------------- Run TX/RX ----------------------
static void load_frame_txmod(int16_t *input)
{
    boolean_T tx_bb_out[TX_BB_OUT_BITS];
    cint16_T tx_mod_out[TX_MOD_OUT_SYMS];
    get_tx_bb_out(tx_bb_out);
    get_tx_mod_out(tx_bb_out, tx_mod_out);
    memcpy(input, (int16_t *) tx_mod_out, TX_MOD_OUT_SYMS * 2 * sizeof(input[0]));
}

static void* tx_thread_fn(void* args)
{
    // Loop init
    printf("TX thread started\n");
    uint32_t loop_num = 0;
    int16_t tx_mod_out[TX_MOD_OUT_SYMS * 2];
    ssize_t nbytes_tx;
    char *p_start;

    // Run loop
    while (!stop)
    {
        // WRITE: Get pointers to TX buf and write IQ to TX buf port 0
        p_start = iio_buffer_first(txbuf, tx0_i);
        load_frame_txmod(tx_mod_out);
        memmove(p_start, tx_mod_out, TX_MOD_OUT_SYMS * 2);

        // Schecule TX Buffer
        nbytes_tx = iio_buffer_push(txbuf);
        if (nbytes_tx < 0)
        { 
            printf("Error scheduling tx buf %d\n", (int) nbytes_tx);
            program_shutdown(); 
        }

        // Sleep
        printf("Finished TX thread %d\n", loop_num);
        usleep(thread_period); // 0.5 seconds = 500000
        loop_num++;
    }

    // Return
    return NULL;
}

static void process_frame_rxdemod(uint32_t* uio_pkt, int* sockfd, struct sockaddr_in * addr_con)
{
    // Process data
    struct payload_struct payload;

    // Reset payload to zero
    memset(&payload, 0, sizeof(payload));

    uint8_t output_frame[RX_BB_OUT_BYTES];
    get_rx_bb_out(uio_pkt, output_frame);

    // Fix endianness of payload portion of packet
    fix_payload_packet(output_frame, &payload);

    // Print
    printf("RX payload (before baseband processing):\n");
    for(int idx = 0; idx < 64; idx++)
    {
        printf("    TEST[%d]: 0x%08x\n",idx,uio_pkt[idx]);
    }
    printf("    Name:  ");
    for (int i = 0; i < 4 ; i++)
    {
        printf("%c",payload.name[i]);
    }
    printf("\n");	
    printf("    Lat:   %f\n", payload.lat);
    printf("    Lon:   %f\n", payload.lon);
    printf("    Speed: %d\n", payload.speed);
    printf("    Dir:   %d\n", payload.dir);
    printf("    DTNS:  %f\n", payload.dist_next_step);

#ifdef USE_UDP_CLIENT

    unsigned int frame_size = TX_BB_IN_BYTES;

    int addrlen = sizeof(*addr_con);

    // usleep(150e3);
    sendto(*sockfd, (void *)output_frame, frame_size, sendrecvflag, (struct sockaddr*)addr_con, addrlen);

#endif
}

static void* rx_thread_fn(void* args)
{
    // Loop init
    printf("RX thread started\n");
    uint32_t loop_num = 0;

    // Polling init
    uint32_t rdy = 0;
    uint32_t uio_pkt[64];

    int sockfd = 0;
    struct sockaddr_in addr_con;

#ifdef USE_UDP_CLIENT

    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM,IP_PROTOCOL);

    if (sockfd < 0)
    {
        printf("\nfile descriptor not received!!\n");
        exit(-1);
    }
    else{
        printf("\nfile descriptor %d received\n", sockfd);
    }
#endif

    // Run loop
    while (!stop)
    {
        // Tell RX to listen
        generic_vio_write(RX_CTL_START_AGC, vio_start_agc | 0x1); // set start bit
        usleep(uio_poll_period);

        // Poll ready signal
        while ((!stop) && (rdy == 0))
        {
            rdy = generic_uio_read(RX_IP_RDY);

            usleep(uio_poll_period); // 0.5 seconds = 500000
        }

        // Read demodulated data
        for (int i = 0; i < 64; i++)
        {
            uio_pkt[i] = generic_uio_read(RX_IP_OUTPUT+(i*4)); // *4 because byte addressing
        }

        // Process demodulated data
        process_frame_rxdemod(uio_pkt, &sockfd, &addr_con);

        // Sleep and reset RX
        generic_vio_write(RX_CTL_START_AGC, vio_start_agc & 0xFFFFFFFE);
        printf("Finished RX thread %d\n", loop_num);
        usleep(thread_period); // 0.5 seconds = 500000
        loop_num++;
    }

#ifdef USE_UDP_CLIENT
    close(sockfd);
#endif

    // Return
    return NULL;
}

void rx_ctl_init(sdrini_t *ini)
{
    // Generic initialize
    generic_vio_init(RX_CTL_DEV);
    vio_start_agc = ini->rx_ctl_start_agc;

    // Write to IP Cores
    printf("* Writing RX_CTL_START_AGC   = %u\n", ini->rx_ctl_start_agc);
    printf("* Writing RX_CTL_CORR_THRESH = %u\n", ini->rx_ctl_corr_thresh);
    printf("* Writing RX_CTL_AGC_POW_REF = %u\n", ini->rx_ctl_agc_pow_ref);
    printf("* Writing RX_CTL_STORE_DELAY = %u\n", ini->rx_ctl_store_delay);
    printf("* Writing RX_CTL_TIME_SEL    = %u\n", ini->rx_ctl_time_sel);

    generic_vio_write(RX_CTL_AP_CTRL,     1);
    generic_vio_write(RX_CTL_START_AGC,   ini->rx_ctl_start_agc);
    generic_vio_write(RX_CTL_CORR_THRESH, ini->rx_ctl_corr_thresh);
    generic_vio_write(RX_CTL_AGC_POW_REF, ini->rx_ctl_agc_pow_ref);
    generic_vio_write(RX_CTL_STORE_DELAY, ini->rx_ctl_store_delay);
    generic_vio_write(RX_CTL_TIME_SEL,    ini->rx_ctl_time_sel);
};

// ---------------------- External functions ----------------------
int run_xcvr(sdrini_t *ini, sdrstat_t *stat)
{
    // Listen to ctrl+c and ASSERT
    signal(SIGINT, handle_sig);

    //------------------------- Initialization variables -------------------------
    // Streaming devices
    struct iio_device *tx;
    struct iio_device *rx;

    // Stream configurations
    struct stream_cfg txcfg;
    struct stream_cfg rxcfg;

    // Initialize
    int numbuf = init_xcvr(ini, &stat, &tx, &rx, &txcfg, &rxcfg);
    (void) numbuf;

    // Initialize generated code
    tx_bb_init();
    tx_mod_init();
    rx_bb_init();

    // Initialize IP Cores
    generic_uio_init(RX_IP_DEV);
    rx_ctl_init(ini);

    //------------------------- Data acquisition variables -------------------------
    printf("* Starting IO streaming (press CTRL+C to cancel)\n" );
    
    // Define threads and poll periods
    pthread_t tx_thread;
    pthread_t rx_thread;
    thread_period = ini->thread_period;
    uio_poll_period = ini->uio_poll_period;

    // Run TX
    if (ini->tx_thread_enable)
    {
        printf("* TX thread enabled \n");
        pthread_create(&tx_thread, NULL, tx_thread_fn, NULL);
    }
    else
    {
        printf("* TX thread disabled \n");
    }

    // Run RX
    if (ini->rx_thread_enable)
    {
        printf("* RX thread enabled \n");
        pthread_create(&rx_thread, NULL, rx_thread_fn, NULL);
    }
    else
    {
        printf("* RX thread disabled \n");
    }

    // Cleanup
    if (ini->tx_thread_enable)
    {
        pthread_join(tx_thread, NULL);
    }
    if (ini->rx_thread_enable)
    {
        pthread_join(rx_thread, NULL);
    }
    generic_uio_exit();
    generic_vio_exit();
    printf("* Exiting \n");
    program_shutdown();
    return 0;
} 
