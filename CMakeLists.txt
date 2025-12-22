cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
add_compile_definitions(STM32F401xC)

set(CMAKE_C_COMPILER   /home/maxmaster/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /home/maxmaster/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER /home/maxmaster/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc)

project(arm_flash_test C CXX ASM)



file(GLOB 
    SOURCES
    CONFIGURE_DEPENDS
    src/*/*.c
    src/*/*/*.c
    src/main.c
    /home/maxmaster/.platformio/packages/framework-cmsis-stm32f4/Source/Templates/gcc/startup_stm32f401xc.S
    /home/maxmaster/.platformio/packages/framework-cmsis-stm32f4/Source/Templates/system_stm32f4xx.c
)


add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -ffunction-sections
    -fdata-sections
    -Wall
    -O2
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -T/home/maxmaster/.platformio/packages/tool-ldscripts-ststm32/stm32f4/STM32F401CCFX_FLASH.ld
    -Wl,--gc-sections
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        /home/maxmaster/.platformio/packages/framework-cmsis-stm32f4/Include
        /home/maxmaster/.platformio/packages/framework-cmsis/CMSIS/Core/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/MCAL
        ${CMAKE_CURRENT_SOURCE_DIR}/include/HAL
        ${CMAKE_CURRENT_SOURCE_DIR}/include/service
)


set(OPENOCD "/home/maxmaster/.platformio/packages/tool-openocd")
set(OPENOCD_BIN "${OPENOCD}/bin/openocd")
set(OPENOCD_SCRIPTS "${OPENOCD}/openocd/scripts")

add_custom_target(flash
    COMMAND ${OPENOCD_BIN}
        -s ${OPENOCD_SCRIPTS}
        -f interface/stlink.cfg
        -f target/stm32f4x.cfg
        -c "program $<TARGET_FILE:${PROJECT_NAME}> verify reset exit"
    DEPENDS ${PROJECT_NAME}
)
