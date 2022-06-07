`timescale 1ns / 1ps
/********************************************************************************
 * @file     pll.v
 * @author   V2X Motorcycle Team
 * @date     
 * @desc     Phase Lock Loop implemenation for QPSK signal using Xilinx IP's and 
 *           a no atan version. Implemented based on our HLS version of the code.
 *
 * @param    
 *
 * @notes    
 *
 ********************************************************************************/
 
module pll_v2
(
  // Inputs
  input i_clk,      
  input i_rst_n, 
  input i_sig_vld,
  input [15:0] i_signal_i,
  input [15:0] i_signal_q,

  // Outputs 
  output wire o_vld,
  output wire [16-1:0] o_signal_i,
  output wire [16-1:0] o_signal_q,

  // Debug Ports
  output wire [15:0] dbg_err
); 

reg do_BPSK = 0;

/****************************************
  * Internal Registers and Wires
****************************************/
  
// output of muliply
wire [16*2-1:0] mult_result;
wire mult_valid;

// ouptu of NCO/DDS
wire nco_valid;
wire [31:0] nco_data;

// debug signals (will be pruned)
wire [15:0] sin, cos;

// Interconnections (feed back)
wire [31:0] pll_fb;
reg pll_to_loopf_v;
wire phase_ap_vld;

// Interconnections (Multiplier for BPSK)
wire [31:0] pll_mult;
wire [15:0] pll_bpsk_simple;

// Interconnections (QPSK phase Detect)
wire [16-1:0] pll_qpsk_simple;
wire [15:0] pll_ps_q, pll_ps_i;
wire pll_ps_v;
reg [15:0] pll_to_loopf;
reg [25:0] pll_fb_inc;

/****************************************
  * Numerically Controlled Oscilator
  * Using the DDS compiler v6.0
  *
  * Need to know the system clock?
  * System Clock at a rate 4 samples per symbol.
  *
****************************************/
phase_adj inst_pnco(
  .aclk(i_clk),                               
  .s_axis_phase_tvalid(1'b1),  // all 6 bits are phase in
  .s_axis_phase_tdata({6'b000000,pll_fb_inc,32'd0}),     // MSBs - phase offset, LSBs - phase increment
  .m_axis_data_tvalid(nco_valid),    
  .m_axis_data_tdata(nco_data)      
);

// Split for Debugging (these should be pruned in compile)
assign sin = nco_data[31:16]; // sine is MSBs
assign cos = nco_data[15:0]; // cosine is LSBs

/****************************************
  * Complex Multiply
  * Using Complex Multiplier v6.0
  * 
  * Input width A-16, B-16
  * Output width A-16 truncating lower 16
  *    This is equivalent to a divide
  *    which makes the B amplitude 
  *    back to 1.
  * Fully Pipelined Latency of 6
  *
  *   (Re{A} + j*Im{A})*(Re{B}+j*Im{B})
  * = (in_I +j*in_Q)*(cos+j*sin)
****************************************/

phase_cmpl_mult inst_cmpl_pmult(
  .aclk(i_clk),                             
  .s_axis_a_tvalid(i_sig_vld),                
  .s_axis_a_tdata({i_signal_q,i_signal_i}), 
  .s_axis_b_tvalid(i_sig_vld),       
  .s_axis_b_tdata({sin,cos}),      
  .m_axis_dout_tvalid(mult_valid),
  .m_axis_dout_tdata(mult_result)   
);

assign pll_ps_q = mult_result[31:16]<<2; // MSBs for imaginary part (this is the sin)
assign pll_ps_i = mult_result[15:0]<<2; // LSBs for real part
assign pll_ps_v = mult_valid;

/****************************************
  * Error SIgnal for BPSK or QPSK
****************************************/
// BPSK ERROR or QPSK ERROR
reg [16-1:0] temp1;
reg [16-1:0] temp2;
reg temp_v;

always @(posedge i_clk)begin
    temp_v <= 1'b0;
    if(pll_ps_v)begin
        temp_v <= 1'b1;
        if(pll_ps_q[15]) begin
            temp1 <= -$signed(pll_ps_i);
        end else begin
            temp1 <= pll_ps_i;
        end
    
        if(pll_ps_i[15]) begin
            temp2 <= -$signed(pll_ps_q);
        end else begin
            temp2 <= pll_ps_q;
        end
    end
end

assign pll_mult = $signed(pll_ps_i)*$signed(pll_ps_q); // different from the initial (so verify)
assign pll_bpsk_simple = pll_mult[26-:16]; // hard coded may want to choose something else
assign pll_qpsk_simple = temp2 - temp1;

always @(posedge i_clk)begin
    pll_to_loopf_v <= temp_v;
    case(do_BPSK)
        1'b0: pll_to_loopf <= pll_qpsk_simple;
        1'b1: pll_to_loopf <= pll_bpsk_simple;
        default: pll_to_loopf <= pll_qpsk_simple;
    endcase
end

// PLL Loop Filter
pll_loopf_0 pll_loopf (
    .x_ap_vld(pll_to_loopf_v),      
    .phase_ap_vld(phase_ap_vld),
    .ap_clk(i_clk),    
    .ap_rst(~i_rst_n),    
    .ap_start(1'b1),  
    .ap_done(),  
    .ap_idle(),       
    .ap_ready(),      
    .x($signed(pll_to_loopf[15:4])),   
    .phase(pll_fb)  
);

always @(posedge i_clk)begin
    if(~i_rst_n)
        pll_fb_inc <= {25{1'b0}};
    else if (phase_ap_vld)
        pll_fb_inc <= -$signed(pll_fb[25:0]); // Is this right? (seems like it should be [25:0])
end

/****************************************
  * Assign Outputs
  * 
  * Split into I (inphase) and Q (quadrature)
****************************************/
assign o_signal_q = pll_ps_q<<3; // MSBs for imaginary part (this is the sin)
assign o_signal_i = pll_ps_i<<3; // LSBs for real part
assign o_vld = pll_ps_v;
assign dbg_err = pll_to_loopf;
endmodule
