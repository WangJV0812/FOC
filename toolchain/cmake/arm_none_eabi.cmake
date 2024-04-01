set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

message("-- **** disabel compiler check ****")
set(CMAKE_C_COMPILER_FORCED TRUE) # skip compiler test
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# 指定工具链
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)



set(SIZE arm-none-eabi-size) 
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 


# 使用的 C 语言版本；
SET(CMAKE_C_STANDARD 17)
# 使用的 cpp 版本；
SET(CMAKE_CXX_STANDARD 17)
# 生成 compile_commands.json，可配合 clangd 实现精准的代码关联与跳转；
SET(CMAKE_EXPORT_COMPILE_COMMANDS True)
# 彩色日志输出；
SET(CMAKE_COLOR_DIAGNOSTICS True)

# 路径查找，保证只会搜索交叉编译工具链指定的路径，而不会搜索主机系统的默认路径
# 设置了在搜索可执行文件（程序）时的路径模式, NEVER 表示不会使用交叉编译工具链的根目录寻找文件
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 设置了在搜索库文件时的路径模式，ONLY 表示只使用交叉编译工具链的根目录来搜索库文件，而不会搜索系统默认的路径
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

# 设置了在搜索头文件时的路径模式，ONLY 表示只使用交叉编译工具链的根目录来搜索头文件，而不会搜索系统默认的路径
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 设置了在搜索 CMake 包配置文件时的路径模式，ONLY 表示只使用交叉编译工具链的根目录来搜索 CMake 包配置文件，而不会搜索系统默认的路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)