
void data2ps(unsigned int input,bool addr_rst, bool *rdy, unsigned int *version, unsigned int output[64])
{
#pragma HLS INTERFACE ap_vld port=input
#pragma HLS INTERFACE s_axilite port=version bundle=BUS_A
#pragma HLS INTERFACE s_axilite port=rdy bundle=BUS_A
#pragma HLS INTERFACE s_axilite port=output bundle=BUS_A

	static unsigned int mem[64];
	static int addr = 0;
#pragma HLS RESET variable=addr


	if(addr_rst)
		addr = 0;

	if(addr < 64){ // only increment when new data
		mem[addr] = input; // store input
		addr=addr+1;
	}

	for(int k = 0; k<64; k++){ // update outputs
		output[k] = mem[k];
	};


	*version = 0x20220516;
	*rdy = addr >= 64;// Read all needed inputs
}
