/*
 * V2X SDR initialize functions
 */

// Includes
#include "v2x_sdr.h"
#include "v2x_sdr_init.h"

// ---------------------- Read INI file helper functions ----------------------
static int GetFileAttributes(const char *file)
{
    // Open file
    FILE *fp;
    if (!(fp=fopen(file,"r")))
    {
        return -1;
    }

    // No issues
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

// ---------------------- Read and run from ini file ----------------------
int readinifile(sdrini_t *ini)
{
    int ret;
    char inifile[]="conf/master.ini";
    char fendfile[256],str[256];

    /* check ini file */
    if ((ret=GetFileAttributes(inifile))<0){
        printf("error: gnss-sdrcli.ini doesn't exist\n");
        return ret;
    }
    /* receiver setting */
    readinistr(inifile,"RCV","FENDCONF",fendfile);

    /* check front-end configuration  file */
    if ((ret=GetFileAttributes(fendfile))<0){
        printf("error: %s doesn't exist\n",fendfile);
        return ret;
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
    ini->msToProcess=readiniint(fendfile,"FEND","MILLISECONDS");
    ini->f_sf=MHZ(readinidouble(fendfile,"FEND","SF1"));
    ini->buffer_size=readiniint(fendfile,"FEND","BUFFERSIZE");
    ini->bb_dc_offset_tracking=readiniint(fendfile,"FEND","BB_DC_OFFSET_TRACKING");
    ini->quadrature_tracking=readiniint(fendfile,"FEND","QUADRATURE_TRACKING");
    ini->rf_dc_offset_tracking=readiniint(fendfile,"FEND","RF_DC_OFFSET_TRACKING");
    ini->ref_clock=readiniint(fendfile,"FEND","CLOCK");
    ini->rx_hardware_gain=readinidouble(fendfile,"FEND","RX_GAIN");
    ini->tx_hardware_gain=readinidouble(fendfile,"FEND","TX_GAIN");
    readinistr(fendfile,"FEND","RX_GAIN_CONTROL_MODE",ini->rx_gain_control_mode);
    ini->f_bw=MHZ(readinidouble(fendfile,"FEND","BW1"));
    ini->f_if=readinidouble(fendfile,"FEND","IF1");
    ini->dtype=readiniint(fendfile,"FEND","DTYPE1");
    ini->datasize=readiniint(fendfile,"FEND","DATASIZE");

    // No issues
    return 0;
}

// ---------------------- Check initial value ----------------------
int chk_initvalue(sdrini_t *ini)
{
    // Checking frequency input
    if ((ini->f_sf<=0||ini->f_sf>100e6) ||
        (ini->f_if<0 ||ini->f_if>100e6))
    {
        printf("error: wrong freq. input sf1: %.0f if1: %.0f\n",
                ini->f_sf,ini->f_if);
        return -1;
    }

    // No issues
    printf("PRINTING SAMPLING FREQ IN initcalue CHECK: %f\n",ini->f_sf);
    return 0;
}
