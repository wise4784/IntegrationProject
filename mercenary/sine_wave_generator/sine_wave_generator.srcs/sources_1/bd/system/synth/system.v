//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
//Date        : Mon Jun 24 17:35:06 2019
//Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
//Command     : generate_target system.bd
//Design      : system
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "system,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=system,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=4,numReposBlks=4,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=3,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "system.hwdef" *) 
module system
   (clk,
    data,
    rst);
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.CLK, ASSOCIATED_RESET rst, CLK_DOMAIN system_clk_0, FREQ_HZ 100000000, PHASE 0.000" *) input clk;
  output [7:0]data;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.RST, POLARITY ACTIVE_LOW" *) input rst;

  wire clk_0_1;
  wire counter_0_tc;
  wire [7:0]counter_1_data;
  wire [7:0]lut_0_data;
  wire rst_0_1;
  wire [0:0]xlconstant_0_dout;

  assign clk_0_1 = clk;
  assign data[7:0] = lut_0_data;
  assign rst_0_1 = rst;
  system_counter_0_0 counter_0
       (.clk(clk_0_1),
        .en(xlconstant_0_dout),
        .rst(rst_0_1),
        .tc(counter_0_tc));
  system_counter_1_0 counter_1
       (.clk(clk_0_1),
        .data(counter_1_data),
        .en(counter_0_tc),
        .rst(rst_0_1));
  system_lut_0_0 lut_0
       (.addr(counter_1_data),
        .clk(clk_0_1),
        .data(lut_0_data));
  system_xlconstant_0_0 xlconstant_0
       (.dout(xlconstant_0_dout));
endmodule
