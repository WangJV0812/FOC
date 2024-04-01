cmake_minimum_required(VERSION 3.16)

# you must include cortex_m4f.cmake before create thr cmake project
# cmake can not check the compiler by compiling a simple project when we ues Cross compiler
# you need disable the compiler checker.
include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/cortex_m4f.cmake)

project(FOC C CXX ASM) 

include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/config.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/complie.cmake)

# 生成编译文件
SET(USER_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/code/core/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/core/stm32g4xx_it.c
    ${CMAKE_CURRENT_LIST_DIR}/code/core/system_stm32g4xx.c)

# 编译结果
ADD_EXECUTABLE(${PROJECT_NAME} ${COMMON_SERVICES_SOURCE} ${USER_SOURCE} ${LINKER_SCRIPT} ${STARTUP_ASM})

# 定义连接器参数； --gc-sections：指示链接器去掉不用的 section
SET(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.map -Wl,--gc-sections,--print-memory-usage"
)




add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/STM32G4xx_HAL_Driver)





# 输出bin hex文件，后面可以再把 elf 文件加上
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Building hex & bin file..."
    COMMENT "EXCUTABLE SIZE:"
    COMMAND ${SIZE} ${PROJECT_NAME}
)