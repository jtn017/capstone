/*
* \file sdrxcvr.c
*
* Mods: Ryan Hiser: FMComms4 is a 1x1 and FMComms2 is a 2x2 so 
*       I am modifiying accoridingly. Removed all rx2.
*
*       Also Adding Stuff for TX.
*
* Based off code from Noel Martinez 
* and Javier Arribas, jarribas(at)cttc.es
*/

/*
 * libiio - AD9361 IIO streaming example
 *
 * Copyright (C) 2014 IABG mbH
 * Author: Michael Feilen <feilen_at_iabg.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **/

#include "sdr.h"

/* helper macros */
#define MHZ(x) ((long long)(x*1000000.0 + .5))
#define GHZ(x) ((long long)(x*1000000000.0 + .5))

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
static struct iio_channel *tx0_i = NULL;
static struct iio_channel *tx0_q = NULL;
static struct iio_buffer  *txbuf = NULL;


static bool stop;
static bool app_running = true;
static struct xflow_pthread_data {
	struct iio_device *dev;
};

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
	 if (v < 0) { fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n", v, what); program_shutdown(); }
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
	switch (d) {
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
static bool get_ad9361_stream_ch(struct iio_context *ctx, enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn)
{
	*chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
	if (!*chn)
		*chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
	return *chn != NULL;
}

/* finds AD9361 phy IIO configuration channel with id chid */
static bool get_phy_chan(struct iio_context *ctx, enum iodev d, int chid, struct iio_channel **chn)
{
	switch (d) {
		case TX: 
			*chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("voltage", chid), false); 
			return *chn != NULL;
		case RX: 
			*chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("voltage", chid), false); 
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
bool cfg_ad9361_streaming_ch(struct iio_context *ctx, struct stream_cfg *cfg, enum iodev type, int chid)
{
	struct iio_channel *chn = NULL;

	// Configure phy and lo channels
	printf("    + Acquiring AD9361 phy channel %d\n", chid);
	if (!get_phy_chan(ctx, type, chid, &chn)) {	return false; }
	wr_ch_str(chn, "rf_port_select",     cfg->rfport);
	wr_ch_lli(chn, "rf_bandwidth",       cfg->bw_hz);
	wr_ch_lli(chn, "sampling_frequency", cfg->fs_hz);
	//wr_ch_lli(chn, "rf_dc_offset_tracking_en", cfg->rf_dc_offset_tracking_);
	//wr_ch_lli(chn, "bb_dc_offset_tracking_en", cfg->bb_dc_offset_tracking_);
	//wr_ch_lli(chn, "quadrature_tracking_en", cfg->quadrature_tracking_);
	//wr_ch_lli(chn, "of_node/adi,xo-disable-use-ext-refclk-enable", cfg->ref_clock_);
	//wr_ch_str(chn, "in_voltage0_gain_control_mode", cfg->gain_mode_rx1_);
	//wr_ch_str(chn, "in_voltage1_gain_control_mode", cfg->gain_mode_rx2_);
	//wr_ch_str(chn, "calib_mode", cfg->gain_mode_rx2_);
	//wr_ch_lli(chn, "xo_correction",10000000);

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
	 ret = iio_device_attr_write_double(ad9361_phy, "in_voltage0_hardwaregain", cfg->rf_gain_rx1_);
    if (ret < 0)
        {
            printf(stderr,"Failed to set in_voltage0_hardwaregain: %d\n",ret );
        }
    /*ret = iio_device_attr_write_double(ad9361_phy, "in_voltage1_hardwaregain", cfg->rf_gain_rx2_);
    if (ret < 0)
        {
            printf(stderr,"Failed to set in_voltage1_hardwaregain: %d\n",ret);
        }*/

	printf("--- END OF FMCOMMS4 RX CONFIGURATION ---\n\n");
	return true;


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

/* simple configuration and streaming */
extern int startxcvr(sdrini_t *ini, sdrstat_t *stat)
{
	
	pthread_t monitor_thread;//thread to monitor overflows/underflows
	static struct xflow_pthread_data xflow_pthread_data;
	int ret;

	// Streaming devices
	struct iio_device *tx;
	struct iio_device *rx;

	// Stream configurations
	struct stream_cfg rxcfg;
	struct stream_cfg txcfg;

	// Listen to ctrl+c and ASSERT
	signal(SIGINT, handle_sig);

	// RX stream config
	printf("RX Configuration:\n");
	rxcfg.bw_hz = (long long)ini->f_bw;   
	rxcfg.fs_hz = (long long)ini->f_sf;   // max sample rate
	rxcfg.lo_hz = (long long)ini->f_cf; 
	printf(" - SAMPLING RATE: %lu Hz\n", rxcfg.fs_hz);
	printf(" - Center Freq: %lu Hz\n", rxcfg.lo_hz);
	printf(" - Rf Bandwidth: %lu Hz\n", rxcfg.bw_hz);
	rxcfg.rfport = "A_BALANCED"; // port A (select for rf freq.)
	rxcfg.rf_gain_rx1_=ini->hardware_gain;
	rxcfg.gain_mode_rx1_=ini->gain_control_mode;
	rxcfg.bb_dc_offset_tracking_=ini->bb_dc_offset_tracking;
	rxcfg.quadrature_tracking_=ini->quadrature_tracking;
	rxcfg.rf_dc_offset_tracking_=ini->rf_dc_offset_tracking;
	rxcfg.ref_clock_=ini->ref_clock;

	// Tx stream config
	printf("TX Configuration:\n");
	txcfg.bw_hz = (long long)ini->f_bw;
	txcfg.fs_hz = (long long)ini->f_sf;
	txcfg.lo_hz = (long long)ini->f_cf; 
	txcfg.rfport = "A_BALANCED"; // port A (select for rf freq.)
	printf(" - SAMPLING RATE: %lu Hz\n", txcfg.fs_hz);
	printf(" - Center Freq: %lu Hz\n", txcfg.lo_hz);
	printf(" - Rf Bandwidth: %lu Hz\n", txcfg.bw_hz);
	printf("\n\n");
	
	printf("* Acquiring IIO context\n");
	ASSERT((ctx = iio_create_default_context()) && "No context");
	ASSERT(iio_context_get_devices_count(ctx) > 1 && "No devices");

	printf("* Acquiring AD9361 streaming devices\n");
	printf("  ~ RX\n");
	ASSERT(get_ad9361_stream_dev(ctx, RX, &rx) && "No rx dev found");
	printf("  ~ TX\n");
	ASSERT(get_ad9361_stream_dev(ctx, TX, &tx) && "No tx dev found");

	printf("* Configuring AD9361 for streaming\n");
	printf("  ~ RX\n");
	ASSERT(cfg_ad9361_streaming_ch(ctx, &rxcfg, RX, 0) && "RX port 0 not found");
	printf("  ~ TX\n");
	ASSERT(cfg_ad9361_streaming_ch(ctx, &txcfg, TX, 0) && "TX port 0 not found");

	printf("* Initializing AD9361 IIO streaming channels\n");
	// if(ini->dtype==DTYPEIQ)
	// {
	printf("* Enableling i and q channels.\n");
	ASSERT(get_ad9361_stream_ch(ctx, TX, tx, 0, &tx0_i) && "TX chan i not found");
	ASSERT(get_ad9361_stream_ch(ctx, TX, tx, 1, &tx0_q) && "TX chan q not found");
	ASSERT(get_ad9361_stream_ch(ctx, RX, rx, 0, &rx0_i) && "RX chan i not found");
	ASSERT(get_ad9361_stream_ch(ctx, RX, rx, 1, &rx0_q) && "RX chan q not found");

	printf("* Enabling IIO streaming channels\n");
	iio_channel_enable(tx0_i);
	iio_channel_enable(tx0_q);
	iio_channel_enable(rx0_i);
	iio_channel_enable(rx0_q);
	
	printf("* Center Freq: %f Hz\n", ini->f_cf);
	printf("* Sampling Freq: %f Hz\n", ini->f_sf);
	printf("* Creating non-cyclic buffer\n");
	
	int16_t sampletime = ini->msToProcess/1E3;
	
	
    stat->buffer_size=ini->buffer_size*4; // 2^22, cant go higher
	printf("BUFFERSIZE = %d\n",stat->buffer_size);

	/* numbuf is the number of kernel buffers that we set, 
	   and also the number of times we write to file, 
	   total bytes written to file = numbuf*buffersize*datasize*datatype 
	*/
	int numbuf = ceil((ini->f_sf * ini->dtype * ini->datasize * sampletime)/(stat->buffer_size * ini->dtype * ini->datasize));
	int i;
	i = iio_device_set_kernel_buffers_count(rx,numbuf);//one kernel buffer per loop iteration, not sure how kernel buffers count does ?
	
	if(i !=0 ) { 
		printf("failed to set kernel buffers count.\n");
	}
	printf("Number of kernel buffers set is: %d\n", numbuf);

	rxbuf = iio_device_create_buffer(rx,stat->buffer_size,false); 
	if (!rxbuf){
		perror("ERROR: Could not create RX buffer");
		program_shutdown();
	}
	
	txbuf = iio_device_create_buffer(tx, stat->buffer_size,false);
	if (!txbuf) {
		perror("ERROR: Could not create TX buffer");
		program_shutdown();
	}

	//
	//DATA ACQUISITION
	//
	printf("* Starting IO streaming (press CTRL+C to cancel)\n" );

	printf("number of bytes attempting to allocate for output buffer is: %lu\n",stat->buffer_size * ini->dtype * ini->datasize); 

	void *outbuf;
	outbuf =(int16_t*)malloc(stat->buffer_size*ini->dtype*ini->datasize); //*2 for I and Q data, *2 for int16.

	void *sendbuf;
	sendbuf =(int16_t*)malloc(stat->buffer_size*ini->dtype*ini->datasize); //*2 for I and Q data, *2 for int16.

	
	if(outbuf == NULL){
		 printf("Failed to allocate memory for output buffer!\n");
	}
	
	if( (ini->fp= fopen(ini->dump_file,"wb")) == NULL){
		printf("FAILED to open file for writing !!!\n");
		return -1; 
	}

	xflow_pthread_data.dev=rx;
	ret = pthread_create(&monitor_thread, NULL, monitor_thread_fn,(void *)&xflow_pthread_data);
	if(ret){ 
		printf(stderr, "Failed to crate monitor thread: %s\n",
		strerror(-ret));
	}
				
	printf("* Writing to File \n");

	ssize_t nbytes_rx, nbytes_tx;
	char *p_start, *p_dat, *p_end;
	ptrdiff_t p_inc;
	
	for (int j=0;j<numbuf;j++)
	{
		
		/*
		/ fill TX data -- Method 2 --
		short cnt = 0;
		for(int idx = 0; idx < (stat->buffer_size*ini->dtype); idx++){}
			sendbuf[i] = cnt << 4;
			if(cnt > 2048)
				cnt = -2048;
		} 

		// WRITE: Get pointers to TX buf and write IQ to TX buf port 0
		p_start = iio_buffer_first(txbuf,tx0_i);
		memmove(p_start,(int16_t*)&outbuf[0],nbytes_tx);
		*/
		
		// fill TX data -- Method 1 --
		p_inc = iio_buffer_step(txbuf);
		p_end = iio_buffer_end(txbuf);
		int16_t cnt = 0; // count for dummy data
		for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
			// Example: fill with zeros
			((int16_t*)p_dat)[0] = cnt << 4; // Real (I)
			((int16_t*)p_dat)[1] = cnt << 4; // Imag (Q)
			cnt += 5;
			if(cnt >= 1024)
				cnt = 0;
			
		}

		// Schecule TX Buffer
		nbytes_tx = iio_buffer_push(txbuf);
		if (nbytes_tx < 0) { 
			printf("Error scheduling tx buf %d\n", (int) nbytes_tx);
			program_shutdown(); 
		}

		// Lets Delay (so buffer is full before reading)
		if(j == 0)
			sleep(2);

		// Refill RX buffer
		nbytes_rx;
		nbytes_rx = iio_buffer_refill(rxbuf);
		if (nbytes_rx < 0) { 
			printf("Error refilling rx buf %d\n",(int) nbytes_rx);
			program_shutdown(); 
		}
		
		// READ: Get pointers to RX buf and read IQ from RX buf port 0
		p_start = iio_buffer_first(rxbuf,rx0_i);
		memmove((int16_t*)&outbuf[0],p_start,nbytes_rx); 
		fwrite(outbuf,ini->datasize,(stat->buffer_size*ini->dtype),ini->fp);
		fflush(ini->fp); 
		
	}
	free(sendbuf);
	free(outbuf);
	fclose(ini->fp);
	printf("* Exiting \n");
	program_shutdown();
	pthread_join(monitor_thread, NULL);
	return 0;
} 

