-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
-- Date        : Mon Jun 24 17:27:42 2019
-- Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
-- Command     : write_vhdl -force -mode synth_stub
--               /home/wrg/tmp/sine_wave_generator/sine_wave_generator.srcs/sources_1/bd/system/ip/system_lut_0_0/system_lut_0_0_stub.vhdl
-- Design      : system_lut_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity system_lut_0_0 is
  Port ( 
    clk : in STD_LOGIC;
    addr : in STD_LOGIC_VECTOR ( 7 downto 0 );
    data : out STD_LOGIC_VECTOR ( 7 downto 0 )
  );

end system_lut_0_0;

architecture stub of system_lut_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk,addr[7:0],data[7:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "lut,Vivado 2018.2";
begin
end;
