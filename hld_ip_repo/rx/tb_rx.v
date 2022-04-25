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
parameter PAY_LEN = 10000*2;

/****************************************
 * Input/Output
****************************************/
reg clk;
reg sys_clk;

reg rst;

reg [11:0] input_i;
reg [11:0] input_q;
reg input_vld;

wire [15:0] output_i;
wire [15:0] output_q;
wire output_vld;

/****************************************
 * UUT
****************************************/
sigproc_top#(
    .CC_GEN('d0),
    .SRRC_GEN('d0)
)
uut(
    // Inputs
    .i_sysclk(sys_clk),
    .i_clk(clk),      
    .i_rst(rst),
    // Control Signals
    .i_ctrl(),
    // ADC Data Input (RX)
    .i_fromADC_i(),
    .i_fromADC_q(),
    .i_fromADC_vld(),
    // Receiver Data
    .o_datarx_i(),
    .o_datarx_q(),
    .o_datarx_vld()
); 

/****************************************
 * 56MHz ADC
****************************************/
initial begin
    clk = 1'b0;
    forever begin
	    #8.92857 clk = ~clk;
	end
end

/****************************************
 * 224MHz ADC
****************************************/
initial begin
    sys_clk = 1'b0;
    forever begin
	   #2.23214 sys_clk = ~sys_clk;
	end
end

/****************************************
 * Main Loop Signal Input Genertation
****************************************/
integer outfile;
integer idx, delay;
integer fd, fd2, fres,in_idx;

reg [15:0] temp_input_i;
reg [15:0] temp_input_q;
reg [15:0] gold_output_i;
reg [15:0] gold_output_q;

reg [15:0] payload [0:PAY_LEN-1];
reg [15:0] gold_out [0:PAY_LEN-1];
    
initial begin
    $display("Starting!");
    outfile = $fopen("tb_rx_output.txt","w");
    fd = $fopen("tb_rx_input.dat","rb"); // matlab 'short','ieee-be'
    
    fres = $fread(payload,fd);// Read in Data
    
    // bypass Selections
    
    // *** Start Main Test ***
    input_i = 'd0;
    input_q = 'd0;
    rst = 1'b0;
    input_vld = 1'b0;
    
    // Apply reset
    @(posedge clk) rst = 1'b1;
    @(posedge clk) rst = 1'b1;
    @(posedge clk) rst = 1'b0;
    
    // Inject Data
    for(idx=0; idx < PAY_LEN-1; idx = idx+2)begin
        @(posedge clk)  begin 
            // Input Data to UUT
            temp_input_i = {payload[idx+0]};
            temp_input_q = {payload[idx+1]};
            input_i = $signed(temp_input_i[11:0]);
            input_q = $signed(temp_input_q[11:0]);
           input_vld = 1'b1;
        end
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
   
    $display("At end. File Closed. Finished!");
    $fclose(outfile);
    $fclose(fd);
    $finish;
end

// *** Record Output ***
integer out_idx = 0;

assign output_vld = 0;
assign output_i = $signed('d0);
assign output_q = $signed('d0);

always begin
    @(posedge clk)  begin 
        if(output_vld)begin
                $fwrite(outfile, "%b,",output_vld); 
                $fwrite(outfile, "%d, ",$signed(output_i)); 
                $fwrite(outfile, "%d\n",$signed(output_q)); 
        end
    end
end


// *** Display Gold Output ***
//integer gold_out_idx = 0;
//integer gidx;

//initial begin
//    gold_output_i = 0;
//    gold_output_q = 0;
//    fd2 = $fopen("tb_rx_gold.dat","rb"); // matlab 'short','ieee-be'
//    $fread(gold_out,fd2);// Read in Data
    
//    for(gidx=0; gidx < 2; gidx = gidx+1) @(posedge clk);
    
//    forever begin
//        @(posedge clk)  begin 
//        gold_output_i = {gold_out[gold_out_idx+0]};
//        gold_output_q = {gold_out[gold_out_idx+1]};
//        gold_out_idx = gold_out_idx + 2'b10;
//        end
//    end
//end

endmodule