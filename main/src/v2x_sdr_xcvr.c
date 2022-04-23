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

// Includes (SDR)
#include "v2x_sdr.h"

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

/* RX is input, TX is output */
enum iodev { RX, TX };

/* common RX and TX streaming params */
struct stream_cfg {
    long long bw_hz; // Analog banwidth in Hz
    long long fs_hz; // Baseband sample rate in Hz
    long long lo_hz; // Local oscillator frequency in Hz
    const char* rfport; // Port name
    double rf_gain_rx1_;
    //double rf_gain_rx2_;
    const char* gain_mode_rx1_;
    //const char* gain_mode_rx2_;
    int bb_dc_offset_tracking_;
    int quadrature_tracking_;
    int rf_dc_offset_tracking_;
    int ref_clock_;
};
/* static scratch mem for strings */
static char tmpstr[64];

/* IIO structs required for streaming */
static struct iio_context *ctx   = NULL;
static struct iio_channel *rx0_i = NULL;
static struct iio_channel *rx0_q = NULL;
static struct iio_buffer  *rxbuf = NULL;

/* Addifng for TX */
static struct iio_device *tx;
static struct iio_channel *tx0_i = NULL;
static struct iio_channel *tx0_q = NULL;
static struct iio_buffer *txbuf = NULL;
static struct iio_context *ctx_dma = NULL;
static struct stream_cfg txcfg;


static bool stop;
static bool app_running = true;
typedef struct xflow_pthread_data {
    struct iio_device *dev;
} xflow_pthread_data;

/* cleanup and exit */
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

/* check return value of attr_write function */
static void errchk(int v, const char* what) {
    if (v < 0)
    {
        fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n",
                v, what);
        program_shutdown();
    }
}

/* write attribute: long long int */
static void wr_ch_lli(struct iio_channel *chn, const char* what, long long val)
{
    errchk(iio_channel_attr_write_longlong(chn, what, val), what);
}

/* write attribute: string */
static void wr_ch_str(struct iio_channel *chn, const char* what, const char* str)
{
    errchk(iio_channel_attr_write(chn, what, str), what);
}

/* helper function generating channel names */
static char* get_ch_name(const char* type, int id)
{
    snprintf(tmpstr, sizeof(tmpstr), "%s%d", type, id);
    return tmpstr;
}

/* returns ad9361 phy device */
static struct iio_device* get_ad9361_phy(struct iio_context *ctx)
{
    struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
    ASSERT(dev && "No ad9361-phy found");
    return dev;
}

/* finds AD9361 streaming IIO devices */
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

/* finds AD9361 streaming IIO channels */
static bool get_ad9361_stream_ch(struct iio_context *ctx, enum iodev d, struct iio_device *dev,
                                 int chid, struct iio_channel **chn)
{
    *chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
    if (!*chn)
        *chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
    return *chn != NULL;
}

/* finds AD9361 phy IIO configuration channel with id chid */
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

/* finds AD9361 local oscillator IIO configuration channels */
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

