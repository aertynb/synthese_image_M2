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
CMAKE_SOURCE_DIR = "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin"

# Include any dependencies generated for this target.
include TP3/CMakeFiles/TP3_ex03_moons.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include TP3/CMakeFiles/TP3_ex03_moons.dir/compiler_depend.make

# Include the progress variables for this target.
include TP3/CMakeFiles/TP3_ex03_moons.dir/progress.make

# Include the compile flags for this target's objects.
include TP3/CMakeFiles/TP3_ex03_moons.dir/flags.make

TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o: TP3/CMakeFiles/TP3_ex03_moons.dir/flags.make
TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o: /home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/TP3/ex03_moons.cpp
TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o: TP3/CMakeFiles/TP3_ex03_moons.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o -MF CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o.d -o CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o -c "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP3/ex03_moons.cpp"

TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.i"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP3/ex03_moons.cpp" > CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.i

TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.s"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP3/ex03_moons.cpp" -o CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.s

# Object files for target TP3_ex03_moons
TP3_ex03_moons_OBJECTS = \
"CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o"

# External object files for target TP3_ex03_moons
TP3_ex03_moons_EXTERNAL_OBJECTS =

TP3_ex03_moons: TP3/CMakeFiles/TP3_ex03_moons.dir/ex03_moons.cpp.o
TP3_ex03_moons: TP3/CMakeFiles/TP3_ex03_moons.dir/build.make
TP3_ex03_moons: glimac/libglimac.a
TP3_ex03_moons: glimac/third-party/glfw/src/libglfw3.a
TP3_ex03_moons: /usr/lib/x86_64-linux-gnu/librt.a
TP3_ex03_moons: /usr/lib/x86_64-linux-gnu/libm.so
TP3_ex03_moons: glimac/libglad.a
TP3_ex03_moons: TP3/CMakeFiles/TP3_ex03_moons.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../TP3_ex03_moons"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TP3_ex03_moons.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
TP3/CMakeFiles/TP3_ex03_moons.dir/build: TP3_ex03_moons
.PHONY : TP3/CMakeFiles/TP3_ex03_moons.dir/build

TP3/CMakeFiles/TP3_ex03_moons.dir/clean:
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" && $(CMAKE_COMMAND) -P CMakeFiles/TP3_ex03_moons.dir/cmake_clean.cmake
.PHONY : TP3/CMakeFiles/TP3_ex03_moons.dir/clean

TP3/CMakeFiles/TP3_ex03_moons.dir/depend:
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP3" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP3/CMakeFiles/TP3_ex03_moons.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : TP3/CMakeFiles/TP3_ex03_moons.dir/depend

