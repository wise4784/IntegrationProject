################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
lwIP_INSTALL_ROOT/src/netif/ethernetif.obj: /home/koitt/project/IntegrationProject/coffee_machine/mpu/HALCoGen_EMAC_Driver_with_lwIP_Demonstration/v00.03.00/lwip-1.4.1/src/netif/ethernetif.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/TMS570LC43_LWIP_MINIMAL" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/HALCoGen_EMAC_Driver_with_lwIP_Demonstration/v00.03.00/lwip-1.4.1/src/include/ipv4" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/HALCoGen_EMAC_Driver_with_lwIP_Demonstration/v00.03.00/lwip-1.4.1/ports/hdk/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/HALCoGen_EMAC_Driver_with_lwIP_Demonstration/v00.03.00/lwip-1.4.1/src/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/HALCoGen_EMAC_Driver_with_lwIP_Demonstration/v00.03.00/lwip-1.4.1" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/TMS570LC43_LWIP_MINIMAL/include" --include_path="/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="lwIP_INSTALL_ROOT/src/netif/$(basename $(<F)).d_raw" --obj_directory="lwIP_INSTALL_ROOT/src/netif" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


