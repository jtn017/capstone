/*
 * General V2X SDR header file
 */
#ifndef V2X_SDR_H
#define V2X_SDR_H


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <inttypes.h>
#include "complex.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <getopt.h>

#ifdef __APPLE__
#include <iio/iio.h>
#else
#include <iio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MHZ(x) 		((long long)(x*1000000.0 + .5))
#define GHZ(x) 		((long long)(x*1000000000.0 + .5))
#define ROUND(X) 	((int)floor((X)+0.5))
#define PI		3.1415926536897932
#define DPI		(2*PI)
#define D2R		(PI/180.0)
#define R2D		(180.0/PI)
#define CLIGHT		2997942458.0
#define	ON		1
#define	OFF		0
/* fornt end settings*/
#define FEND_FILE	1	/* Front end file signal source */
#define FEND_RAPTOR	2	/* Front end raptor signal source */
#define DTYPEI		1	/* smapling type: real   */
#define DTYPEIQ		2	/* smapling type: real+imag   */
#define LEN_L1CA	1023
#define CRATE_L1CA	1.023E6
#define MAXGPSSATNO	32
#define NFFTTHREAD	4
/* thread functions */
#define mlock_t		pthread_mutex_t
#define initmlock(f)	pthread_mutex_init(&f,NULL)
#define mlock(f)	pthread_mutex_lock(&f)
#define unmlock(f)	pthread_mutex_unlock(&f)
#define delmlock(f)	pthread_mutex_destroy(&f)
#define event_t		pthread_cond_t
#define initevent(f)	pthread_cond_init(&f,NULL)
#define setevent(f)	pthread_cond_signal(&f)
#define waitevent(f,m)	pthread_cond_wait(&f,&m)
#define delevent(f)	pthread_cond_destroy(&f)
#define waitthread(f)	pthread_join(f,NULL)
#define createthread(f,func,arg)	pthread_create(&f,NULL,func,arg)
#define THRETVAL	NULL

typedef pthread_t thread_t;
/* sdr initialization struct */

typedef struct {
	int fend;	/* front-end type, starting with file only */
	double f_cf;	/* center frequency (GHz) 1.57542 GHz */
	double f_sf; 	/* sampling frequency (MHz) */
	double f_if; 	/* intermediate frequency (MHz) */
	double f_bw; 	/* intermediate frequency (MHz) */
	int buffer_size; 	/* buffersize Bytes*/
	double ts;        /* sampling period*/
	int dtype;	/* data type 2=IQ 1= Real */
	int datasize;
	FILE *fp;	/* file pointer */
	char dump_file[1024];	/* file path */
	int msToProcess; //seconds samples
	int skipnb;
	int nch;
	int ref_clock;	
	double rx_hardware_gain;
	double tx_hardware_gain;
	int bb_dc_offset_tracking;	
	int quadrature_tracking;	
	int rf_dc_offset_tracking;
	char rx_gain_control_mode[50];
	uint32_t rx_ctl_start_agc;
	uint32_t rx_ctl_corr_thresh;
	uint32_t rx_ctl_agc_pow_ref;
	uint32_t rx_ctl_store_delay;
	uint32_t rx_ctl_time_sel;
} sdrini_t;

typedef struct {
	int stopflag;
	unsigned long long int buffer_size;
	unsigned long long int acq_buffer_size;
	unsigned long long int trk_buffer_size;
	int fendbuffsize;
	int16_t *buff;
	int16_t *signal2;
	int16_t *signal1;
	uint64_t buffcnt;
	int datasize;
	float complex *data;
} sdrstat_t;

extern sdrini_t sdrini;       /* sdr initialization struct */
extern sdrstat_t sdrstat;       /* sdr initialization struct */

#ifdef __cplusplus
}
#endif
#endif // V2X_SDR_H
