set(PROJECT_NAME_ "FOC")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(OUTPUT_ELF_NAME "FOC")

# need -D<macro> to define macro
add_definitions(
    -DUSE_HAL_DRIVER
    -DARM_MATH_CM4
    -DUSE_STDPERIPH_DRIVER
    -DSTM32
    -DSTM32G474xx               # device
    -DHSE_VALUE=8000000         # HSE clock frequency
    -DFOC                       # project name
    -DUSING_NON_RTOS=0          # sue RTOS
    -DUSING_FREERTOS=1          # use FreeRTOS
    -DUSING_THREADX=2           # use Threadx
    -DUSING_RTOS=USING_NON_RTOS # use RTOS in our project
    # -DMARCO                     # use micro lib
)

add_definitions(
    -DARM_MATH_MATRIX_CHECK     # check the dimension when performing matrix multiplication   
    -DARM_MATH_ROUNDING         # enable aggrassive rounding strategies for some function that require less precision
    -DARM_MATH_LOOPUNROLL       # expand the loop to accelerate computing
)

set(FLASH_ADDRESS 0x08000000)

set(OPENOCD_STLINK_CONFIG ${CMAKE_SOURCE_DIR}/toolchain/openocd_scripts/openocd-stm32g4-stlink.cfg )
set(OPENOCD_JLINK_CONFIG ${CMAKE_SOURCE_DIR}/toolchain/openocd_scripts/openocd-stm32g4-stlink.cfg )
set(OPENOCD_DAPLINK_CONFIG ${CMAKE_SOURCE_DIR}/toolchain/openocd_scripts/openocd-stm32g4-daplink.cfg )

set(IS_USE_DSPLIB OFF)