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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build

# Include any dependencies generated for this target.
include CMakeFiles/satellite_sim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/satellite_sim.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/satellite_sim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/satellite_sim.dir/flags.make

satellite_sim_autogen/timestamp: /Users/nathanblackburn/Qt/6.8.1/macos/libexec/moc
satellite_sim_autogen/timestamp: /Users/nathanblackburn/Qt/6.8.1/macos/libexec/uic
satellite_sim_autogen/timestamp: CMakeFiles/satellite_sim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target satellite_sim"
	/opt/homebrew/bin/cmake -E cmake_autogen /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles/satellite_sim_autogen.dir/AutogenInfo.json ""
	/opt/homebrew/bin/cmake -E touch /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/satellite_sim_autogen/timestamp

CMakeFiles/satellite_sim.dir/codegen:
.PHONY : CMakeFiles/satellite_sim.dir/codegen

CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o: CMakeFiles/satellite_sim.dir/flags.make
CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o: satellite_sim_autogen/mocs_compilation.cpp
CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o: CMakeFiles/satellite_sim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o -MF CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o -c /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/satellite_sim_autogen/mocs_compilation.cpp

CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/satellite_sim_autogen/mocs_compilation.cpp > CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.i

CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/satellite_sim_autogen/mocs_compilation.cpp -o CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.s

CMakeFiles/satellite_sim.dir/main.cpp.o: CMakeFiles/satellite_sim.dir/flags.make
CMakeFiles/satellite_sim.dir/main.cpp.o: /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/main.cpp
CMakeFiles/satellite_sim.dir/main.cpp.o: CMakeFiles/satellite_sim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/satellite_sim.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/satellite_sim.dir/main.cpp.o -MF CMakeFiles/satellite_sim.dir/main.cpp.o.d -o CMakeFiles/satellite_sim.dir/main.cpp.o -c /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/main.cpp

CMakeFiles/satellite_sim.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/satellite_sim.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/main.cpp > CMakeFiles/satellite_sim.dir/main.cpp.i

CMakeFiles/satellite_sim.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/satellite_sim.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/main.cpp -o CMakeFiles/satellite_sim.dir/main.cpp.s

# Object files for target satellite_sim
satellite_sim_OBJECTS = \
"CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/satellite_sim.dir/main.cpp.o"

# External object files for target satellite_sim
satellite_sim_EXTERNAL_OBJECTS =

satellite_sim.app/Contents/MacOS/satellite_sim: CMakeFiles/satellite_sim.dir/satellite_sim_autogen/mocs_compilation.cpp.o
satellite_sim.app/Contents/MacOS/satellite_sim: CMakeFiles/satellite_sim.dir/main.cpp.o
satellite_sim.app/Contents/MacOS/satellite_sim: CMakeFiles/satellite_sim.dir/build.make
satellite_sim.app/Contents/MacOS/satellite_sim: /Users/nathanblackburn/Qt/6.8.1/macos/lib/QtWidgets.framework/Versions/A/QtWidgets
satellite_sim.app/Contents/MacOS/satellite_sim: /Users/nathanblackburn/Qt/6.8.1/macos/lib/QtGui.framework/Versions/A/QtGui
satellite_sim.app/Contents/MacOS/satellite_sim: /Users/nathanblackburn/Qt/6.8.1/macos/lib/QtCore.framework/Versions/A/QtCore
satellite_sim.app/Contents/MacOS/satellite_sim: CMakeFiles/satellite_sim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable satellite_sim.app/Contents/MacOS/satellite_sim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/satellite_sim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/satellite_sim.dir/build: satellite_sim.app/Contents/MacOS/satellite_sim
.PHONY : CMakeFiles/satellite_sim.dir/build

CMakeFiles/satellite_sim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/satellite_sim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/satellite_sim.dir/clean

CMakeFiles/satellite_sim.dir/depend: satellite_sim_autogen/timestamp
	cd /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite_sim/build/CMakeFiles/satellite_sim.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/satellite_sim.dir/depend

