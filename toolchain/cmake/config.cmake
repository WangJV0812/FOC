set(PROJECT_NAME_ "FOC")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_VERBOSE_MAKEFILE ON)

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
    # -D<macro>                   # use micro lib
)

set(FLASH_ADDRESS 0x08000000)

set(OPENOCD_CONFIG ${CMAKE_SOURCE_DIR}/toolchain/openocd_scripts/openocd-stm32g4-stlink.cfg )