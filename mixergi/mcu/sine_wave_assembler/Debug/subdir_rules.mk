################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jabinkim/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 --include_path="/home/jabinkim/IntegrationProject/mixergi/mcu/sine_wave_assembler" --include_path="/home/jabinkim/IntegrationProject/mixergi/mcu/sine_wave_assembler/include" --include_path="/home/jabinkim/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

