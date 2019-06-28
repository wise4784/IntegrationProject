-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
-- Date        : Mon Jun 24 17:27:42 2019
-- Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
-- Command     : write_vhdl -force -mode funcsim
--               /home/wrg/tmp/sine_wave_generator/sine_wave_generator.srcs/sources_1/bd/system/ip/system_lut_0_0/system_lut_0_0_sim_netlist.vhdl
-- Design      : system_lut_0_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity system_lut_0_0 is
  port (
    clk : in STD_LOGIC;
    addr : in STD_LOGIC_VECTOR ( 7 downto 0 );
    data : out STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of system_lut_0_0 : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of system_lut_0_0 : entity is "system_lut_0_0,lut,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of system_lut_0_0 : entity is "yes";
  attribute IP_DEFINITION_SOURCE : string;
  attribute IP_DEFINITION_SOURCE of system_lut_0_0 : entity is "module_ref";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of system_lut_0_0 : entity is "lut,Vivado 2018.2";
end system_lut_0_0;

architecture STRUCTURE of system_lut_0_0 is
  signal \<const0>\ : STD_LOGIC;
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of clk : signal is "xilinx.com:signal:clock:1.0 clk CLK";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of clk : signal is "XIL_INTERFACENAME clk, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN system_clk_0";
begin
  data(7) <= \<const0>\;
  data(6) <= \<const0>\;
  data(5) <= \<const0>\;
  data(4) <= \<const0>\;
  data(3) <= \<const0>\;
  data(2) <= \<const0>\;
  data(1) <= \<const0>\;
  data(0) <= \<const0>\;
GND: unisim.vcomponents.GND
     port map (
      G => \<const0>\
    );
end STRUCTURE;
