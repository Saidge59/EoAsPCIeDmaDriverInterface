# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/denis/develop/EoAsPCIeDmaDriverInterface/user

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/denis/develop/EoAsPCIeDmaDriverInterface/user/build

# Include any dependencies generated for this target.
include CMakeFiles/driver_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/driver_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/driver_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/driver_test.dir/flags.make

CMakeFiles/driver_test.dir/src/main.cpp.o: CMakeFiles/driver_test.dir/flags.make
CMakeFiles/driver_test.dir/src/main.cpp.o: /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/main.cpp
CMakeFiles/driver_test.dir/src/main.cpp.o: CMakeFiles/driver_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/denis/develop/EoAsPCIeDmaDriverInterface/user/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/driver_test.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/driver_test.dir/src/main.cpp.o -MF CMakeFiles/driver_test.dir/src/main.cpp.o.d -o CMakeFiles/driver_test.dir/src/main.cpp.o -c /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/main.cpp

CMakeFiles/driver_test.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/driver_test.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/main.cpp > CMakeFiles/driver_test.dir/src/main.cpp.i

CMakeFiles/driver_test.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/driver_test.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/main.cpp -o CMakeFiles/driver_test.dir/src/main.cpp.s

CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o: CMakeFiles/driver_test.dir/flags.make
CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o: /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/DriverInterface.cpp
CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o: CMakeFiles/driver_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/denis/develop/EoAsPCIeDmaDriverInterface/user/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o -MF CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o.d -o CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o -c /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/DriverInterface.cpp

CMakeFiles/driver_test.dir/src/DriverInterface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/driver_test.dir/src/DriverInterface.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/DriverInterface.cpp > CMakeFiles/driver_test.dir/src/DriverInterface.cpp.i

CMakeFiles/driver_test.dir/src/DriverInterface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/driver_test.dir/src/DriverInterface.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/denis/develop/EoAsPCIeDmaDriverInterface/user/src/DriverInterface.cpp -o CMakeFiles/driver_test.dir/src/DriverInterface.cpp.s

# Object files for target driver_test
driver_test_OBJECTS = \
"CMakeFiles/driver_test.dir/src/main.cpp.o" \
"CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o"

# External object files for target driver_test
driver_test_EXTERNAL_OBJECTS =

driver_test: CMakeFiles/driver_test.dir/src/main.cpp.o
driver_test: CMakeFiles/driver_test.dir/src/DriverInterface.cpp.o
driver_test: CMakeFiles/driver_test.dir/build.make
driver_test: CMakeFiles/driver_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/denis/develop/EoAsPCIeDmaDriverInterface/user/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable driver_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/driver_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/driver_test.dir/build: driver_test
.PHONY : CMakeFiles/driver_test.dir/build

CMakeFiles/driver_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/driver_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/driver_test.dir/clean

CMakeFiles/driver_test.dir/depend:
	cd /home/denis/develop/EoAsPCIeDmaDriverInterface/user/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/denis/develop/EoAsPCIeDmaDriverInterface/user /home/denis/develop/EoAsPCIeDmaDriverInterface/user /home/denis/develop/EoAsPCIeDmaDriverInterface/user/build /home/denis/develop/EoAsPCIeDmaDriverInterface/user/build /home/denis/develop/EoAsPCIeDmaDriverInterface/user/build/CMakeFiles/driver_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/driver_test.dir/depend

