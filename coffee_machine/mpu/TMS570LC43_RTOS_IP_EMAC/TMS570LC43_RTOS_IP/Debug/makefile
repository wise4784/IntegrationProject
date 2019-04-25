################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/BufferAllocation_2.obj" \
"./source/FreeRTOS_DHCP.obj" \
"./source/FreeRTOS_DNS.obj" \
"./source/FreeRTOS_Sockets.obj" \
"./source/FreeRTOS_UDP_IP.obj" \
"./source/HL_emac.obj" \
"./source/HL_epc.obj" \
"./source/HL_errata.obj" \
"./source/HL_errata_SSWF021_45.obj" \
"./source/HL_esm.obj" \
"./source/HL_gio.obj" \
"./source/HL_mdio.obj" \
"./source/HL_nmpu.obj" \
"./source/HL_notification.obj" \
"./source/HL_phy_dp83640.obj" \
"./source/HL_phy_tlk111.obj" \
"./source/HL_pinmux.obj" \
"./source/HL_sci.obj" \
"./source/HL_sys_core.obj" \
"./source/HL_sys_dma.obj" \
"./source/HL_sys_intvecs.obj" \
"./source/HL_sys_main.obj" \
"./source/HL_sys_mpu.obj" \
"./source/HL_sys_pcr.obj" \
"./source/HL_sys_phantom.obj" \
"./source/HL_sys_pmm.obj" \
"./source/HL_sys_pmu.obj" \
"./source/HL_sys_startup.obj" \
"./source/HL_sys_vim.obj" \
"./source/HL_system.obj" \
"./source/NetworkInterface.obj" \
"./source/heap_4.obj" \
"./source/os_croutine.obj" \
"./source/os_event_groups.obj" \
"./source/os_list.obj" \
"./source/os_mpu_wrappers.obj" \
"./source/os_port.obj" \
"./source/os_portasm.obj" \
"./source/os_queue.obj" \
"./source/os_tasks.obj" \
"./source/os_timer.obj" \
"./source/sys_flush_cache.obj" \
"../source/HL_sys_link.cmd" \
$(GEN_CMDS__FLAG) \
-l"C:\ti\ccsv6\tools\compiler\ti-cgt-arm_5.2.5\lib\rtsv7R4_T_be_v3D16_eabi.lib" \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include source/subdir_vars.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
TMS570LC43_RTOS_IP.out \

EXE_OUTPUTS__QUOTED += \
"TMS570LC43_RTOS_IP.out" \

BIN_OUTPUTS += \
TMS570LC43_RTOS_IP.hex \

BIN_OUTPUTS__QUOTED += \
"TMS570LC43_RTOS_IP.hex" \


# All Target
all: TMS570LC43_RTOS_IP.out

# Tool invocations
TMS570LC43_RTOS_IP.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi -z -m"TMS570LC43_RTOS_IP.map" --heap_size=0x800 --stack_size=0x800 -i"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/lib" -i"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="TMS570LC43_RTOS_IP_linkInfo.xml" --rom_model --be32 -o "TMS570LC43_RTOS_IP.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

TMS570LC43_RTOS_IP.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armhex"  -o "TMS570LC43_RTOS_IP.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "source/BufferAllocation_2.obj" "source/FreeRTOS_DHCP.obj" "source/FreeRTOS_DNS.obj" "source/FreeRTOS_Sockets.obj" "source/FreeRTOS_UDP_IP.obj" "source/HL_emac.obj" "source/HL_epc.obj" "source/HL_errata.obj" "source/HL_errata_SSWF021_45.obj" "source/HL_esm.obj" "source/HL_gio.obj" "source/HL_mdio.obj" "source/HL_nmpu.obj" "source/HL_notification.obj" "source/HL_phy_dp83640.obj" "source/HL_phy_tlk111.obj" "source/HL_pinmux.obj" "source/HL_sci.obj" "source/HL_sys_core.obj" "source/HL_sys_dma.obj" "source/HL_sys_intvecs.obj" "source/HL_sys_main.obj" "source/HL_sys_mpu.obj" "source/HL_sys_pcr.obj" "source/HL_sys_phantom.obj" "source/HL_sys_pmm.obj" "source/HL_sys_pmu.obj" "source/HL_sys_startup.obj" "source/HL_sys_vim.obj" "source/HL_system.obj" "source/NetworkInterface.obj" "source/heap_4.obj" "source/os_croutine.obj" "source/os_event_groups.obj" "source/os_list.obj" "source/os_mpu_wrappers.obj" "source/os_port.obj" "source/os_portasm.obj" "source/os_queue.obj" "source/os_tasks.obj" "source/os_timer.obj" 
	-$(RM) "source/sys_flush_cache.obj" 
	-$(RM) "source/BufferAllocation_2.d" "source/FreeRTOS_DHCP.d" "source/FreeRTOS_DNS.d" "source/FreeRTOS_Sockets.d" "source/FreeRTOS_UDP_IP.d" "source/HL_emac.d" "source/HL_epc.d" "source/HL_errata.d" "source/HL_errata_SSWF021_45.d" "source/HL_esm.d" "source/HL_gio.d" "source/HL_mdio.d" "source/HL_nmpu.d" "source/HL_notification.d" "source/HL_phy_dp83640.d" "source/HL_phy_tlk111.d" "source/HL_pinmux.d" "source/HL_sci.d" "source/HL_sys_dma.d" "source/HL_sys_main.d" "source/HL_sys_pcr.d" "source/HL_sys_phantom.d" "source/HL_sys_pmm.d" "source/HL_sys_startup.d" "source/HL_sys_vim.d" "source/HL_system.d" "source/NetworkInterface.d" "source/heap_4.d" "source/os_croutine.d" "source/os_event_groups.d" "source/os_list.d" "source/os_mpu_wrappers.d" "source/os_port.d" "source/os_queue.d" "source/os_tasks.d" "source/os_timer.d" 
	-$(RM) "source/HL_sys_core.d" "source/HL_sys_intvecs.d" "source/HL_sys_mpu.d" "source/HL_sys_pmu.d" "source/os_portasm.d" "source/sys_flush_cache.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

