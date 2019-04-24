################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS-Plus-TCP/portable/NetworkInterface/Hercules03/NetworkInterface.obj: ../FreeRTOS-Plus-TCP/portable/NetworkInterface/Hercules03/NetworkInterface.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 -O4 --opt_for_speed=0 --fp_mode=relaxed --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/source" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-CLI" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-CLI/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-IO/Include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-IO/Device/tms570ls20x31x/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-IO/Device/tms570ls20x31x/SupportedBoards" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-TCP/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-TCP/protocols/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-TCP/portable/Compiler/CCS" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-TCP" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-FAT/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-FAT/portable/common" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-UDP-Console/include" --include_path="C:/Users/Loszi/workspace_v6_2/LaunchPad_FreeRTOS/FreeRTOS-Plus-UART-Console/include" -g --gcc --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --enum_type=packed --preproc_with_compile --preproc_dependency="FreeRTOS-Plus-TCP/portable/NetworkInterface/Hercules03/NetworkInterface.d" --obj_directory="FreeRTOS-Plus-TCP/portable/NetworkInterface/Hercules03" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


