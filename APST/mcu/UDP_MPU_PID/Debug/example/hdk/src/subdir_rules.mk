################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
example/hdk/src/lwip_main.obj: /home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/example/hdk/src/lwip_main.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/kiott01a/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID" --include_path="/home/kiott01a/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/include" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/example/hdk/inc" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/src/include" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/src/include/ipv4" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/ports/hdk/check" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/ports/hdk/include/netif" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/ports/hdk/include" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/ports/hdk/netif" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/ports/hdk" --include_path="/home/kiott01a/final_project/IntegrationProject/APST/mcu/UDP_MPU_PID/lwip-1.4.1/apps/httpserver_raw" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="example/hdk/src/$(basename $(<F)).d_raw" --obj_directory="example/hdk/src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


