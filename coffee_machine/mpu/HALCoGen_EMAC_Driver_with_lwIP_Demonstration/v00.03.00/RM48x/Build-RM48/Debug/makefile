################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/arm_5.1.6

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./HALCoGen-RM48/source/system.obj" \
"./HALCoGen-RM48/source/sys_vim.obj" \
"./HALCoGen-RM48/source/sys_startup.obj" \
"./HALCoGen-RM48/source/sys_selftest.obj" \
"./HALCoGen-RM48/source/sys_pmu.obj" \
"./HALCoGen-RM48/source/sys_pmm.obj" \
"./HALCoGen-RM48/source/sys_phantom.obj" \
"./HALCoGen-RM48/source/sys_pcr.obj" \
"./HALCoGen-RM48/source/sys_mpu.obj" \
"./HALCoGen-RM48/source/sys_main.obj" \
"./HALCoGen-RM48/source/sys_intvecs.obj" \
"./HALCoGen-RM48/source/sys_dma.obj" \
"./HALCoGen-RM48/source/sys_core.obj" \
"./HALCoGen-RM48/source/sci.obj" \
"./HALCoGen-RM48/source/pinmux.obj" \
"./HALCoGen-RM48/source/phy_dp83640.obj" \
"./HALCoGen-RM48/source/notification.obj" \
"./HALCoGen-RM48/source/mdio.obj" \
"./HALCoGen-RM48/source/esm.obj" \
"./HALCoGen-RM48/source/emac.obj" \
"./HALCoGen-RM48/source/dabort.obj" \
"./example/hdk/src/lwip_main.obj" \
"./lwip-1.4.1/apps/httpserver_raw/httpd.obj" \
"./lwip-1.4.1/apps/httpserver_raw/fs.obj" \
"./lwip-1.4.1/ports/hdk/lwiplib.obj" \
"./lwip-1.4.1/src/core/timers.obj" \
"./lwip-1.4.1/src/core/def.obj" \
"./lwip-1.4.1/src/netif/ethernetif.obj" \
"C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.02.00/RM48x/HALCoGen-RM48/source/sys_link.cmd" \
-l"rtsv7R4_T_le_v3D16_eabi.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include lwip-1.4.1/src/netif/subdir_vars.mk
-include lwip-1.4.1/src/core/subdir_vars.mk
-include lwip-1.4.1/ports/hdk/subdir_vars.mk
-include lwip-1.4.1/apps/httpserver_raw/subdir_vars.mk
-include example/hdk/src/subdir_vars.mk
-include HALCoGen-RM48/source/subdir_vars.mk
-include lwip-1.4.1/src/netif/subdir_rules.mk
-include lwip-1.4.1/src/core/subdir_rules.mk
-include lwip-1.4.1/ports/hdk/subdir_rules.mk
-include lwip-1.4.1/apps/httpserver_raw/subdir_rules.mk
-include example/hdk/src/subdir_rules.mk
-include HALCoGen-RM48/source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Build-RM48.out \

EXE_OUTPUTS__QUOTED += \
"Build-RM48.out" \


# All Target
all: Build-RM48.out

# Tool invocations
Build-RM48.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --abi=eabi -me -g --diag_warning=225 --display_error_number --diag_wrap=off --enum_type=packed -z -m"Build-RM48.map" -i"c:/ti/ccsv6/tools/compiler/arm_5.1.6/lib" -i"c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="Build-RM48_linkInfo.xml" --rom_model -o "Build-RM48.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "lwip-1.4.1\src\netif\ethernetif.pp" "lwip-1.4.1\src\core\def.pp" "lwip-1.4.1\src\core\timers.pp" "lwip-1.4.1\ports\hdk\lwiplib.pp" "lwip-1.4.1\apps\httpserver_raw\fs.pp" "lwip-1.4.1\apps\httpserver_raw\httpd.pp" "example\hdk\src\lwip_main.pp" "HALCoGen-RM48\source\emac.pp" "HALCoGen-RM48\source\esm.pp" "HALCoGen-RM48\source\mdio.pp" "HALCoGen-RM48\source\notification.pp" "HALCoGen-RM48\source\phy_dp83640.pp" "HALCoGen-RM48\source\pinmux.pp" "HALCoGen-RM48\source\sci.pp" "HALCoGen-RM48\source\sys_dma.pp" "HALCoGen-RM48\source\sys_main.pp" "HALCoGen-RM48\source\sys_pcr.pp" "HALCoGen-RM48\source\sys_phantom.pp" "HALCoGen-RM48\source\sys_pmm.pp" "HALCoGen-RM48\source\sys_selftest.pp" "HALCoGen-RM48\source\sys_startup.pp" "HALCoGen-RM48\source\sys_vim.pp" "HALCoGen-RM48\source\system.pp" 
	-$(RM) "lwip-1.4.1\src\netif\ethernetif.obj" "lwip-1.4.1\src\core\def.obj" "lwip-1.4.1\src\core\timers.obj" "lwip-1.4.1\ports\hdk\lwiplib.obj" "lwip-1.4.1\apps\httpserver_raw\fs.obj" "lwip-1.4.1\apps\httpserver_raw\httpd.obj" "example\hdk\src\lwip_main.obj" "HALCoGen-RM48\source\dabort.obj" "HALCoGen-RM48\source\emac.obj" "HALCoGen-RM48\source\esm.obj" "HALCoGen-RM48\source\mdio.obj" "HALCoGen-RM48\source\notification.obj" "HALCoGen-RM48\source\phy_dp83640.obj" "HALCoGen-RM48\source\pinmux.obj" "HALCoGen-RM48\source\sci.obj" "HALCoGen-RM48\source\sys_core.obj" "HALCoGen-RM48\source\sys_dma.obj" "HALCoGen-RM48\source\sys_intvecs.obj" "HALCoGen-RM48\source\sys_main.obj" "HALCoGen-RM48\source\sys_mpu.obj" "HALCoGen-RM48\source\sys_pcr.obj" "HALCoGen-RM48\source\sys_phantom.obj" "HALCoGen-RM48\source\sys_pmm.obj" "HALCoGen-RM48\source\sys_pmu.obj" "HALCoGen-RM48\source\sys_selftest.obj" "HALCoGen-RM48\source\sys_startup.obj" "HALCoGen-RM48\source\sys_vim.obj" "HALCoGen-RM48\source\system.obj" 
	-$(RM) "HALCoGen-RM48\source\dabort.pp" "HALCoGen-RM48\source\sys_core.pp" "HALCoGen-RM48\source\sys_intvecs.pp" "HALCoGen-RM48\source\sys_mpu.pp" "HALCoGen-RM48\source\sys_pmu.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

