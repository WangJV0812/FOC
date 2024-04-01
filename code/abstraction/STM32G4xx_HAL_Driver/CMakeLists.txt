aux_source_directory(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}\\Src)

target_sources(${CMAKE_PROJECT_NAME}
                PRIVATE ${SRC_PATH})

include_directories(${CMAKE_PROJECT_NAME}
                    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Inc
                    ${CMAKE_CURRENT_LIST_DIR}/Inc/Legacy)