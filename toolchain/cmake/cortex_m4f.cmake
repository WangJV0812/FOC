SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)

SET(THREADX_ARCH "cortex_m4")
SET(THREADX_TOOLCHAIN "gnu")

ADD_DEFINITIONS(
    -DARM_MATH_CM4 
    -DARM_MATH_MATRIX_CHECK 
    -DARM_MATH_ROUNDING 
)

# you can find the valid compile options in https://developer.arm.com/documentation/100748/0621/Using-Common-Compiler-Options/Common-Arm-Compiler-for-Embedded-toolchain-options?lang=en
SET(MCPU_FLAGS -mthumb -mthumb-interwork -mcpu=cortex-m4)
SET(VFP_FLAGS -mfloat-abi=hard -mfpu=fpv4-sp-d16)

MESSAGE(STATUS "**** Platform: ${MCPU_FLAGS} ${VFP_FLAGS} ****")

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../STM32G474VETx_FLASH.ld)

set(STARTUP_ASM ${CMAKE_CURRENT_LIST_DIR}/../startup_stm32g474xx.s)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/arm_none_eabi.cmake)
