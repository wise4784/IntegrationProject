//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
//Date        : Mon Jun 24 17:35:06 2019
//Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
//Command     : generate_target system_wrapper.bd
//Design      : system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module system_wrapper
   (clk,
    data,
    rst);
  input clk;
  output [7:0]data;
  input rst;

  wire clk;
  wire [7:0]data;
  wire rst;

  system system_i
       (.clk(clk),
        .data(data),
        .rst(rst));
endmodule
