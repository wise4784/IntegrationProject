-------------------------------------------------------------------------------
-- proc_sys_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity proc_sys_stub is
  port (
    fpga_0_RS232_RX_pin : in std_logic;
    fpga_0_RS232_TX_pin : out std_logic;
    fpga_0_clk_1_sys_clk_pin : in std_logic;
    fpga_0_rst_1_sys_rst_pin : in std_logic;
    xps_epc_0_PRH_CS_n_pin : out std_logic;
    xps_epc_0_PRH_Addr_pin : out std_logic_vector(0 to 31);
    xps_epc_0_PRH_RNW_pin : out std_logic;
    xps_epc_0_PRH_Rdy_pin : in std_logic;
    xps_epc_0_PRH_Clk_pin : in std_logic;
    xps_epc_0_PRH_Rst_pin : in std_logic;
    xps_epc_0_PRH_Data_I_pin : in std_logic_vector(0 to 31);
    xps_epc_0_PRH_Data_O_pin : out std_logic_vector(0 to 31);
    xps_mch_emc_0_Mem_DQ : inout std_logic_vector(0 to 15);
    xps_mch_emc_0_Mem_A_pin : out std_logic_vector(0 to 31);
    xps_mch_emc_0_Mem_CEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_0_Mem_WEN_pin : out std_logic;
    xps_mch_emc_0_Mem_OEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_0_RdClk_pin : in std_logic
  );
end proc_sys_stub;

architecture STRUCTURE of proc_sys_stub is

  component proc_sys is
    port (
      fpga_0_RS232_RX_pin : in std_logic;
      fpga_0_RS232_TX_pin : out std_logic;
      fpga_0_clk_1_sys_clk_pin : in std_logic;
      fpga_0_rst_1_sys_rst_pin : in std_logic;
      xps_epc_0_PRH_CS_n_pin : out std_logic;
      xps_epc_0_PRH_Addr_pin : out std_logic_vector(0 to 31);
      xps_epc_0_PRH_RNW_pin : out std_logic;
      xps_epc_0_PRH_Rdy_pin : in std_logic;
      xps_epc_0_PRH_Clk_pin : in std_logic;
      xps_epc_0_PRH_Rst_pin : in std_logic;
      xps_epc_0_PRH_Data_I_pin : in std_logic_vector(0 to 31);
      xps_epc_0_PRH_Data_O_pin : out std_logic_vector(0 to 31);
      xps_mch_emc_0_Mem_DQ : inout std_logic_vector(0 to 15);
      xps_mch_emc_0_Mem_A_pin : out std_logic_vector(0 to 31);
      xps_mch_emc_0_Mem_CEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_0_Mem_WEN_pin : out std_logic;
      xps_mch_emc_0_Mem_OEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_0_RdClk_pin : in std_logic
    );
  end component;

 attribute BOX_TYPE : STRING;
 attribute BOX_TYPE of proc_sys : component is "user_black_box";

begin

  proc_sys_i : proc_sys
    port map (
      fpga_0_RS232_RX_pin => fpga_0_RS232_RX_pin,
      fpga_0_RS232_TX_pin => fpga_0_RS232_TX_pin,
      fpga_0_clk_1_sys_clk_pin => fpga_0_clk_1_sys_clk_pin,
      fpga_0_rst_1_sys_rst_pin => fpga_0_rst_1_sys_rst_pin,
      xps_epc_0_PRH_CS_n_pin => xps_epc_0_PRH_CS_n_pin,
      xps_epc_0_PRH_Addr_pin => xps_epc_0_PRH_Addr_pin,
      xps_epc_0_PRH_RNW_pin => xps_epc_0_PRH_RNW_pin,
      xps_epc_0_PRH_Rdy_pin => xps_epc_0_PRH_Rdy_pin,
      xps_epc_0_PRH_Clk_pin => xps_epc_0_PRH_Clk_pin,
      xps_epc_0_PRH_Rst_pin => xps_epc_0_PRH_Rst_pin,
      xps_epc_0_PRH_Data_I_pin => xps_epc_0_PRH_Data_I_pin,
      xps_epc_0_PRH_Data_O_pin => xps_epc_0_PRH_Data_O_pin,
      xps_mch_emc_0_Mem_DQ => xps_mch_emc_0_Mem_DQ,
      xps_mch_emc_0_Mem_A_pin => xps_mch_emc_0_Mem_A_pin,
      xps_mch_emc_0_Mem_CEN_pin => xps_mch_emc_0_Mem_CEN_pin(0 to 0),
      xps_mch_emc_0_Mem_WEN_pin => xps_mch_emc_0_Mem_WEN_pin,
      xps_mch_emc_0_Mem_OEN_pin => xps_mch_emc_0_Mem_OEN_pin(0 to 0),
      xps_mch_emc_0_RdClk_pin => xps_mch_emc_0_RdClk_pin
    );

end architecture STRUCTURE;

