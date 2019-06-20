vlib work
vlib activehdl

vlib activehdl/xilinx_vip
vlib activehdl/xil_defaultlib
vlib activehdl/xpm
vlib activehdl/axi_infrastructure_v1_1_0
vlib activehdl/smartconnect_v1_0
vlib activehdl/axi_protocol_checker_v2_0_3
vlib activehdl/axi_vip_v1_1_3
vlib activehdl/processing_system7_vip_v1_0_5
vlib activehdl/xlconstant_v1_1_5

vmap xilinx_vip activehdl/xilinx_vip
vmap xil_defaultlib activehdl/xil_defaultlib
vmap xpm activehdl/xpm
vmap axi_infrastructure_v1_1_0 activehdl/axi_infrastructure_v1_1_0
vmap smartconnect_v1_0 activehdl/smartconnect_v1_0
vmap axi_protocol_checker_v2_0_3 activehdl/axi_protocol_checker_v2_0_3
vmap axi_vip_v1_1_3 activehdl/axi_vip_v1_1_3
vmap processing_system7_vip_v1_0_5 activehdl/processing_system7_vip_v1_0_5
vmap xlconstant_v1_1_5 activehdl/xlconstant_v1_1_5

vlog -work xilinx_vip  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/axi_vip_if.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/clk_vip_if.sv" \
"/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work xil_defaultlib  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"/opt/Xilinx/Vivado/2018.2/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"/opt/Xilinx/Vivado/2018.2/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \
"/opt/Xilinx/Vivado/2018.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \

vcom -work xpm -93 \
"/opt/Xilinx/Vivado/2018.2/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work axi_infrastructure_v1_1_0  -v2k5 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work smartconnect_v1_0  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/sc_util_v1_0_vl_rfs.sv" \

vlog -work axi_protocol_checker_v2_0_3  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/03a9/hdl/axi_protocol_checker_v2_0_vl_rfs.sv" \

vlog -work axi_vip_v1_1_3  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/b9a8/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work processing_system7_vip_v1_0_5  -sv2k12 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ip/ps_spi_processing_system7_0_0/sim/ps_spi_processing_system7_0_0.v" \

vlog -work xlconstant_v1_1_5  -v2k5 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/f1c3/hdl/xlconstant_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -v2k5 "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/ec67/hdl" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/5bb9/hdl/verilog" "+incdir+../../../../ps_spi.srcs/sources_1/bd/ps_spi/ipshared/70fd/hdl" "+incdir+/opt/Xilinx/Vivado/2018.2/data/xilinx_vip/include" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/ip/ps_spi_xlconstant_0_0/sim/ps_spi_xlconstant_0_0.v" \
"../../../../ps_spi.srcs/sources_1/bd/ps_spi/sim/ps_spi.v" \

vlog -work xil_defaultlib \
"glbl.v"

