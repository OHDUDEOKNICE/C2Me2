# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kali/projects/C2Me2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kali/projects/C2Me2

# Include any dependencies generated for this target.
include src/common/CMakeFiles/common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/common/CMakeFiles/common.dir/compiler_depend.make

# Include the progress variables for this target.
include src/common/CMakeFiles/common.dir/progress.make

# Include the compile flags for this target's objects.
include src/common/CMakeFiles/common.dir/flags.make

src/common/CMakeFiles/common.dir/logging.cpp.o: src/common/CMakeFiles/common.dir/flags.make
src/common/CMakeFiles/common.dir/logging.cpp.o: src/common/logging.cpp
src/common/CMakeFiles/common.dir/logging.cpp.o: src/common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kali/projects/C2Me2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/common/CMakeFiles/common.dir/logging.cpp.o"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/common/CMakeFiles/common.dir/logging.cpp.o -MF CMakeFiles/common.dir/logging.cpp.o.d -o CMakeFiles/common.dir/logging.cpp.o -c /home/kali/projects/C2Me2/src/common/logging.cpp

src/common/CMakeFiles/common.dir/logging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/common.dir/logging.cpp.i"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kali/projects/C2Me2/src/common/logging.cpp > CMakeFiles/common.dir/logging.cpp.i

src/common/CMakeFiles/common.dir/logging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/common.dir/logging.cpp.s"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kali/projects/C2Me2/src/common/logging.cpp -o CMakeFiles/common.dir/logging.cpp.s

src/common/CMakeFiles/common.dir/config.cpp.o: src/common/CMakeFiles/common.dir/flags.make
src/common/CMakeFiles/common.dir/config.cpp.o: src/common/config.cpp
src/common/CMakeFiles/common.dir/config.cpp.o: src/common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kali/projects/C2Me2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/common/CMakeFiles/common.dir/config.cpp.o"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/common/CMakeFiles/common.dir/config.cpp.o -MF CMakeFiles/common.dir/config.cpp.o.d -o CMakeFiles/common.dir/config.cpp.o -c /home/kali/projects/C2Me2/src/common/config.cpp

src/common/CMakeFiles/common.dir/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/common.dir/config.cpp.i"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kali/projects/C2Me2/src/common/config.cpp > CMakeFiles/common.dir/config.cpp.i

src/common/CMakeFiles/common.dir/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/common.dir/config.cpp.s"
	cd /home/kali/projects/C2Me2/src/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kali/projects/C2Me2/src/common/config.cpp -o CMakeFiles/common.dir/config.cpp.s

# Object files for target common
common_OBJECTS = \
"CMakeFiles/common.dir/logging.cpp.o" \
"CMakeFiles/common.dir/config.cpp.o"

# External object files for target common
common_EXTERNAL_OBJECTS =

src/common/libcommon.a: src/common/CMakeFiles/common.dir/logging.cpp.o
src/common/libcommon.a: src/common/CMakeFiles/common.dir/config.cpp.o
src/common/libcommon.a: src/common/CMakeFiles/common.dir/build.make
src/common/libcommon.a: src/common/CMakeFiles/common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kali/projects/C2Me2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libcommon.a"
	cd /home/kali/projects/C2Me2/src/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean_target.cmake
	cd /home/kali/projects/C2Me2/src/common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/common/CMakeFiles/common.dir/build: src/common/libcommon.a
.PHONY : src/common/CMakeFiles/common.dir/build

src/common/CMakeFiles/common.dir/clean:
	cd /home/kali/projects/C2Me2/src/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean.cmake
.PHONY : src/common/CMakeFiles/common.dir/clean

src/common/CMakeFiles/common.dir/depend:
	cd /home/kali/projects/C2Me2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kali/projects/C2Me2 /home/kali/projects/C2Me2/src/common /home/kali/projects/C2Me2 /home/kali/projects/C2Me2/src/common /home/kali/projects/C2Me2/src/common/CMakeFiles/common.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/common/CMakeFiles/common.dir/depend