/* applies streaming configuration through IIO */
bool cfg_ad9361_streaming_ch(struct iio_context *ctx, struct stream_cfg *cfg, 
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

bool config_ad9361_rx_local(struct stream_cfg *cfg, struct iio_context *ctx)
{
    struct iio_device *ad9361_phy;
    printf("* Hardware gain to be set: %f dB\n", cfg->rf_gain_rx1_);
    ad9361_phy = iio_context_find_device(ctx, "ad9361-phy");
    int ret;
    ret = iio_device_attr_write_double(ad9361_phy, "in_voltage0_hardwaregain",
                                       cfg->rf_gain_rx1_);
    if (ret < 0)
    {
        fprintf(stderr,"Failed to set in_voltage0_hardwaregain: %d\n",ret );
    }

    printf("--- END OF FMCOMMS4 RX CONFIGURATION ---\n\n");
    return true;
}

// --------- TX modulator functions ----------
static void load_frame_txmod(int16_t *input)
{
    boolean_T tx_bb_out[TX_BB_OUT_BITS];
    cint16_T tx_mod_out[TX_MOD_OUT_SYMS];
    get_tx_bb_out(tx_bb_out, 0);
    get_tx_mod_out(tx_bb_out, tx_mod_out);
    memcpy(input, (int16_t *) tx_mod_out, TX_MOD_OUT_SYMS * 2 * sizeof(input[0]));
}

//*************************************
//****** Adding Functions for Tx ******
//*************************************
bool config_ad9361_tx_local(uint64_t bandwidth_, uint64_t sample_rate_, uint64_t freq_,
                            int numbufs,int bytes)
{
    ASSERT((ctx_dma = iio_create_default_context()) && "No context");     
    
    if (!cfg_ad9361_streaming_ch(ctx_dma, &txcfg, TX, 0))
    {
        printf("TX port 0 not found\n");
        printf("AD9361 IIO TX port 0 not found");
    }

    ASSERT(iio_context_get_devices_count(ctx_dma) > 0 && "No Devices");     
    ASSERT(get_ad9361_stream_dev(ctx_dma, TX, &tx) && "No tx dev found");
    ASSERT(get_ad9361_stream_ch(ctx_dma, TX, tx, 0, &tx0_i) && "TX chan i not found");
    ASSERT(get_ad9361_stream_ch(ctx_dma, TX, tx, 1, &tx0_q) && "TX chan q not found");

    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    iio_device_set_kernel_buffers_count(tx, numbufs);
    txbuf = iio_device_create_buffer(tx, bytes, false);

    if (!txbuf)
    {
        perror("Could not create TX buffer");
        if (txbuf) { iio_buffer_destroy(txbuf); }
        if (ctx_dma) { iio_context_destroy(ctx_dma); }
        if (tx0_i) { iio_channel_disable(tx0_i); }
        if (tx0_q) { iio_channel_disable(tx0_q); }
    }

    return false;
}

void iio_buffer_DMA_tx(uint64_t bandwidth_,uint64_t sample_rate_,uint64_t freq_,
                       void *data, int numbuf, int bytes)
{
    ASSERT((ctx_dma = iio_create_default_context()) && "No context");     
    ASSERT(iio_context_get_devices_count(ctx_dma) > 0 && "No Devices");     
    ASSERT(get_ad9361_stream_dev(ctx_dma, TX, &tx) && "No tx dev found");
    ASSERT(get_ad9361_stream_ch(ctx_dma, TX, tx, 0, &tx0_i) && "TX chan i not found");
    ASSERT(get_ad9361_stream_ch(ctx_dma, TX, tx, 1, &tx0_q) && "TX chan q not found");

    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    iio_device_set_kernel_buffers_count(tx,numbuf);
    txbuf = iio_device_create_buffer(tx, bytes, false);

    if (!txbuf)
    {
        perror("Could not create TX buffer");
        if (txbuf) { iio_buffer_destroy(txbuf); }
        if (ctx_dma) { iio_context_destroy(ctx_dma); }
        if (tx0_i) { iio_channel_disable(tx0_i); }
        if (tx0_q) { iio_channel_disable(tx0_q); }
    }
    if (!txbuf)
    {
        perror("Could not create TX buffer");
        if (txbuf) { iio_buffer_destroy(txbuf); }
        if (ctx_dma) { iio_context_destroy(ctx_dma); }
        if (tx0_i) { iio_channel_disable(tx0_i); }
        if (tx0_q) { iio_channel_disable(tx0_q); }
    }
    
    iio_buffer_set_data(txbuf, data);
    iio_channel_write_raw(tx0_i, txbuf, data, bytes);
    iio_buffer_push_partial(txbuf, (bytes/2 ));

    if (txbuf) { iio_buffer_destroy(txbuf); }
        //std::cout << "Done Destroying buffer " << std::endl;
        //std::cout << "Done Destroying context " << std::endl;
    if (tx0_i) { iio_channel_disable(tx0_i); }
        //std::cout << "Done Disableing channel i" << std::endl;
    if (tx0_q) { iio_channel_disable(tx0_q); }
        //std::cout << "Done Disableing channel q" << std::endl;
        
    if (ctx_dma) { iio_context_destroy(ctx_dma); }
}

static void *monitor_thread_fn(void *data)
{
    printf("Thread to Monitor data overflow activated. \n");
    struct xflow_pthread_data *xflow_pthread_data = data;
    uint32_t status;
    int ret;
    
    if(!xflow_pthread_data->dev)
    {
        fprintf(stderr, "Unable to find IIO device\n");
        return (void*)-1;
    }
    
    sleep(1);
    
    ret = iio_device_reg_write(xflow_pthread_data->dev, 0x80000088, 0x6);
    
    while(app_running)
    {
        ret = iio_device_reg_read(xflow_pthread_data->dev, 0x80000088, &status);
        if (ret) {
            fprintf(stderr,"Failed to read overflow status register: %s\n",
                    strerror(-ret));
            continue;
        }

        if (status & 4) {

            fprintf(stderr, "Overflow detected\n");
        }
    
        if(status)	
            iio_device_reg_write(xflow_pthread_data->dev,0x80000088,status);
        sleep(1);
    }

    return (void*)0;
}

// Initialize XCVR
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
    rxcfg->rf_gain_rx1_=ini->hardware_gain;
    rxcfg->gain_mode_rx1_=ini->gain_control_mode;
    rxcfg->bb_dc_offset_tracking_=ini->bb_dc_offset_tracking;
    rxcfg->quadrature_tracking_=ini->quadrature_tracking;
    rxcfg->rf_dc_offset_tracking_=ini->rf_dc_offset_tracking;
    rxcfg->ref_clock_=ini->ref_clock;

    // Tx stream config
    printf("TX Configuration:\n");
    txcfg->bw_hz = (long long)ini->f_bw;
    txcfg->fs_hz = (long long)ini->f_sf;
    txcfg->lo_hz = (long long)ini->f_cf; 
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
    
    printf("* Center Freq: %f Hz\n", ini->f_cf);
    printf("* Sampling Freq: %f Hz\n", ini->f_sf);
    printf("* Creating non-cyclic buffer\n");
    
    int16_t sampletime = ini->msToProcess/1E3;
    
    (*stat)->buffer_size = ini->buffer_size*4; // 2^22, cant go higher, x4 for 2 bytes per IQ
    printf("BUFFERSIZE = %llu\n", (*stat)->buffer_size);
    printf("stat->buffer_size = %llu, ini->buffer_size = %u\n",
           (*stat)->buffer_size, ini->buffer_size);

    /* numbuf is the number of kernel buffers that we set, 
       and also the number of times we write to file, 
       total bytes written to file = numbuf*buffersize*datasize*datatype 
    */
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

// Simple configuration and streaming
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

    //------------------------- Debug settings -------------------------
    int16_t tx_mod_out[TX_MOD_OUT_SYMS * 2];
    numbuf = NUM_FRAMES;

    // Initialize generated code
    tx_bb_init();
    tx_mod_init();
    rx_bb_init();

    //------------------------- Data acquisition variables -------------------------
    pthread_t monitor_thread; // Thread to monitor overflows/underflows
    static struct xflow_pthread_data xflow_pthread_data;
    int ret;

    // Output buffer
    // x2 for I and Q data, x2 for int16.
    printf("* Starting IO streaming (press CTRL+C to cancel)\n" );
    int16_t* outbuf = (int16_t*) malloc(stat->buffer_size * 4); // Allocated bytes is x4 samples
    printf("number of bytes attempted to allocate for output buffer is: %llu\n",
           stat->buffer_size); 
    if(outbuf == NULL)
    {
         printf("Failed to allocate memory for output buffer!\n");
    }
    
    // Open file to record data
    if( (ini->fp = fopen(ini->dump_file, "wb")) == NULL)
    {
        printf("FAILED to open file for writing !!!\n");
        return -1; 
    }

    // Setup RX thread
    xflow_pthread_data.dev = rx;
    if ((ret = pthread_create(&monitor_thread, NULL, monitor_thread_fn, 
         (void *)&xflow_pthread_data)))
    { 
        fprintf(stderr, "Failed to crate monitor thread: %s\n",
        strerror(-ret));
    }

    // Write to file
    printf("* Writing to File \n");
    if (ini->datasize == 2)	
    {
        ssize_t nbytes_rx, nbytes_tx;
        char *p_start;
        // char *p_dat;
        // char *p_end;
        // ptrdiff_t p_inc;
        printf("Number of debug buffers set is: %d\n", numbuf);
           
        for (int i = 0; i < numbuf; i++)
        {
            // Debug statements
            printf("Entetered frame: %d\n", i);

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

            // Sleep until processing is finished
            sleep(1);

            // Refill RX buffer
            nbytes_rx = iio_buffer_refill(rxbuf);
            if (nbytes_rx < 0) { 
                printf("Error refilling rx buf %d\n", (int) nbytes_rx);
                program_shutdown(); 
            }
            
            // READ: Get pointers to RX buf and read IQ from RX buf port 0
            // p_start = iio_buffer_first(rxbuf, rx0_i);
            p_start = iio_buffer_first(txbuf, tx0_i);
            memmove(outbuf, p_start, nbytes_tx);
            fwrite(outbuf, ini->datasize, stat->buffer_size, ini->fp);
            fflush(ini->fp);

            // // Debug statements
            // printf("Sleep time is: %d ms\n", ini->msToProcess);
            // printf("nbytes_rx = %d, nbytes_tx = %d\n", nbytes_rx, nbytes_tx);
        }
    }

    // Cleanup
    free(outbuf);
    fclose(ini->fp);
    printf("* Exiting \n");
    program_shutdown();
    pthread_join(monitor_thread, NULL);
    return 0;
} 
