/********************************************************************************
 * @file     tb_rx.v
 * @author   V2X Motorcycle Team
 * @date     
 * @desc     Test Bench For RX Signal Processing which will integrate
 *           with fmcomms HDL
 *
 * @param    
 *
 * @notes    
 *
 ********************************************************************************/
 
`timescale 1ns / 1ps
module tb_rx;

parameter NSPAMPS = 10000;
parameter PAY_LEN = 3000*2;

/****************************************
 * Input/Output
****************************************/
reg clk;
reg sys_clk;
reg axi_clk;

reg rst;

reg [15:0] input_i;
reg [15:0] input_q;
reg input_vld;

wire [15:0] output_i;
wire [15:0] output_q;
wire output_vld;

reg agc_en_n = 1'b1;
reg start;
/****************************************
 * UUT
****************************************/
rx uut(
    // Inputs
    .i_sysclk(sys_clk),
    .i_clk(clk),      
    .i_rst(rst),
    // Control Signals
    .i_ctrl({29'd0,1'b1,1'b0,1'b0,agc_en_n,start}),
    .i_thresh('d50000),//400,000 
    .i_pow_ref('d768),
    .i_store_dly('h3),
    .i_time_sel('d5),
    // ADC Data Input (RX)
    .i_fromADC_i(input_i),
    .i_fromADC_q(input_q),
    .i_fromADC_vld(input_vld),
    // Receiver Data
    .axi_clk(axi_clk),
    .axi_rstn(~rst)
); 

/****************************************
 * 32MHz ADC = 32/8=4MHz
****************************************/
initial begin
    clk = 1'b0;
    forever begin
	    #15.625 clk = ~clk;
	end
end

/****************************************
 * 200MHz Sys Clock
****************************************/
initial begin
    sys_clk = 1'b0;
    forever begin
	   #2.5 sys_clk = ~sys_clk;
	end
end

/****************************************
 * 100MHz AXI PS Sys Clock
****************************************/
initial begin
    axi_clk = 1'b0;
    forever begin
	   #5 axi_clk = ~axi_clk;
	end
end
/****************************************
 * Main Loop Signal Input Genertation
****************************************/
integer outfile;
integer idx, delay;
integer fd, fd2, fres,in_idx;
integer fd_i, fd_q;

reg [15:0] temp_input_i;
reg [15:0] temp_input_q;
reg [15:0] gold_output_i;
reg [15:0] gold_output_q;

//reg [15:0] payload [0:PAY_LEN-1];
reg [15:0] payload_i [0:PAY_LEN-1];
reg [15:0] payload_q [0:PAY_LEN-1];
reg [15:0] gold_out [0:PAY_LEN-1];

parameter TOPBIT = 'd15;
initial begin
    $display("Starting!");
    outfile = $fopen("tb_rx_output.txt","w");
    //fd = $fopen("tb_rx_input.dat","rb"); // matlab 'short','ieee-be'
    fd_i = $fopen("v2x_tx_mod_out_16bit_imag.bin","rb"); // matlab 'short','ieee-be'
    fd_q = $fopen("v2x_tx_mod_out_16bit_real.bin","rb"); // matlab 'short','ieee-be'
    //fres = $fread(payload,fd);// Read in Data
    fres = $fread(payload_i,fd_i);// Read in Data
    fres = $fread(payload_q,fd_q);// Read in Data
    // bypass Selections
    
    // *** Start Main Test ***
    input_i = 'd0;
    input_q = 'd0;
    rst = 1'b0;
    input_vld = 1'b0;
    start = 1'b0;
    // Apply reset
    @(posedge clk) rst = 1'b1;
    @(posedge clk) rst = 1'b1;
    @(posedge clk) rst = 1'b0;
    @(posedge clk) start = 1'b1;
    
    // Inject Data
    //for(idx=0; idx < PAY_LEN-1; idx = idx+2)begin
    for(idx=0; idx < PAY_LEN-1; idx = idx+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            temp_input_i = {payload_i[idx+0][7:0],payload_i[idx+0][15:8]};
            temp_input_q = {payload_q[idx+0][7:0],payload_q[idx+0][15:8]};
            input_i = $signed(temp_input_i[14-:12])*4;
            input_q = $signed(temp_input_q[14-:12])*4;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end

    // Pause signal input for a while
    for(delay=0; delay < 100-1; delay = delay+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            input_i = 'd0;
            input_q = 'd0;
            input_vld = 1'b0;
        end
    end
   
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b1;

    // Inject Data (SOME ZEROS FIRST)
    for(delay=0; delay < 20-1; delay = delay+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            input_i = 'd0;
            input_q = 'd0;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
    // Inject Data
    for(idx=0; idx < PAY_LEN-1; idx = idx+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            temp_input_i = {payload_i[idx+0][7:0],payload_i[idx+0][15:8]};
            temp_input_q = {payload_q[idx+0][7:0],payload_q[idx+0][15:8]};
            input_i = $signed(temp_input_i[14-:12])*2;
            input_q = $signed(temp_input_q[14-:12])*2;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
    // Pause signal input for a while
    for(delay=0; delay < 100-1; delay = delay+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            input_i = 'd0;
            input_q = 'd0;
            input_vld = 1'b0;
        end
    end
    
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b1;
    
    // Inject Data (SOME ZEROS FIRST)
    for(delay=0; delay < 100-1; delay = delay+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            input_i = 'd0;
            input_q = 'd0;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
    //for(idx=0; idx < PAY_LEN-1; idx = idx+2)begin
    for(idx=0; idx < PAY_LEN-1; idx = idx+1)begin
        @(posedge clk)  begin 
            temp_input_i = {payload_i[idx+0][7:0],payload_i[idx+0][15:8]};
            temp_input_q = {payload_q[idx+0][7:0],payload_q[idx+0][15:8]};
            input_i = $signed(temp_input_i[14-:12])*2.5;
            input_q = $signed(temp_input_q[14-:12])*2.5;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
        @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b0;
    @(posedge clk) start = 1'b1;
    
    // Inject Data (SOME ZEROS FIRST)
    for(delay=0; delay < 133+4-1; delay = delay+1)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            input_i = 'd0;
            input_q = 'd0;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
    //for(idx=0; idx < PAY_LEN-1; idx = idx+2)begin
    for(idx=0; idx < PAY_LEN-1; idx = idx+1)begin
        @(posedge clk)  begin 
            temp_input_i = {payload_i[idx+0][7:0],payload_i[idx+0][15:8]};
            temp_input_q = {payload_q[idx+0][7:0],payload_q[idx+0][15:8]};
            input_i = $signed(temp_input_i[14-:12])*2;
            input_q = $signed(temp_input_q[14-:12])*2;
            input_vld = 1'b1;
        end
        @(posedge clk)input_vld = 1'b0;
        @(posedge clk);
        @(posedge clk);
        @(posedge clk);
        @(posedge clk); 
        @(posedge clk);
        @(posedge clk);
    end
    
    $display("At end. File Closed. Finished!");
    $fclose(outfile);
    $fclose(fd_i);
    $fclose(fd_q);
    //$fclose(fd);
    $finish;
end

endmodule