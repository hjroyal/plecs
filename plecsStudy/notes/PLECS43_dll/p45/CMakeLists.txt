cmake_minimum_required(VERSION 3.10.0)

# 项目名称和语言
set(Proj_Name "lininterp-dll")
project(${Proj_Name} LANGUAGES C)  


# 使用的语言标准是C89/ANSI C
set(CMAKE_C_STANDARD 99)


set(PLECS_DLL_PATH "C:\\env\\PLECS49\\Standalone\\include\\plecs") # 动态库搜索文件夹


# 头文件搜索目录
include_directories("include" ${PLECS_DLL_PATH} )


# 源码文件
# aux_source_directory("src" Src_List) #遍历src文件夹的所有源码文件, 存放在变量Src_List上
set(Src_List ${Src_List} "lininterp.c")

#生成动态库
set(Dll_Name "lininterp")

add_library(${Dll_Name} SHARED ${Src_List}) #生成动态库, 名称为Dll_Name, 类型为SHARED, 源码文件为Src_List

