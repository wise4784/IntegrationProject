################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../source/HL_sys_link.cmd 

ASM_SRCS += \
../source/HL_sys_core.asm \
../source/HL_sys_intvecs.asm \
../source/HL_sys_mpu.asm \
../source/HL_sys_pmu.asm \
../source/os_portasm.asm \
../source/sys_flush_cache.asm 

C_SRCS += \
../source/BufferAllocation_2.c \
../source/FreeRTOS_DHCP.c \
../source/FreeRTOS_DNS.c \
../source/FreeRTOS_Sockets.c \
../source/FreeRTOS_UDP_IP.c \
../source/HL_emac.c \
../source/HL_epc.c \
../source/HL_errata.c \
../source/HL_errata_SSWF021_45.c \
../source/HL_esm.c \
../source/HL_gio.c \
../source/HL_mdio.c \
../source/HL_nmpu.c \
../source/HL_notification.c \
../source/HL_phy_dp83640.c \
../source/HL_phy_tlk111.c \
../source/HL_pinmux.c \
../source/HL_sci.c \
../source/HL_sys_dma.c \
../source/HL_sys_main.c \
../source/HL_sys_pcr.c \
../source/HL_sys_phantom.c \
../source/HL_sys_pmm.c \
../source/HL_sys_startup.c \
../source/HL_sys_vim.c \
../source/HL_system.c \
../source/NetworkInterface.c \
../source/heap_4.c \
../source/os_croutine.c \
../source/os_event_groups.c \
../source/os_list.c \
../source/os_mpu_wrappers.c \
../source/os_port.c \
../source/os_queue.c \
../source/os_tasks.c \
../source/os_timer.c 

C_DEPS += \
./source/BufferAllocation_2.d \
./source/FreeRTOS_DHCP.d \
./source/FreeRTOS_DNS.d \
./source/FreeRTOS_Sockets.d \
./source/FreeRTOS_UDP_IP.d \
./source/HL_emac.d \
./source/HL_epc.d \
./source/HL_errata.d \
./source/HL_errata_SSWF021_45.d \
./source/HL_esm.d \
./source/HL_gio.d \
./source/HL_mdio.d \
./source/HL_nmpu.d \
./source/HL_notification.d \
./source/HL_phy_dp83640.d \
./source/HL_phy_tlk111.d \
./source/HL_pinmux.d \
./source/HL_sci.d \
./source/HL_sys_dma.d \
./source/HL_sys_main.d \
./source/HL_sys_pcr.d \
./source/HL_sys_phantom.d \
./source/HL_sys_pmm.d \
./source/HL_sys_startup.d \
./source/HL_sys_vim.d \
./source/HL_system.d \
./source/NetworkInterface.d \
./source/heap_4.d \
./source/os_croutine.d \
./source/os_event_groups.d \
./source/os_list.d \
./source/os_mpu_wrappers.d \
./source/os_port.d \
./source/os_queue.d \
./source/os_tasks.d \
./source/os_timer.d 

OBJS += \
./source/BufferAllocation_2.obj \
./source/FreeRTOS_DHCP.obj \
./source/FreeRTOS_DNS.obj \
./source/FreeRTOS_Sockets.obj \
./source/FreeRTOS_UDP_IP.obj \
./source/HL_emac.obj \
./source/HL_epc.obj \
./source/HL_errata.obj \
./source/HL_errata_SSWF021_45.obj \
./source/HL_esm.obj \
./source/HL_gio.obj \
./source/HL_mdio.obj \
./source/HL_nmpu.obj \
./source/HL_notification.obj \
./source/HL_phy_dp83640.obj \
./source/HL_phy_tlk111.obj \
./source/HL_pinmux.obj \
./source/HL_sci.obj \
./source/HL_sys_core.obj \
./source/HL_sys_dma.obj \
./source/HL_sys_intvecs.obj \
./source/HL_sys_main.obj \
./source/HL_sys_mpu.obj \
./source/HL_sys_pcr.obj \
./source/HL_sys_phantom.obj \
./source/HL_sys_pmm.obj \
./source/HL_sys_pmu.obj \
./source/HL_sys_startup.obj \
./source/HL_sys_vim.obj \
./source/HL_system.obj \
./source/NetworkInterface.obj \
./source/heap_4.obj \
./source/os_croutine.obj \
./source/os_event_groups.obj \
./source/os_list.obj \
./source/os_mpu_wrappers.obj \
./source/os_port.obj \
./source/os_portasm.obj \
./source/os_queue.obj \
./source/os_tasks.obj \
./source/os_timer.obj \
./source/sys_flush_cache.obj 

ASM_DEPS += \
./source/HL_sys_core.d \
./source/HL_sys_intvecs.d \
./source/HL_sys_mpu.d \
./source/HL_sys_pmu.d \
./source/os_portasm.d \
./source/sys_flush_cache.d 

