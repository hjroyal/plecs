CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(cmake_demo VERSION 1.0.0)
# print message
# project()：非强制，会生许多有用的变量
message("cmake_demo_BINARY_DIR = ${cmake_demo_BINARY_DIR}, PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")

SET(CMAKE_C_STANDARD 11)

# Set the output folder where your program will be created

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib) #静态库的默认输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib) #动态库linux .so的默认输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/bin) #动态库windows .dll的默认输出路径


# 自定义宏，代码中可以使用 
if (CMAKE_HOST_UNIX)
    ADD_DEFINITIONS(-DMY_LINUX)
else ()
    ADD_DEFINITIONS(-DMY_WINDOWS)
endif()
	 
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(module)
ADD_SUBDIRECTORY(app)

