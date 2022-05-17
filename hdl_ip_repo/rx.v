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
 * @notes    Currently Using Vivado 2019.2
 *
 ********************************************************************************/

module rx#(
    parameter SRRC_GEN = 1'b0,
    parameter NORM_GEN = 1'b0
)
(
    // Inputs
    input wire i_sysclk,
    input wire i_clk,      
    input wire i_rst,
    
    // Control Signals
    input wire [31:0] i_ctrl,
    input wire [31:0] i_thresh, // for xcorr (ap_uint<22>) ='d50000
    input wire [25:0] i_pow_ref, // for agc ap_ufixed<26,16> = 26'b1100_00_0000_0000
    input wire [31:0] i_store_dly,
    
    // ADC Data Input (RX)
    input wire [15:0] i_fromADC_i,
    input wire [15:0] i_fromADC_q,
    input wire i_fromADC_vld,
 
    // DBG
    output wire [31:0] dbg_corr,
    output wire dbg_max,
    output wire [1:0] dbg_sym,
    output wire dbg_store,
    output wire [15:0] dbg_agc_i,
    output wire dbg_agc_vld,
    output wire [15:0] dbg_tec_i,
    output wire [15:0] dbg_tec_q,
    output wire dbg_tec_vld,
    
    // AXI Interface
    input wire axi_clk,
    input wire axi_rstn,
    input [8:0]s_axi_BUS_A_AWADDR,
    input s_axi_BUS_A_AWVALID,
    output s_axi_BUS_A_AWREADY,
    input [31:0]s_axi_BUS_A_WDATA,
    input [3:0]s_axi_BUS_A_WSTRB,
    input s_axi_BUS_A_WVALID,
    output s_axi_BUS_A_WREADY,
    output [1:0]s_axi_BUS_A_BRESP,
    output s_axi_BUS_A_BVALID,
    input s_axi_BUS_A_BREADY,
    input [8:0]s_axi_BUS_A_ARADDR,
    input s_axi_BUS_A_ARVALID,
    output s_axi_BUS_A_ARREADY,
    output [31:0]s_axi_BUS_A_RDATA,
    output [1:0]s_axi_BUS_A_RRESP,
    output s_axi_BUS_A_RVALID,
    input s_axi_BUS_A_RREADY
); 
    /*******************************************
         * Internal Registers and Wires
    *******************************************/
    wire rst, rstn;
    assign rst = i_rst;
    assign rstn = ~rst;
    
    // control if needed
    wire [2:0] bypass;
    wire atart;
    wire clk;
    
    assign start = i_ctrl[0]; 
    assign bypass = i_ctrl[3:1]; // Bit 0 (agc), 1 (srrc)

    /****************************************
      * Clock Crossing: Slow->Fast (easy case)
    ****************************************/
    wire [15:0] to_agc_i;
    wire [15:0] to_agc_q;
    wire to_agc_v;
    
    reg [15:0] cc_i;
    reg [15:0] cc_q;
    reg cc_v;
    
    reg [15:0] r1_fromADC_i, r2_fromADC_i, r3_fromADC_i;
    reg [15:0] r1_fromADC_q, r2_fromADC_q, r3_fromADC_q;
    reg        r1_fromADC_v, r2_fromADC_v, r3_fromADC_v;
    wire pe_adc_v;
    
    // Register inputs
    always @(posedge i_clk)
    begin
        cc_i <= i_fromADC_i; // meta-stable
        cc_q <= i_fromADC_q;
        cc_v <= i_fromADC_vld;
    end
            
    // cc circuit
    assign clk = i_sysclk;
    always @(posedge clk)
    begin
        r1_fromADC_i <= cc_i; // meta-stable
        r2_fromADC_i <= r1_fromADC_i;
        r3_fromADC_i <= r2_fromADC_i;
        
        r1_fromADC_q <= cc_q; // meta-stable
        r2_fromADC_q <= r1_fromADC_q;
        r3_fromADC_q <= r2_fromADC_q;
        
        r1_fromADC_v <= cc_v; // meta-stable
        r2_fromADC_v <= r1_fromADC_v;
        r3_fromADC_v <= r2_fromADC_v;
    end
    assign pe_adc_v = r3_fromADC_v == 1'b1 && r2_fromADC_v == 1'b0;
    
    // cc outputs
    assign to_agc_i = r3_fromADC_i;
    assign to_agc_q = r3_fromADC_q ;
    assign to_agc_v = pe_adc_v;

    
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
    
    generate 
        if(NORM_GEN)begin
            normalizer_0 spt_norm (
              .ap_local_block(),        // output wire ap_local_block
              .ap_local_deadlock(),  // output wire ap_local_deadlock
              .in_i_ap_vld(to_agc_v),              // input wire in_i_ap_vld
              .in_q_ap_vld(to_agc_v),              // input wire in_q_ap_vld
              .out_i_ap_vld(agc_v),            // output wire out_i_ap_vld
              .out_q_ap_vld(),            // output wire out_q_ap_vld
              .ap_clk(clk),                        // input wire ap_clk
              .ap_rst(rst),                        // input wire ap_rst
              .ap_start(start),                    // input wire ap_start
              .ap_done(),                      // output wire ap_done
              .ap_idle(),                      // output wire ap_idle
              .ap_ready(),                    // output wire ap_ready
              .in_i(to_agc_i),                            // input wire [15 : 0] in_i
              .in_q(to_agc_q),                            // input wire [15 : 0] in_q
              .out_i(agc_i),                          // output wire [15 : 0] out_i
              .out_q(agc_q)                          // output wire [15 : 0] out_q
            );
        end else begin
            agc_0 spt_agc (
              .in_i_ap_vld(to_agc_v),    // input wire in_i_ap_vld
              .in_q_ap_vld(to_agc_v),    // input wire in_q_ap_vld
              .out_i_ap_vld(agc_v),  // output wire out_i_ap_vld
              .out_q_ap_vld(),  // output wire out_q_ap_vld
              .dbg_ap_vld(),  // output wire dbg_V_ap_vld
              .ap_clk(clk),              // input wire ap_clk
              .ap_rst(rst),              // input wire ap_rst
              .ap_start(start),          // input wire ap_start
              .ap_done(),            // output wire ap_done
              .ap_idle(),            // output wire ap_idle
              .ap_ready(),          // output wire ap_ready
              .pref({6'b000000,i_pow_ref}),              // input wire [31 : 0] pref_V
              .in_i(to_agc_i),                  // input wire [15 : 0] in_i
              .in_q(to_agc_q),                  // input wire [15 : 0] in_q
              .out_i(agc_i),                // output wire [15 : 0] out_i
              .out_q(agc_q),                // output wire [15 : 0] out_q
              .dbg()                // output wire [23 : 0] dbg_V
            );

        end
    endgenerate

    assign agc2srrc_i = bypass[0] ? to_agc_i : agc_i;
    assign agc2srrc_q = bypass[0] ? to_agc_q : agc_q;
    assign agc2srrc_v = bypass[0] ? to_agc_v : agc_v;
    
    /****************************************
      * Matched Filter SRRC
    ****************************************/
    
    wire [15:0] to_cfc_i;
    wire [15:0] to_cfc_q;
    wire to_cfc_v;
    
//    generate 
//        if(SRRC_GEN)begin
//            wire rx_srrc_vld;
//            wire [63:0] rx_srrc_out;
            
//            fir_srrc spt_srrc(
//                .aclk(clk),                             
//                .s_axis_data_tvalid(agc2srrc_v),  // could just use one valid
//                .s_axis_data_tready(), 
//                .s_axis_data_tdata({agc2srrc_q,agc2srrc_i}),   
//                .m_axis_data_tvalid(rx_srrc_vld),  
//                .m_axis_data_tdata(rx_srrc_out)   
//            );
            
//            assign to_cfc_i = bypass[1] ? agc2srrc_i : rx_srrc_out[31-5-:16]; // set the right bits
//            assign to_cfc_q = bypass[1] ? agc2srrc_q : rx_srrc_out[63-5-:16];
//            assign to_cfc_v = bypass[1] ? agc2srrc_v : rx_srrc_vld;
//        end else begin
            assign to_cfc_i = agc2srrc_i; // set the right bits
            assign to_cfc_q = agc2srrc_q;
            assign to_cfc_v = agc2srrc_v;
//        end
//    endgenerate
    
    /****************************************
      * Coarse Frequency Correction
    ****************************************/
    
    wire [15:0] to_tec_i;
    wire [15:0] to_tec_q;
    wire to_tec_v;
    
    wire [31:0] to_atan_i, to_atan_q;
    wire to_atan_vld;
    wire [31:0] corr;
    wire sop;
    
    xcorr_0 spt_cfc (
      .thresh_ap_vld(1'b1),        // input wire thresh_ap_vld
      .in_i_ap_vld(to_cfc_v),        
      .in_q_ap_vld(to_cfc_v),     
      .out_i_ap_vld(to_tec_v),          
      .out_q_ap_vld(),           
      .corr_ap_vld(),           
      .to_atan_i_ap_vld(to_atan_vld),  
      .to_atan_q_ap_vld(),    
      .ap_clk(clk),          
      .ap_rst(rst),         
      .ap_start(start),          
      .ap_done(),         
      .ap_idle(),        
      .ap_ready(),  
      .thresh(i_thresh),                   
      .in_i(to_cfc_i),            
      .in_q(to_cfc_q),            
      .out_i(to_tec_i),            
      .out_q(to_tec_q),         
      .corr(corr),           
      .max_vld(sop),                
      .to_atan_i(to_atan_i),         
      .to_atan_q(to_atan_q)          
    );
    
    // Add atan and complex mult for frequency correction (not completely necessary)
    
    /****************************************
      * Timing Error Correction (TEC)
    ****************************************/
    
    wire [15:0] to_pll_i, to_pll_q; // <32,2>
    wire to_pll_v;
    wire to_pll_out_vld;
    wire samp_sel;
    wire samp_sel_v;
    
    tec_0 spt_tec (
      .real_r_ap_vld(to_tec_v),          
      .imag_ap_vld  (to_tec_v),              
      .out_real_ap_vld(to_pll_out_vld),
      .out_imag_ap_vld(),      
      .out_vld_ap_vld(samp_sel_v),        
      .bank_ap_vld(),              
      .ap_clk(clk),                   
      .ap_rst(rst),                    
      .ap_start(start),       
      .ap_done(),                   
      .ap_idle(),                    
      .ap_ready(),                 
      .real_r(to_tec_i),                 
      .imag  (to_tec_q),                
      .out_real(to_pll_i),         
      .out_imag(to_pll_q),                  
      .out_vld (samp_sel),                 
      .bank()                           
    );

    assign to_pll_v = samp_sel && to_pll_out_vld;
    /****************************************
      * PLL
    ****************************************/
    
    wire [15:0] to_hard_decision_i, to_hard_decision_q;
    wire to_hard_decision_v;
    
    pll_0 spt_pll (
      .IN_R_ap_vld(to_pll_v),       
      .IN_I_ap_vld(to_pll_v),          
      .OUT_R_ap_vld(),    
      .OUT_I_ap_vld(),      
      .ap_clk(clk),          
      .ap_rst(rst),            
      .ap_start(start),    
      .ap_done(to_hard_decision_v),         
      .ap_idle(),          
      .ap_ready(),           
      .IN_R(to_pll_i),        
      .IN_I(to_pll_q),                   
      .OUT_R(to_hard_decision_i),                  
      .OUT_I(to_hard_decision_q)                   
    );
    
    //
    // Data Store logic
    //
    wire store;
    reg [31:0] dly_cnt;
    reg data_sop = 1'b0;

    always @(posedge clk)begin
        if(~start)begin
            data_sop <= 1'b0;
            dly_cnt <= 1'b0;
        end else begin
            if(sop)begin// && samp_sel && samp_sel_v)begin
                dly_cnt <= 0;
                data_sop <= 1'b1;  
            end else if (dly_cnt < i_store_dly && to_hard_decision_v) begin
                dly_cnt <= dly_cnt + 1'b1; 
            end 
        end
    end
    
    assign store = (data_sop && (dly_cnt >= i_store_dly)) ? 1'b1: 1'b0;
    
    /****************************************
      * Hard Decision
      * Need to add valid logic for know start
      * of packet
    ****************************************/
    reg [3:0] r_data_word_vld; //0 to 15
    reg data_word_vld;
    reg [31:0] data_word;
    reg [1:0] sym;
    reg err;
    reg data_word_v = 1'b0;
    reg r_data_word_v = 1'b0;
    always @(posedge clk)begin
        data_word_v <= 1'b0;
        if(sop)begin // use sop as reset
            data_word_v <= 1'b0;
            r_data_word_v <= 1'b0;
            r_data_word_vld <= 4'd0;
            sym <= 2'b00;
            err <= 1'b0;
        end else if(to_hard_decision_v && store)begin
            r_data_word_v <= (r_data_word_vld == 4'd15);
            data_word_v <= r_data_word_v;
            r_data_word_vld <= r_data_word_vld + 1;
            data_word <= {data_word[29:0],sym};// shift register to accumulate byte
            if( ($signed(to_hard_decision_i) > 0) && ($signed(to_hard_decision_q) > 0)) begin //(1,1) (I,Q)
                sym <= 2'b00;
                err <= 1'b0;
            end else if ( ($signed(to_hard_decision_i) > 0) && ($signed(to_hard_decision_q) < 0)) begin //(1,-1) 
                sym <= 2'b10;
                err <= 1'b0;
            end else if ( ($signed(to_hard_decision_i) < 0) && ($signed(to_hard_decision_q) > 0)) begin //(-1,1)
                sym <= 2'b01;
                err <= 1'b0;
            end else if ( ($signed(to_hard_decision_i) < 0) && ($signed(to_hard_decision_q) < 0)) begin //(-1,-1)
                sym <= 2'b11;
                err <= 1'b0;
            end else begin // can't decide
                sym <= 2'b00;
                err <= 1'b1;
            end
        end
    end


    /****************************************
      * CC for AXI-LITE @100MHz
    ****************************************/
    wire word_valid_cc;
    wire data_sop_cc;
    
    FlagAck_CrossDomain cc_1_inst(
    .clkA(clk),
    .FlagIn_clkA(data_word_v),
    .Busy_clkA(),
    .clkB(axi_clk),
    .FlagOut_clkB(word_valid_cc)
    );
    
    FlagAck_CrossDomain cc_2_inst(
    .clkA(clk),
    .FlagIn_clkA(data_sop),
    .Busy_clkA(),
    .clkB(axi_clk),
    .FlagOut_clkB(data_sop_cc)
    );
    
    reg [31:0] data_word_cc; 
    always @(posedge clk)begin
        if(data_word_v)
            data_word_cc <= data_word;
    end
    
    /****************************************
      * Data Packet Storage for DMA (or Axilite)
    ****************************************/
    data2ps_0 spt_data_storage(
      .input_r_ap_vld(word_valid_cc),            // input wire input_r_ap_vld
      .s_axi_BUS_A_AWADDR(s_axi_BUS_A_AWADDR),    // input wire [8 : 0] s_axi_BUS_A_AWADDR
      .s_axi_BUS_A_AWVALID(s_axi_BUS_A_AWVALID),  // input wire s_axi_BUS_A_AWVALID
      .s_axi_BUS_A_AWREADY(s_axi_BUS_A_AWREADY),  // output wire s_axi_BUS_A_AWREADY
      .s_axi_BUS_A_WDATA(s_axi_BUS_A_WDATA),      // input wire [31 : 0] s_axi_BUS_A_WDATA
      .s_axi_BUS_A_WSTRB(s_axi_BUS_A_WSTRB),      // input wire [3 : 0] s_axi_BUS_A_WSTRB
      .s_axi_BUS_A_WVALID(s_axi_BUS_A_WVALID),    // input wire s_axi_BUS_A_WVALID
      .s_axi_BUS_A_WREADY(s_axi_BUS_A_WREADY),    // output wire s_axi_BUS_A_WREADY
      .s_axi_BUS_A_BRESP(s_axi_BUS_A_BRESP),      // output wire [1 : 0] s_axi_BUS_A_BRESP
      .s_axi_BUS_A_BVALID(s_axi_BUS_A_BVALID),    // output wire s_axi_BUS_A_BVALID
      .s_axi_BUS_A_BREADY(s_axi_BUS_A_BREADY),    // input wire s_axi_BUS_A_BREADY
      .s_axi_BUS_A_ARADDR(s_axi_BUS_A_ARADDR),    // input wire [8 : 0] s_axi_BUS_A_ARADDR
      .s_axi_BUS_A_ARVALID(s_axi_BUS_A_ARVALID),  // input wire s_axi_BUS_A_ARVALID
      .s_axi_BUS_A_ARREADY(s_axi_BUS_A_ARREADY),  // output wire s_axi_BUS_A_ARREADY
      .s_axi_BUS_A_RDATA(s_axi_BUS_A_RDATA),      // output wire [31 : 0] s_axi_BUS_A_RDATA
      .s_axi_BUS_A_RRESP(s_axi_BUS_A_RRESP),      // output wire [1 : 0] s_axi_BUS_A_RRESP
      .s_axi_BUS_A_RVALID(s_axi_BUS_A_RVALID),    // output wire s_axi_BUS_A_RVALID
      .s_axi_BUS_A_RREADY(s_axi_BUS_A_RREADY),    // input wire s_axi_BUS_A_RREADY
      .ap_clk(axi_clk),                            // input wire ap_clk
      .ap_rst_n(axi_rstn),                        // input wire ap_rst_n
      .ap_start(store),                        // input wire ap_start
      .ap_done(),                          // output wire ap_done
      .ap_idle(),                          // output wire ap_idle
      .ap_ready(),                        // output wire ap_ready
      .input_r(data_word_cc),                          // input wire [31 : 0] input_r
      .addr_rst(~store)                        // input wire addr_rst
    );

/****************************************
  * Assign Outputs
****************************************/
    assign dbg_corr = corr;
    assign dbg_max = sop;
    assign dbg_sym = sym;
    assign dbg_store = store;
    assign dbg_agc = to_cfc_i;
    assign dbg_agc_vld = to_cfc_v;
    assign dbg_tec_i   = to_tec_i;
    assign dbg_tec_q   = to_tec_q;
    assign dbg_tec_vld = to_tec_v;
    
endmodule

module FlagAck_CrossDomain(
    input clkA,
    input FlagIn_clkA,
    output Busy_clkA,
    input clkB,
    output FlagOut_clkB
);

    reg FlagToggle_clkA = 1'd0;
    always @(posedge clkA) FlagToggle_clkA <= FlagToggle_clkA ^ (FlagIn_clkA & ~Busy_clkA);
    
    reg [2:0] SyncA_clkB = 3'd0;
    always @(posedge clkB) SyncA_clkB <= {SyncA_clkB[1:0], FlagToggle_clkA};
    
    reg [1:0] SyncB_clkA = 2'd0;
    always @(posedge clkA) SyncB_clkA <= {SyncB_clkA[0], SyncA_clkB[2]};
    
    assign FlagOut_clkB = (SyncA_clkB[2] ^ SyncA_clkB[1]);
    assign Busy_clkA = FlagToggle_clkA ^ SyncB_clkA[1];
endmodule