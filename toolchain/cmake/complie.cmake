add_compile_options(-pipe 
                    ${MCPU_FLAGS} ${VFP_FLAGS} 
                    -Werror
                    -Wall -fmessage-length=0
                    -ffunction-sections -fdata-sections -fno-common )
                    
add_link_options(-pipe
                ${MCPU_FLAGS} ${VFP_FLAGS} 
                -T${LINKER_SCRIPT} 
                -Wl,--no-warn-rwx-segments # close RWX warning
                -lm -lc -lnosys # lib options
                -Wl,--gc-sections -flto
                -specs=nosys.specs
                #  -flto -specs=nano.specs -specs=nosys.specs -specs=rdimon.specs # optimize options
                -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--cref -Wl,--print-memory-usage # map options
                ) # if your executable is too large , try option '-s' to strip symbols


# Build types
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g -gdwarf-2)
    add_definitions(-DESC_DEBUG) # ESC Debug
endif ()

MESSAGE(STATUS "**** Build for ${CMAKE_BUILD_TYPE} ****")

# 设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib_obj)

SET(ELF_FILE ${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.elf)
SET(HEX_FILE ${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.hex)
SET(BIN_FILE ${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.bin)
