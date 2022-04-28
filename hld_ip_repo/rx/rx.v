`timescale 1ns / 1ps
/********************************************************************************
 * @file     rx.v
 * @author   V2X Motorcycle Team
 * @date     
 * @desc     Top Level of RX Signal Processing which will integrate
 *           with fmcomms HDL
 *
 * @param    
 *
 * @notes    
 *
 ********************************************************************************/

module rx#(
    parameter CC_GEN = 0,
    parameter SRRC_GEN = 0
)
(
    // Inputs
    input wire i_sysclk,
    input wire i_clk,      
    input wire i_rst,
    // Control Signals
    input wire [31:0] i_ctrl,
    // ADC Data Input (RX)
    input wire [15:0] i_fromADC_i,
    input wire [15:0] i_fromADC_q,
    input wire i_fromADC_vld,
    // Receiver Data
    output wire [15:0] o_datarx_i,
    output wire [15:0] o_datarx_q,
    output wire o_datarx_vld
); 
    /*******************************************
         * Internal Registers and Wires
    *******************************************/
    reg [1:0] bypass = 2'b00; // Bit 0 (agc), 1 (srrc)
    wire rst, rstn;
    assign rst = i_rst;
    assign rstn = ~rst;
    
    // control if needed
    wire [1:0] mode = i_ctrl[1:0]; // Test Case 1 (mode = 0)
    
    wire clk;

    /****************************************
      * Clock Crossing: Slow->Fast (easy case)
    ****************************************/
    wire [15:0] to_agc_i, to_agc_q;
    wire to_agc_v;
    
    generate
        if(CC_GEN)begin
            reg [15:0] r1_fromADC_i, r2_fromADC_i, r3_fromADC_i;
            reg [15:0] r1_fromADC_q, r2_fromADC_q, r3_fromADC_q;
            reg        r1_fromADC_v, r2_fromADC_v, r3_fromADC_v;
            wire pe_adc_v;
            
            // cc circuit
            always @(posedge i_sysclk)
            begin
                r1_fromADC_i <= i_fromADC_i; // meta-stable
                r2_fromADC_i <= r1_fromADC_i;
                r3_fromADC_i <= r2_fromADC_i;
                
                r1_fromADC_q <= i_fromADC_q; // meta-stable
                r2_fromADC_q <= r1_fromADC_q;
                r3_fromADC_q <= r2_fromADC_q;
                
                r1_fromADC_v <= i_fromADC_vld; // meta-stable
                r2_fromADC_v <= r1_fromADC_v;
                r3_fromADC_v <= r2_fromADC_v;
            end
            assign pe_adc_v = r3_fromADC_v == 1'b0 && r2_fromADC_v == 1'b1;
            
            // cc outputs
            assign clk = i_sysclk;
            assign to_agc_i = r3_fromADC_i;
            assign to_agc_q = r3_fromADC_q ;
            assign to_agc_v = pe_adc_v;
        end else begin
            // outputs if not using cc circuit
            assign clk = i_clk;
            assign to_agc_i = i_fromADC_i;
            assign to_agc_q = i_fromADC_q;
            assign to_agc_v = i_fromADC_vld;
        end
    endgenerate
    
    /****************************************
      * Automatic Gain Control 
      *   Log Approximation Approach
    ****************************************/
    wire [15:0] agc_i;
    wire [15:0] agc_q;
    wire agc_v;
    wire [23:0] dbg_gain;
    
    wire [15:0] agc2srrc_i;
    wire [15:0] agc2srrc_q;
    wire agc2srrc_v;
    
    agc_0 spt_agc(
      .in_i_ap_vld(to_agc_v),    // input wire in_i_ap_vld
      .in_q_ap_vld(to_agc_v),    // input wire in_q_ap_vld
      .out_i_ap_vld(agc_v),  // output wire out_i_ap_vld
      .out_q_ap_vld(),  // output wire out_q_ap_vld
      .dbg_V_ap_vld(),  // output wire dbg_V_ap_vld
      .ap_clk(clk),              // input wire ap_clk
      .ap_rst(rst),              // input wire ap_rst
      .ap_start(1'b1),          // input wire ap_start
      .ap_done(),            // output wire ap_done
      .ap_idle(),            // output wire ap_idle
      .ap_ready(),          // output wire ap_ready
      .pref_V({16'h000C,16'h0000}),              // input wire [31 : 0] pref_V
      .in_i(to_agc_i),                  // input wire [15 : 0] in_i
      .in_q(to_agc_q),                  // input wire [15 : 0] in_q
      .out_i(agc_i),                // output wire [15 : 0] out_i
      .out_q(agc_q),                // output wire [15 : 0] out_q
      .dbg_V(dbg_gain)                // output wire [23 : 0] dbg_V
    );

    assign agc2srrc_i = bypass[0] ? i_fromADC_i : agc_i;
    assign agc2srrc_q = bypass[0] ? i_fromADC_q : agc_q;
    assign agc2srrc_v = bypass[0] ? i_fromADC_vld : agc_v;
    
    /****************************************
      * Matched Filter SRRC
    ****************************************/
    wire [15:0] to_cfc_i;
    wire [15:0] to_cfc_q;
    wire to_cfc_v;
    
    generate 
        if(SRRC_GEN)begin
            wire rx_srrc_vld;
            wire [63:0] rx_srrc_out;
            
            fir_srrc spt_srrc(
                .aclk(i_clk),                             
                .s_axis_data_tvalid(agc2rx_vld),  // could just use one valid
                .s_axis_data_tready(), 
                .s_axis_data_tdata({agc2rx_q,agc2rx_i}),   
                .m_axis_data_tvalid(rx_srrc_vld),  
                .m_axis_data_tdata(rx_srrc_out)   
            );
            
            assign to_cfc_i = bypass[1] ? agc2srrc_i : rx_srrc_out[31-5-:16]; // set the right bits
            assign to_cfc_q = bypass[1] ? agc2srrc_q : rx_srrc_out[63-5-:16];
            assign to_cfc_v = bypass[1] ? agc2srrc_v : rx_srrc_vld;
        end else begin
            assign to_cfc_i = agc2srrc_i; // set the right bits
            assign to_cfc_q = agc2srrc_q;
            assign to_cfc_v = agc2srrc_v;
        end
    endgenerate
    
    /****************************************
      * Coarse Frequency Correction
    ****************************************/
    wire [15:0] to_tec_i, to_tec_q;
    wire to_tec_v;
    
    xcorr_0 spt_cfc (
      .in_i_ap_vld(to_cfc_v),              // input wire in_i_ap_vld
      .in_q_ap_vld(to_cfc_v),              // input wire in_q_ap_vld
      .out_i_ap_vld(to_tec_v),            // output wire out_i_ap_vld
      .out_q_ap_vld(),            // output wire out_q_ap_vld
      .corr_ap_vld(),              // output wire corr_ap_vld
      .max_vld_ap_vld(),        // output wire max_vld_ap_vld
      .to_atan_i_ap_vld(),    // output wire to_atan_i_ap_vld
      .to_atan_q_ap_vld(),    // output wire to_atan_q_ap_vld
      .ap_clk(clk),                        // input wire ap_clk
      .ap_rst(rst),                        // input wire ap_rst
      .ap_start(),                    // input wire ap_start
      .ap_done(),                      // output wire ap_done
      .ap_idle(),                      // output wire ap_idle
      .ap_ready(),                    // output wire ap_ready
      .in_i(to_cfc_i),                            // input wire [15 : 0] in_i
      .in_q(to_cfc_q),                            // input wire [15 : 0] in_q
      .out_i(to_tec_i),                          // output wire [15 : 0] out_i
      .out_q(to_tec_q),                          // output wire [15 : 0] out_q
      .corr(),                            // output wire [21 : 0] corr
      .max_vld(),                      // output wire max_vld
      .to_atan_i(),                  // output wire [31 : 0] to_atan_i
      .to_atan_q()                  // output wire [31 : 0] to_atan_q
    );
    
    /****************************************
      * Timing Error Correction (TEC)
    ****************************************/

    /****************************************
      * PLL
    ****************************************/

    /****************************************
      * Hard Decision
    ****************************************/

    /****************************************
      * Data Packet Storage for DMA
    ****************************************/
    
/****************************************
  * Assign Outputs
****************************************/

assign o_datarx_vld = 1'b0;
assign o_datarx_i = 'd0;
assign o_datarx_q = 'd0;

endmodule