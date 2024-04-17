cmake_minimum_required(VERSION 3.16)

# you must include cortex_m4f.cmake before create thr cmake project
# cmake can not check the compiler by compiling a simple project when we ues Cross compiler
# you need disable the compiler checker.
include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/cortex_m4f.cmake)

project(FOC C CXX ASM)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/config.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/complie.cmake)

#################### add files to compiler ####################
SET(USER_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/code/core/Src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/core/Src/stm32g4xx_it.c
    ${CMAKE_CURRENT_LIST_DIR}/code/core/Src/system_stm32g4xx.c
)


# add public head files
include_directories (
    ${CMAKE_CURRENT_LIST_DIR}/code/core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/code/core/config
    ${CMAKE_CURRENT_LIST_DIR}/code/abstraction/CMSIS/Include

)

set(ASM_SOURCE
    ${STARTUP_ASM}
)

set_source_files_properties (
    ${ASM_SOURCES} 
    PROPERTIES COMPILE_FLAGS 
    "-x assembler-with-cpp"
)

add_executable(${PROJECT_NAME}.elf
                ${COMMON_SERVICES_SOURCE} 
                ${USER_SOURCE} 
                ${LINKER_SCRIPT} 
                ${ASM_SOURCE})

#################### add submodules to compiler ####################
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/CMSIS)

if(IS_USE_DSPLIB)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/lib/dsp/Source)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/STM32G4xx_HAL_Driver)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/bsp)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/module)


#################### link math library ####################
target_link_libraries(
    ${PROJECT_NAME}.elf 
    m       # Math lib
)

# 输出bin hex文件
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Building hex & bin file..."
    COMMENT "EXCUTABLE SIZE:"
    COMMAND ${SIZE} ${PROJECT_NAME}.elf
)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain/cmake/custom_rule.cmake)