# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\qp\qtools\bin\cmake.exe

# The command to remove a file.
RM = C:\qp\qtools\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\code\plecs\github\plecs\notes\PLECS43_dll\p44

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build

# Include any dependencies generated for this target.
include CMakeFiles/plecs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/plecs.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/plecs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/plecs.dir/flags.make

CMakeFiles/plecs.dir/dllmain.c.obj: CMakeFiles/plecs.dir/flags.make
CMakeFiles/plecs.dir/dllmain.c.obj: CMakeFiles/plecs.dir/includes_C.rsp
CMakeFiles/plecs.dir/dllmain.c.obj: D:/code/plecs/github/plecs/notes/PLECS43_dll/p44/dllmain.c
CMakeFiles/plecs.dir/dllmain.c.obj: CMakeFiles/plecs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/plecs.dir/dllmain.c.obj"
	C:\app\c_cpp\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/plecs.dir/dllmain.c.obj -MF CMakeFiles\plecs.dir\dllmain.c.obj.d -o CMakeFiles\plecs.dir\dllmain.c.obj -c D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\dllmain.c

CMakeFiles/plecs.dir/dllmain.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/plecs.dir/dllmain.c.i"
	C:\app\c_cpp\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\dllmain.c > CMakeFiles\plecs.dir\dllmain.c.i

CMakeFiles/plecs.dir/dllmain.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/plecs.dir/dllmain.c.s"
	C:\app\c_cpp\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\dllmain.c -o CMakeFiles\plecs.dir\dllmain.c.s

# Object files for target plecs
plecs_OBJECTS = \
"CMakeFiles/plecs.dir/dllmain.c.obj"

# External object files for target plecs
plecs_EXTERNAL_OBJECTS =

libplecs.dll: CMakeFiles/plecs.dir/dllmain.c.obj
libplecs.dll: CMakeFiles/plecs.dir/build.make
libplecs.dll: CMakeFiles/plecs.dir/linkLibs.rsp
libplecs.dll: CMakeFiles/plecs.dir/objects1.rsp
libplecs.dll: CMakeFiles/plecs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libplecs.dll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\plecs.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/plecs.dir/build: libplecs.dll
.PHONY : CMakeFiles/plecs.dir/build

CMakeFiles/plecs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\plecs.dir\cmake_clean.cmake
.PHONY : CMakeFiles/plecs.dir/clean

CMakeFiles/plecs.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\code\plecs\github\plecs\notes\PLECS43_dll\p44 D:\code\plecs\github\plecs\notes\PLECS43_dll\p44 D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build D:\code\plecs\github\plecs\notes\PLECS43_dll\p44\build\CMakeFiles\plecs.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/plecs.dir/depend

