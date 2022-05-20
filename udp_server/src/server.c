// server code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Include socket/http headers and curl
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <curl/curl.h>

#include "v2x_datatypes.h"

void send_packet_esp8266(char * str);
void tx_payload_wifimodule2(struct payload_struct * pyld);

// driver code
int main()
{
	int sockfd, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = INADDR_ANY;

	unsigned int frame_size = TX_BB_IN_BYTES;
	char cur_frame[frame_size];

	struct payload_struct pyld;

	// socket()
	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

	if (sockfd < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", sockfd);

	// bind()
	if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0)
		printf("\nSuccessfully binded!\n");
	else{
		printf("\nBinding Failed!\n");
		exit(-1);
	}

	for(int i = 0; i< NUM_FRAMES; i++)
	{
		nBytes = recvfrom(sockfd, cur_frame, frame_size,sendrecvflag, (struct sockaddr*)&addr_con,&addrlen);

		memcpy(&pyld, &cur_frame[0], 14);
    	memcpy(&pyld.dist_next_step, &cur_frame[14], 4);

		tx_payload_wifimodule2(&pyld);

	}

	close(sockfd);

	return 0;
}

void tx_payload_wifimodule2(struct payload_struct * pyld){
  
    const char ip_addr[] = IP_ADDR;

    char str1[200];
    char str2[200];
    char str3[200];
    char str4[200];
    char str5[200];
    char str6[200];

    // Reset char string structure to 0
    struct char_strings strs;
    memset(&strs, 0, sizeof(strs));

    // Copy every member of payload into string arrays
    memcpy(&strs.name, pyld->name, 4);
    gcvt(pyld->lat, N_FLOAT_DIGITS, strs.lat);
    gcvt(pyld->lon, N_FLOAT_DIGITS, strs.lon);
    snprintf( strs.speed, N_FLOAT_DIGITS, "%d", pyld->speed );
    snprintf( strs.dir, N_FLOAT_DIGITS, "%d", pyld->dir );
    gcvt(pyld->dist_next_step, N_FLOAT_DIGITS, strs.dist_next_step);

    /*
    printf("%s\n",strs.name);
    printf("%s\n",strs.lat);
    printf("%s\n",strs.lon);
    printf("%s\n",strs.speed);
    printf("%s\n",strs.dir);
    printf("%s\n",strs.dist_next_step);
    */

    // Create HTTP GET requests(URL path and query)
    sprintf(str1, "%s/name?value=%s",           ip_addr, strs.name);
    sprintf(str2, "%s/lat?value=%s",            ip_addr, strs.lat);
    sprintf(str3, "%s/lon?value=%s",            ip_addr, strs.lon);
    sprintf(str4, "%s/speed?value=%s",          ip_addr, strs.speed);
    sprintf(str5, "%s/dir?value=%s",            ip_addr, strs.dir);
    sprintf(str6, "%s/dist_next_step?value=%s", ip_addr, strs.dist_next_step);

#if 0
    printf("%s\n",str1);
    printf("%s\n",str2);
    printf("%s\n",str3);
    printf("%s\n",str4);
    printf("%s\n",str5);
    printf("%s\n",str6);
    printf("\n");
#else
    send_packet_esp8266(str1);
    send_packet_esp8266(str2);
    send_packet_esp8266(str3);
    send_packet_esp8266(str4);
    send_packet_esp8266(str5);
    send_packet_esp8266(str6);
#endif

    return;

}


void send_packet_esp8266(char * str){

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, str);

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK)
		{
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
	        curl_easy_strerror(res));
		}

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

}

