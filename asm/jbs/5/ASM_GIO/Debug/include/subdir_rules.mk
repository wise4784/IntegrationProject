################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
include/%.obj: ../include/%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/jbs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 --include_path="/home/jbs/proj/IntegrationProject/asm/jbs/5/ASM_GIO" --include_path="/home/jbs/proj/IntegrationProject/asm/jbs/5/ASM_GIO/include" --include_path="/home/jbs/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="include/$(basename $(<F)).d_raw" --obj_directory="include" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


