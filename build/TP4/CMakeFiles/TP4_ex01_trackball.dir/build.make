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
CMAKE_BINARY_DIR = "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build"

# Include any dependencies generated for this target.
include TP4/CMakeFiles/TP4_ex01_trackball.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include TP4/CMakeFiles/TP4_ex01_trackball.dir/compiler_depend.make

# Include the progress variables for this target.
include TP4/CMakeFiles/TP4_ex01_trackball.dir/progress.make

# Include the compile flags for this target's objects.
include TP4/CMakeFiles/TP4_ex01_trackball.dir/flags.make

TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o: TP4/CMakeFiles/TP4_ex01_trackball.dir/flags.make
TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o: /home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/TP4/ex01_trackball.cpp
TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o: TP4/CMakeFiles/TP4_ex01_trackball.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o -MF CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o.d -o CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o -c "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP4/ex01_trackball.cpp"

TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.i"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP4/ex01_trackball.cpp" > CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.i

TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.s"
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP4/ex01_trackball.cpp" -o CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.s

# Object files for target TP4_ex01_trackball
TP4_ex01_trackball_OBJECTS = \
"CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o"

# External object files for target TP4_ex01_trackball
TP4_ex01_trackball_EXTERNAL_OBJECTS =

/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: TP4/CMakeFiles/TP4_ex01_trackball.dir/ex01_trackball.cpp.o
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: TP4/CMakeFiles/TP4_ex01_trackball.dir/build.make
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: glimac/libglimac.a
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: glimac/third-party/glfw/src/libglfw3.a
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: /usr/lib/x86_64-linux-gnu/librt.a
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: /usr/lib/x86_64-linux-gnu/libm.so
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: glimac/libglad.a
/home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball: TP4/CMakeFiles/TP4_ex01_trackball.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable \"/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/bin/TP4_ex01_trackball\""
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TP4_ex01_trackball.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
TP4/CMakeFiles/TP4_ex01_trackball.dir/build: /home/aertynb/Documents/M2/Synthese\ Image/GL_UGE_Template/bin/TP4_ex01_trackball
.PHONY : TP4/CMakeFiles/TP4_ex01_trackball.dir/build

TP4/CMakeFiles/TP4_ex01_trackball.dir/clean:
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" && $(CMAKE_COMMAND) -P CMakeFiles/TP4_ex01_trackball.dir/cmake_clean.cmake
.PHONY : TP4/CMakeFiles/TP4_ex01_trackball.dir/clean

TP4/CMakeFiles/TP4_ex01_trackball.dir/depend:
	cd "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/TP4" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4" "/home/aertynb/Documents/M2/Synthese Image/GL_UGE_Template/build/TP4/CMakeFiles/TP4_ex01_trackball.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : TP4/CMakeFiles/TP4_ex01_trackball.dir/depend

