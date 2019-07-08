// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
// Date        : Mon Jun 24 17:27:40 2019
// Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ system_counter_0_0_sim_netlist.v
// Design      : system_counter_0_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter
   (data,
    tc,
    rst,
    en,
    clk);
  output [8:0]data;
  output tc;
  input rst;
  input en;
  input clk;

  wire clk;
  wire [8:0]data;
  wire \data[5]_i_2_n_0 ;
  wire \data[8]_i_2_n_0 ;
  wire \data[8]_i_3_n_0 ;
  wire en;
  wire [8:0]p_0_in;
  wire rst;
  wire tc;
  wire tc_INST_0_i_1_n_0;

  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT4 #(
    .INIT(16'h0E0F)) 
    \data[0]_i_1 
       (.I0(data[2]),
        .I1(data[1]),
        .I2(data[0]),
        .I3(tc_INST_0_i_1_n_0),
        .O(p_0_in[0]));
  LUT2 #(
    .INIT(4'h6)) 
    \data[1]_i_1 
       (.I0(data[0]),
        .I1(data[1]),
        .O(p_0_in[1]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \data[2]_i_1 
       (.I0(data[0]),
        .I1(data[1]),
        .I2(data[2]),
        .O(p_0_in[2]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h3FFDC000)) 
    \data[3]_i_1 
       (.I0(tc_INST_0_i_1_n_0),
        .I1(data[0]),
        .I2(data[1]),
        .I3(data[2]),
        .I4(data[3]),
        .O(p_0_in[3]));
  LUT6 #(
    .INIT(64'h3FFDFFFDC0000000)) 
    \data[4]_i_1 
       (.I0(tc_INST_0_i_1_n_0),
        .I1(data[0]),
        .I2(data[1]),
        .I3(data[2]),
        .I4(data[3]),
        .I5(data[4]),
        .O(p_0_in[4]));
  LUT6 #(
    .INIT(64'h55545555AAA8AAAA)) 
    \data[5]_i_1 
       (.I0(\data[5]_i_2_n_0 ),
        .I1(data[2]),
        .I2(data[1]),
        .I3(data[0]),
        .I4(tc_INST_0_i_1_n_0),
        .I5(data[5]),
        .O(p_0_in[5]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h80000000)) 
    \data[5]_i_2 
       (.I0(data[4]),
        .I1(data[2]),
        .I2(data[0]),
        .I3(data[1]),
        .I4(data[3]),
        .O(\data[5]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hAAA8AAAA55545555)) 
    \data[6]_i_1 
       (.I0(\data[8]_i_2_n_0 ),
        .I1(data[2]),
        .I2(data[1]),
        .I3(data[0]),
        .I4(tc_INST_0_i_1_n_0),
        .I5(data[6]),
        .O(p_0_in[6]));
  LUT5 #(
    .INIT(32'h0DDD0222)) 
    \data[7]_i_1 
       (.I0(data[6]),
        .I1(\data[8]_i_2_n_0 ),
        .I2(\data[8]_i_3_n_0 ),
        .I3(tc_INST_0_i_1_n_0),
        .I4(data[7]),
        .O(p_0_in[7]));
  LUT6 #(
    .INIT(64'h00BFBFBF00404040)) 
    \data[8]_i_1 
       (.I0(\data[8]_i_2_n_0 ),
        .I1(data[6]),
        .I2(data[7]),
        .I3(\data[8]_i_3_n_0 ),
        .I4(tc_INST_0_i_1_n_0),
        .I5(data[8]),
        .O(p_0_in[8]));
  LUT6 #(
    .INIT(64'h7FFFFFFFFFFFFFFF)) 
    \data[8]_i_2 
       (.I0(data[5]),
        .I1(data[3]),
        .I2(data[1]),
        .I3(data[0]),
        .I4(data[2]),
        .I5(data[4]),
        .O(\data[8]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'h01)) 
    \data[8]_i_3 
       (.I0(data[2]),
        .I1(data[1]),
        .I2(data[0]),
        .O(\data[8]_i_3_n_0 ));
  FDRE \data_reg[0] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[0]),
        .Q(data[0]),
        .R(rst));
  FDRE \data_reg[1] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[1]),
        .Q(data[1]),
        .R(rst));
  FDRE \data_reg[2] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[2]),
        .Q(data[2]),
        .R(rst));
  FDRE \data_reg[3] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[3]),
        .Q(data[3]),
        .R(rst));
  FDRE \data_reg[4] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[4]),
        .Q(data[4]),
        .R(rst));
  FDRE \data_reg[5] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[5]),
        .Q(data[5]),
        .R(rst));
  FDRE \data_reg[6] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[6]),
        .Q(data[6]),
        .R(rst));
  FDRE \data_reg[7] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[7]),
        .Q(data[7]),
        .R(rst));
  FDRE \data_reg[8] 
       (.C(clk),
        .CE(en),
        .D(p_0_in[8]),
        .Q(data[8]),
        .R(rst));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h0002)) 
    tc_INST_0
       (.I0(tc_INST_0_i_1_n_0),
        .I1(data[0]),
        .I2(data[1]),
        .I3(data[2]),
        .O(tc));
  LUT6 #(
    .INIT(64'h2000000000000000)) 
    tc_INST_0_i_1
       (.I0(data[3]),
        .I1(data[4]),
        .I2(data[5]),
        .I3(data[6]),
        .I4(data[8]),
        .I5(data[7]),
        .O(tc_INST_0_i_1_n_0));
endmodule

(* CHECK_LICENSE_TYPE = "system_counter_0_0,counter,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* IP_DEFINITION_SOURCE = "module_ref" *) 
(* X_CORE_INFO = "counter,Vivado 2018.2" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (clk,
    rst,
    en,
    data,
    tc);
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME clk, ASSOCIATED_RESET rst, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN system_clk_0" *) input clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME rst, POLARITY ACTIVE_LOW" *) input rst;
  input en;
  output [8:0]data;
  output tc;

  wire clk;
  wire [8:0]data;
  wire en;
  wire rst;
  wire tc;

  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter inst
       (.clk(clk),
        .data(data),
        .en(en),
        .rst(rst),
        .tc(tc));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule
`endif
