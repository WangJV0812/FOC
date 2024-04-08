add_custom_target(
    download_stlink
    COMMAND openocd -f ${OPENOCD_STLINK_CONFIG}
                    -c "init" -c "halt" 
                    -c "flash write_image erase ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin ${FLASH_ADDRESS}"   # programming MUS's xflash
                    -c "reset"      # reset the target MCU
                    -c "shutdown"   # shutdown openocd server
    COMMENT "Downloading firmware to target via openocd"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
)

add_custom_target(
    download_jlink
    COMMAND openocd -f ${OPENOCD_JLINK_CONFIG}
                    -c "init" -c "halt" 
                    -c "flash write_image erase ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin ${FLASH_ADDRESS}"   # programming MUS's xflash
                    -c "reset"      # reset the target MCU
                    -c "shutdown"   # shutdown openocd server
    COMMENT "Downloading firmware to target via openocd"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
)
