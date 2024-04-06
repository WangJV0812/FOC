add_custom_target(
    download_stlink
    COMMAND openocd -f ${CMAKE_SOURCE_DIR}/toolchain/openocd_scripts/openocd-stm32g4-stlink.cfg 
                    -c "init" -c "halt" 
                    -c "flash write_image erase ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin 0x08000000"  
                    -c "reset"
                    -c "shutdown"
    COMMENT "Downloading firmware to target via openocd"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
)
