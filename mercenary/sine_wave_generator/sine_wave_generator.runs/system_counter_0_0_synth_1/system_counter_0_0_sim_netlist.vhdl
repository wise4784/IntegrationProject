-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
-- Date        : Mon Jun 24 17:27:40 2019
-- Host        : wrg-900X5N running 64-bit Ubuntu 16.04.6 LTS
-- Command     : write_vhdl -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ system_counter_0_0_sim_netlist.vhdl
-- Design      : system_counter_0_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter is
  port (
    data : out STD_LOGIC_VECTOR ( 8 downto 0 );
    tc : out STD_LOGIC;
    rst : in STD_LOGIC;
    en : in STD_LOGIC;
    clk : in STD_LOGIC
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter is
  signal \^data\ : STD_LOGIC_VECTOR ( 8 downto 0 );
  signal \data[5]_i_2_n_0\ : STD_LOGIC;
  signal \data[8]_i_2_n_0\ : STD_LOGIC;
  signal \data[8]_i_3_n_0\ : STD_LOGIC;
  signal p_0_in : STD_LOGIC_VECTOR ( 8 downto 0 );
  signal tc_INST_0_i_1_n_0 : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \data[0]_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \data[2]_i_1\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \data[3]_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \data[5]_i_2\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \data[8]_i_3\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of tc_INST_0 : label is "soft_lutpair2";
begin
  data(8 downto 0) <= \^data\(8 downto 0);
\data[0]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0E0F"
    )
        port map (
      I0 => \^data\(2),
      I1 => \^data\(1),
      I2 => \^data\(0),
      I3 => tc_INST_0_i_1_n_0,
      O => p_0_in(0)
    );
\data[1]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => \^data\(0),
      I1 => \^data\(1),
      O => p_0_in(1)
    );
\data[2]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"78"
    )
        port map (
      I0 => \^data\(0),
      I1 => \^data\(1),
      I2 => \^data\(2),
      O => p_0_in(2)
    );
\data[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"3FFDC000"
    )
        port map (
      I0 => tc_INST_0_i_1_n_0,
      I1 => \^data\(0),
      I2 => \^data\(1),
      I3 => \^data\(2),
      I4 => \^data\(3),
      O => p_0_in(3)
    );
\data[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"3FFDFFFDC0000000"
    )
        port map (
      I0 => tc_INST_0_i_1_n_0,
      I1 => \^data\(0),
      I2 => \^data\(1),
      I3 => \^data\(2),
      I4 => \^data\(3),
      I5 => \^data\(4),
      O => p_0_in(4)
    );
\data[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"55545555AAA8AAAA"
    )
        port map (
      I0 => \data[5]_i_2_n_0\,
      I1 => \^data\(2),
      I2 => \^data\(1),
      I3 => \^data\(0),
      I4 => tc_INST_0_i_1_n_0,
      I5 => \^data\(5),
      O => p_0_in(5)
    );
\data[5]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"80000000"
    )
        port map (
      I0 => \^data\(4),
      I1 => \^data\(2),
      I2 => \^data\(0),
      I3 => \^data\(1),
      I4 => \^data\(3),
      O => \data[5]_i_2_n_0\
    );
\data[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAA8AAAA55545555"
    )
        port map (
      I0 => \data[8]_i_2_n_0\,
      I1 => \^data\(2),
      I2 => \^data\(1),
      I3 => \^data\(0),
      I4 => tc_INST_0_i_1_n_0,
      I5 => \^data\(6),
      O => p_0_in(6)
    );
\data[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0DDD0222"
    )
        port map (
      I0 => \^data\(6),
      I1 => \data[8]_i_2_n_0\,
      I2 => \data[8]_i_3_n_0\,
      I3 => tc_INST_0_i_1_n_0,
      I4 => \^data\(7),
      O => p_0_in(7)
    );
\data[8]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00BFBFBF00404040"
    )
        port map (
      I0 => \data[8]_i_2_n_0\,
      I1 => \^data\(6),
      I2 => \^data\(7),
      I3 => \data[8]_i_3_n_0\,
      I4 => tc_INST_0_i_1_n_0,
      I5 => \^data\(8),
      O => p_0_in(8)
    );
\data[8]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"7FFFFFFFFFFFFFFF"
    )
        port map (
      I0 => \^data\(5),
      I1 => \^data\(3),
      I2 => \^data\(1),
      I3 => \^data\(0),
      I4 => \^data\(2),
      I5 => \^data\(4),
      O => \data[8]_i_2_n_0\
    );
\data[8]_i_3\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"01"
    )
        port map (
      I0 => \^data\(2),
      I1 => \^data\(1),
      I2 => \^data\(0),
      O => \data[8]_i_3_n_0\
    );
\data_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(0),
      Q => \^data\(0),
      R => rst
    );
\data_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(1),
      Q => \^data\(1),
      R => rst
    );
\data_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(2),
      Q => \^data\(2),
      R => rst
    );
\data_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(3),
      Q => \^data\(3),
      R => rst
    );
\data_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(4),
      Q => \^data\(4),
      R => rst
    );
\data_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(5),
      Q => \^data\(5),
      R => rst
    );
\data_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(6),
      Q => \^data\(6),
      R => rst
    );
\data_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(7),
      Q => \^data\(7),
      R => rst
    );
\data_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => en,
      D => p_0_in(8),
      Q => \^data\(8),
      R => rst
    );
tc_INST_0: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0002"
    )
        port map (
      I0 => tc_INST_0_i_1_n_0,
      I1 => \^data\(0),
      I2 => \^data\(1),
      I3 => \^data\(2),
      O => tc
    );
tc_INST_0_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"2000000000000000"
    )
        port map (
      I0 => \^data\(3),
      I1 => \^data\(4),
      I2 => \^data\(5),
      I3 => \^data\(6),
      I4 => \^data\(8),
      I5 => \^data\(7),
      O => tc_INST_0_i_1_n_0
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    clk : in STD_LOGIC;
    rst : in STD_LOGIC;
    en : in STD_LOGIC;
    data : out STD_LOGIC_VECTOR ( 8 downto 0 );
    tc : out STD_LOGIC
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "system_counter_0_0,counter,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute IP_DEFINITION_SOURCE : string;
  attribute IP_DEFINITION_SOURCE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "module_ref";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "counter,Vivado 2018.2";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of clk : signal is "xilinx.com:signal:clock:1.0 clk CLK";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of clk : signal is "XIL_INTERFACENAME clk, ASSOCIATED_RESET rst, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN system_clk_0";
  attribute X_INTERFACE_INFO of rst : signal is "xilinx.com:signal:reset:1.0 rst RST";
  attribute X_INTERFACE_PARAMETER of rst : signal is "XIL_INTERFACENAME rst, POLARITY ACTIVE_LOW";
begin
inst: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter
     port map (
      clk => clk,
      data(8 downto 0) => data(8 downto 0),
      en => en,
      rst => rst,
      tc => tc
    );
end STRUCTURE;
