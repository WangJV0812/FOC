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
    ${CMAKE_CURRENT_LIST_DIR}/code/core/main.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/code/core/stm32g4xx_it.c
    # ${CMAKE_CURRENT_LIST_DIR}/code/core/system_stm32g4xx.c
    )

set(ASM_SOURCE
    ${STARTUP_ASM})

set_source_files_properties(${ASM_SOURCES} 
                            PROPERTIES COMPILE_FLAGS 
                            "-x assembler-with-cpp")

add_executable(${PROJECT_NAME} 
                ${COMMON_SERVICES_SOURCE} 
                ${USER_SOURCE} 
                ${LINKER_SCRIPT} 
                ${ASM_SOURCE})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code/abstraction/STM32G4xx_HAL_Driver)

# link math library
target_link_libraries(${PROJECT_NAME} m)



# 输出bin hex文件，后面可以再把 elf 文件加上
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Building hex & bin file..."
    COMMENT "EXCUTABLE SIZE:"
    COMMAND ${SIZE} ${PROJECT_NAME}
)