OBJS__QUOTED += \
"source/BufferAllocation_2.obj" \
"source/FreeRTOS_DHCP.obj" \
"source/FreeRTOS_DNS.obj" \
"source/FreeRTOS_Sockets.obj" \
"source/FreeRTOS_UDP_IP.obj" \
"source/HL_emac.obj" \
"source/HL_epc.obj" \
"source/HL_errata.obj" \
"source/HL_errata_SSWF021_45.obj" \
"source/HL_esm.obj" \
"source/HL_gio.obj" \
"source/HL_mdio.obj" \
"source/HL_nmpu.obj" \
"source/HL_notification.obj" \
"source/HL_phy_dp83640.obj" \
"source/HL_phy_tlk111.obj" \
"source/HL_pinmux.obj" \
"source/HL_sci.obj" \
"source/HL_sys_core.obj" \
"source/HL_sys_dma.obj" \
"source/HL_sys_intvecs.obj" \
"source/HL_sys_main.obj" \
"source/HL_sys_mpu.obj" \
"source/HL_sys_pcr.obj" \
"source/HL_sys_phantom.obj" \
"source/HL_sys_pmm.obj" \
"source/HL_sys_pmu.obj" \
"source/HL_sys_startup.obj" \
"source/HL_sys_vim.obj" \
"source/HL_system.obj" \
"source/NetworkInterface.obj" \
"source/heap_4.obj" \
"source/os_croutine.obj" \
"source/os_event_groups.obj" \
"source/os_list.obj" \
"source/os_mpu_wrappers.obj" \
"source/os_port.obj" \
"source/os_portasm.obj" \
"source/os_queue.obj" \
"source/os_tasks.obj" \
"source/os_timer.obj" \
"source/sys_flush_cache.obj" 

C_DEPS__QUOTED += \
"source/BufferAllocation_2.d" \
"source/FreeRTOS_DHCP.d" \
"source/FreeRTOS_DNS.d" \
"source/FreeRTOS_Sockets.d" \
"source/FreeRTOS_UDP_IP.d" \
"source/HL_emac.d" \
"source/HL_epc.d" \
"source/HL_errata.d" \
"source/HL_errata_SSWF021_45.d" \
"source/HL_esm.d" \
"source/HL_gio.d" \
"source/HL_mdio.d" \
"source/HL_nmpu.d" \
"source/HL_notification.d" \
"source/HL_phy_dp83640.d" \
"source/HL_phy_tlk111.d" \
"source/HL_pinmux.d" \
"source/HL_sci.d" \
"source/HL_sys_dma.d" \
"source/HL_sys_main.d" \
"source/HL_sys_pcr.d" \
"source/HL_sys_phantom.d" \
"source/HL_sys_pmm.d" \
"source/HL_sys_startup.d" \
"source/HL_sys_vim.d" \
"source/HL_system.d" \
"source/NetworkInterface.d" \
"source/heap_4.d" \
"source/os_croutine.d" \
"source/os_event_groups.d" \
"source/os_list.d" \
"source/os_mpu_wrappers.d" \
"source/os_port.d" \
"source/os_queue.d" \
"source/os_tasks.d" \
"source/os_timer.d" 

ASM_DEPS__QUOTED += \
"source/HL_sys_core.d" \
"source/HL_sys_intvecs.d" \
"source/HL_sys_mpu.d" \
"source/HL_sys_pmu.d" \
"source/os_portasm.d" \
"source/sys_flush_cache.d" 

C_SRCS__QUOTED += \
"../source/BufferAllocation_2.c" \
"../source/FreeRTOS_DHCP.c" \
"../source/FreeRTOS_DNS.c" \
"../source/FreeRTOS_Sockets.c" \
"../source/FreeRTOS_UDP_IP.c" \
"../source/HL_emac.c" \
"../source/HL_epc.c" \
"../source/HL_errata.c" \
"../source/HL_errata_SSWF021_45.c" \
"../source/HL_esm.c" \
"../source/HL_gio.c" \
"../source/HL_mdio.c" \
"../source/HL_nmpu.c" \
"../source/HL_notification.c" \
"../source/HL_phy_dp83640.c" \
"../source/HL_phy_tlk111.c" \
"../source/HL_pinmux.c" \
"../source/HL_sci.c" \
"../source/HL_sys_dma.c" \
"../source/HL_sys_main.c" \
"../source/HL_sys_pcr.c" \
"../source/HL_sys_phantom.c" \
"../source/HL_sys_pmm.c" \
"../source/HL_sys_startup.c" \
"../source/HL_sys_vim.c" \
"../source/HL_system.c" \
"../source/NetworkInterface.c" \
"../source/heap_4.c" \
"../source/os_croutine.c" \
"../source/os_event_groups.c" \
"../source/os_list.c" \
"../source/os_mpu_wrappers.c" \
"../source/os_port.c" \
"../source/os_queue.c" \
"../source/os_tasks.c" \
"../source/os_timer.c" 

ASM_SRCS__QUOTED += \
"../source/HL_sys_core.asm" \
"../source/HL_sys_intvecs.asm" \
"../source/HL_sys_mpu.asm" \
"../source/HL_sys_pmu.asm" \
"../source/os_portasm.asm" \
"../source/sys_flush_cache.asm" 


