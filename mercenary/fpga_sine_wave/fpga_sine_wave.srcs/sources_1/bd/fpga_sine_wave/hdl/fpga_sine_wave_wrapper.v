//Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2017.4 (lin64) Build 2086221 Fri Dec 15 20:54:30 MST 2017
//Date        : Thu Jun 27 19:54:58 2019
//Host        : sdr-Samsung-DeskTop-System running 64-bit Ubuntu 16.04.3 LTS
//Command     : generate_target fpga_sine_wave_wrapper.bd
//Design      : fpga_sine_wave_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module fpga_sine_wave_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    je_pin10_io,
    je_pin1_io,
    je_pin2_io,
    je_pin3_io,
    je_pin4_io,
    je_pin7_io,
    je_pin8_io,
    je_pin9_io);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  inout je_pin10_io;
  inout je_pin1_io;
  inout je_pin2_io;
  inout je_pin3_io;
  inout je_pin4_io;
  inout je_pin7_io;
  inout je_pin8_io;
  inout je_pin9_io;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire je_pin10_i;
  wire je_pin10_io;
  wire je_pin10_o;
  wire je_pin10_t;
  wire je_pin1_i;
  wire je_pin1_io;
  wire je_pin1_o;
  wire je_pin1_t;
  wire je_pin2_i;
  wire je_pin2_io;
  wire je_pin2_o;
  wire je_pin2_t;
  wire je_pin3_i;
  wire je_pin3_io;
  wire je_pin3_o;
  wire je_pin3_t;
  wire je_pin4_i;
  wire je_pin4_io;
  wire je_pin4_o;
  wire je_pin4_t;
  wire je_pin7_i;
  wire je_pin7_io;
  wire je_pin7_o;
  wire je_pin7_t;
  wire je_pin8_i;
  wire je_pin8_io;
  wire je_pin8_o;
  wire je_pin8_t;
  wire je_pin9_i;
  wire je_pin9_io;
  wire je_pin9_o;
  wire je_pin9_t;

  fpga_sine_wave fpga_sine_wave_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .je_pin10_i(je_pin10_i),
        .je_pin10_o(je_pin10_o),
        .je_pin10_t(je_pin10_t),
        .je_pin1_i(je_pin1_i),
        .je_pin1_o(je_pin1_o),
        .je_pin1_t(je_pin1_t),
        .je_pin2_i(je_pin2_i),
        .je_pin2_o(je_pin2_o),
        .je_pin2_t(je_pin2_t),
        .je_pin3_i(je_pin3_i),
        .je_pin3_o(je_pin3_o),
        .je_pin3_t(je_pin3_t),
        .je_pin4_i(je_pin4_i),
        .je_pin4_o(je_pin4_o),
        .je_pin4_t(je_pin4_t),
        .je_pin7_i(je_pin7_i),
        .je_pin7_o(je_pin7_o),
        .je_pin7_t(je_pin7_t),
        .je_pin8_i(je_pin8_i),
        .je_pin8_o(je_pin8_o),
        .je_pin8_t(je_pin8_t),
        .je_pin9_i(je_pin9_i),
        .je_pin9_o(je_pin9_o),
        .je_pin9_t(je_pin9_t));
  IOBUF je_pin10_iobuf
       (.I(je_pin10_o),
        .IO(je_pin10_io),
        .O(je_pin10_i),
        .T(je_pin10_t));
  IOBUF je_pin1_iobuf
       (.I(je_pin1_o),
        .IO(je_pin1_io),
        .O(je_pin1_i),
        .T(je_pin1_t));
  IOBUF je_pin2_iobuf
       (.I(je_pin2_o),
        .IO(je_pin2_io),
        .O(je_pin2_i),
        .T(je_pin2_t));
  IOBUF je_pin3_iobuf
       (.I(je_pin3_o),
        .IO(je_pin3_io),
        .O(je_pin3_i),
        .T(je_pin3_t));
  IOBUF je_pin4_iobuf
       (.I(je_pin4_o),
        .IO(je_pin4_io),
        .O(je_pin4_i),
        .T(je_pin4_t));
  IOBUF je_pin7_iobuf
       (.I(je_pin7_o),
        .IO(je_pin7_io),
        .O(je_pin7_i),
        .T(je_pin7_t));
  IOBUF je_pin8_iobuf
       (.I(je_pin8_o),
        .IO(je_pin8_io),
        .O(je_pin8_i),
        .T(je_pin8_t));
  IOBUF je_pin9_iobuf
       (.I(je_pin9_o),
        .IO(je_pin9_io),
        .O(je_pin9_i),
        .T(je_pin9_t));
endmodule
