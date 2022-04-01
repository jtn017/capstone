/*------------------------------------------------------------------------------
* sdrinit.c : SDR initialize/cleanup functions
*
* noel martinez
*-----------------------------------------------------------------------------*/
#include "sdr.h"

/* read ini file -------------------------------------------------------------*/

/* functions used in read ini file ---------------------------------------------
* initiializes variables from config file
*-----------------------------------------------------------------------------*/
static int GetFileAttributes(const char *file)
{
	    FILE *fp;
	        if (!(fp=fopen(file,"r"))) return -1;
		    fclose(fp);
		        return 0;
}


static void GetPrivateProfileString(const char *sec, const char *key,
		    const char *def, char *str, int len, const char *file)
{
	FILE *fp;
	char buff[1024],*p,*q;
	int enter=0;

	strncpy(str,def,len-1); str[len-1]='\0';
	if (!(fp=fopen(file,"r"))) {
		fprintf(stderr,"ini file open error [%s]\n",file);
		return;
	}
	while (fgets(buff,sizeof(buff),fp)) {
		if ((p=strchr(buff,';'))) *p='\0';
		if ((p=strchr(buff,'['))&&(q=strchr(p+1,']'))) {
			*q='\0';
			enter=!strcmp(p+1,sec);
		}
		else if (enter&&(p=strchr(buff,'='))) {
			*p='\0';
			for (q=p-1;q>=buff&&(*q==' '||*q=='\t');) *q--='\0';
			if (strcmp(buff,key)) continue;
			for (q=p+1+strlen(p+1)-1;q>=p+1&&(*q=='\r'||*q=='\n');) *q--='\0';
		strncpy(str,p+1,len-1); str[len-1]='\0';
		break;
		}
    }
    fclose(fp);
}

int splitint(char *src, char *dlm, int *out, int n)
{
    int i;
    char *str;
    for (i=0;i<n;i++) {
        if ((str=strtok((i==0)?src:NULL,dlm))==NULL) return -1;
        out[i]=atoi(str);
    }
    return 0;
}
int splitdouble(char *src, char *dlm, double *out, int n)
{
    int i;
    char *str;
    for (i=0;i<n;i++) {
        if ((str=strtok((i==0)?src:NULL,dlm))==NULL) return -1;
        out[i]=atof(str);
    }
    return 0;
}
int readiniint(char *file, char *sec, char *key)
{
    char str[256];
    GetPrivateProfileString(sec,key,"",str,256,file);
    return atoi(str);
}
int readiniints(char *file, char *sec, char *key, int *out, int n)
{
    char str[256];
    GetPrivateProfileString(sec,key,"",str,256,file);
    return splitint(str,",",out,n);
}
double readinidouble(char *file, char *sec, char *key)
{
    char str[256];
    GetPrivateProfileString(sec,key,"",str,256,file);
    return atof(str);
}
int readinidoubles(char *file, char *sec, char *key, double *out, int n)
{
    char str[256];
    GetPrivateProfileString(sec,key,"",str,256,file);
    return splitdouble(str,",",out,n);
}
void readinistr(char *file, char *sec, char *key, char *out)
{
    GetPrivateProfileString(sec,key,"",out,256,file);
}
/* read ini file ---------------------------------------------------------------
* read ini file and set value to sdrini struct
* args   : sdrini_t *ini    I/0 sdrini struct
* return : int                  0:okay -1:error
* note : this function is only used in CLI application
*-----------------------------------------------------------------------------*/
extern int readinifile(sdrini_t *ini)
{
    int i,ret;
    char inifile[]="../conf/master.ini";
    char fendfile[256],str[256];

    /* check ini file */
    if ((ret=GetFileAttributes(inifile))<0){
        printf("error: gnss-sdrcli.ini doesn't exist\n");
        return -1;
    }
    /* receiver setting */
    readinistr(inifile,"RCV","FENDCONF",fendfile);

    /* check front-end configuration  file */
    if ((ret=GetFileAttributes(fendfile))<0){
        printf("error: %s doesn't exist\n",fendfile);
        return -1;
    }
    readinistr(fendfile,"FEND","TYPE",str);
    if (strcmp(str,"FILE")==0)       
        ini->fend=FEND_FILE;
    else if (strcmp(str,"FMCOMMS2")==0)       
        ini->fend=FEND_RAPTOR;
    else { 
        printf("error: wrong frontend type: %s\n",str); 
        return -1; 
    }
    if (ini->fend==FEND_RAPTOR) {
        readinistr(fendfile,"FEND","FILEDUMP",ini->dump_file);
    }

    ini->f_cf=GHZ(readinidouble(fendfile,"FEND","CF1"));
    ini->msToProcess=readiniint(fendfile,"FEND","MILISECONDS");
    ini->f_sf=MHZ(readinidouble(fendfile,"FEND","SF1"));
    ini->buffer_size=readiniint(fendfile,"FEND","BUFFERSIZE");
    ini->bb_dc_offset_tracking=readiniint(fendfile,"FEND","BB_DC_OFFSET_TRACKING");
    ini->quadrature_tracking=readiniint(fendfile,"FEND","QUADRATURE_TRACKING");
    ini->rf_dc_offset_tracking=readiniint(fendfile,"FEND","RF_DC_OFFSET_TRACKING");
    ini->ref_clock=readiniint(fendfile,"FEND","CLOCK");
    ini->hardware_gain=readinidouble(fendfile,"FEND","GAIN");
    readinistr(fendfile,"FEND","GAIN_CONTROL_MODE",ini->gain_control_mode);
    ini->f_bw=MHZ(readinidouble(fendfile,"FEND","BW1"));
    ini->f_if=readinidouble(fendfile,"FEND","IF1");
    ini->dtype=readiniint(fendfile,"FEND","DTYPE1");
    ini->datasize=readiniint(fendfile,"FEND","DATASIZE");
 
}
/* check initial value ---------------------------------------------------------
* checking value in sdrini struct
* args   : sdrini_t *ini    I   sdrini struct
* return : int                  0:okay -1:error
*-----------------------------------------------------------------------------*/
extern int chk_initvalue(sdrini_t *ini)
{
    int ret;

    /* checking frequency input */
    if ((ini->f_sf<=0||ini->f_sf>100e6) ||
        (ini->f_if<0 ||ini->f_if>100e6)) {
            printf("error: wrong freq. input sf1: %.0f if1: %.0f\n",
                ini->f_sf,ini->f_if);
            return -1;
    }

    printf("PRINTING SAMPLING FREQ IN initcalue CHECK: %f\n",ini->f_sf);

    return 0;
}








