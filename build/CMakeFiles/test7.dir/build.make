# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/arthur/Documents/repos/sfq_workspace/sfq_verifier

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build

# Include any dependencies generated for this target.
include CMakeFiles/test7.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test7.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test7.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test7.dir/flags.make

CMakeFiles/test7.dir/codegen:
.PHONY : CMakeFiles/test7.dir/codegen

CMakeFiles/test7.dir/test/test7.cpp.o: CMakeFiles/test7.dir/flags.make
CMakeFiles/test7.dir/test/test7.cpp.o: /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/test/test7.cpp
CMakeFiles/test7.dir/test/test7.cpp.o: CMakeFiles/test7.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test7.dir/test/test7.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test7.dir/test/test7.cpp.o -MF CMakeFiles/test7.dir/test/test7.cpp.o.d -o CMakeFiles/test7.dir/test/test7.cpp.o -c /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/test/test7.cpp

CMakeFiles/test7.dir/test/test7.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test7.dir/test/test7.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/test/test7.cpp > CMakeFiles/test7.dir/test/test7.cpp.i

CMakeFiles/test7.dir/test/test7.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test7.dir/test/test7.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/test/test7.cpp -o CMakeFiles/test7.dir/test/test7.cpp.s

# Object files for target test7
test7_OBJECTS = \
"CMakeFiles/test7.dir/test/test7.cpp.o"

# External object files for target test7
test7_EXTERNAL_OBJECTS =

test7: CMakeFiles/test7.dir/test/test7.cpp.o
test7: CMakeFiles/test7.dir/build.make
test7: CMakeFiles/test7.dir/compiler_depend.ts
test7: src/lib/libTALib.a
test7: src/lib/gates/libGateLib.a
test7: CMakeFiles/test7.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test7"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test7.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test7.dir/build: test7
.PHONY : CMakeFiles/test7.dir/build

CMakeFiles/test7.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test7.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test7.dir/clean

CMakeFiles/test7.dir/depend:
	cd /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arthur/Documents/repos/sfq_workspace/sfq_verifier /home/arthur/Documents/repos/sfq_workspace/sfq_verifier /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build /home/arthur/Documents/repos/sfq_workspace/sfq_verifier/build/CMakeFiles/test7.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test7.dir/depend

