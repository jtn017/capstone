// client code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0

#define NUM_FRAMES 5
#define TX_BB_IN_BITS 784
#define TX_BB_IN_BYTES (TX_BB_IN_BITS/8)

// function to clear buffer
void clearBuf(char* b)
{
	int i;
	for (i = 0; i < NET_BUF_SIZE; i++)
		b[i] = '\0';
}

// function for decryption
char Cipher(char ch)
{
	return ch ^ cipherKey;
}

// function to receive file
int recvFile(char* buf, int s)
{
	int i;
	char ch;
	for (i = 0; i < s; i++) {
		ch = buf[i];
		ch = Cipher(ch);
		if (ch == EOF)
			return 1;
		else
			printf("%c", ch);
	}
	return 0;
}

void read_data_into_buffer(const char* filename, char* data, unsigned int num_frames);

// driver code
int main()
{

	const char input_file[] = "../models/main/data/v2x_rx_bb_out.bin";

	unsigned int num_frames = NUM_FRAMES;
	unsigned int frame_size = TX_BB_IN_BYTES;
	unsigned int payload_size = frame_size*num_frames;
    char buffer[payload_size];
	char cur_frame[frame_size];

	read_data_into_buffer(input_file, buffer, num_frames);

	int sockfd, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	char net_buf[NET_BUF_SIZE];

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

	// while (1) 
	for(int i = 0; i< num_frames; i++)
	{
		memcpy(&cur_frame[0], &buffer[i*frame_size], frame_size);

 		printf("have new frame!\n");

		// printf("\nPlease enter file name to receive:\n");
		// scanf("%s", net_buf);
		sendto(sockfd, cur_frame, frame_size, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);

		printf("\n---------Data Sent---------\n");

	}

	return 0;
}

void read_data_into_buffer(const char* filename, char* data, unsigned int num_frames)
{

    // Reading char from file
	unsigned int payload_size = TX_BB_IN_BYTES*num_frames;
	int ret_val;
    //char buffer[payload_size*num_frames];
    FILE * bin_file = fopen(filename, "rb");
    ret_val = fread(data, payload_size, 1, bin_file);

	// Close file
    fclose(bin_file);

	if(ret_val != 1){
		printf("Error, couldn't read all file contents!\n");
		exit(-1);
	}
    
}

