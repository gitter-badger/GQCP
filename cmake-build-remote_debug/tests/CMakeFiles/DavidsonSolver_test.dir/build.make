# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.bjdnmW1y5W

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug

# Include any dependencies generated for this target.
include tests/CMakeFiles/DavidsonSolver_test.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/DavidsonSolver_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/DavidsonSolver_test.dir/flags.make

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o: tests/CMakeFiles/DavidsonSolver_test.dir/flags.make
tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o: ../tests/math/optimization/DavidsonSolver_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o"
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o -c /tmp/tmp.bjdnmW1y5W/tests/math/optimization/DavidsonSolver_test.cpp

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.i"
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.bjdnmW1y5W/tests/math/optimization/DavidsonSolver_test.cpp > CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.i

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.s"
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.bjdnmW1y5W/tests/math/optimization/DavidsonSolver_test.cpp -o CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.s

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.requires:

.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.requires

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.provides: tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/DavidsonSolver_test.dir/build.make tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.provides.build
.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.provides

tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.provides.build: tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o


# Object files for target DavidsonSolver_test
DavidsonSolver_test_OBJECTS = \
"CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o"

# External object files for target DavidsonSolver_test
DavidsonSolver_test_EXTERNAL_OBJECTS =

tests/DavidsonSolver_test: tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o
tests/DavidsonSolver_test: tests/CMakeFiles/DavidsonSolver_test.dir/build.make
tests/DavidsonSolver_test: src/libgqcp.so
tests/DavidsonSolver_test: /home/tmhuysen/miniconda/lib/libboost_program_options.so
tests/DavidsonSolver_test: /home/tmhuysen/miniconda/lib/libint2.so
tests/DavidsonSolver_test: tests/CMakeFiles/DavidsonSolver_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DavidsonSolver_test"
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DavidsonSolver_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/DavidsonSolver_test.dir/build: tests/DavidsonSolver_test

.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/build

tests/CMakeFiles/DavidsonSolver_test.dir/requires: tests/CMakeFiles/DavidsonSolver_test.dir/math/optimization/DavidsonSolver_test.cpp.o.requires

.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/requires

tests/CMakeFiles/DavidsonSolver_test.dir/clean:
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/DavidsonSolver_test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/clean

tests/CMakeFiles/DavidsonSolver_test.dir/depend:
	cd /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.bjdnmW1y5W /tmp/tmp.bjdnmW1y5W/tests /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests /tmp/tmp.bjdnmW1y5W/cmake-build-remote_debug/tests/CMakeFiles/DavidsonSolver_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/DavidsonSolver_test.dir/depend

