# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/alan/ha/lvglsim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alan/ha/lvglsim/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/mouse_cursor_icon.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/mouse_cursor_icon.c.o: /home/alan/ha/lvglsim/mouse_cursor_icon.c
CMakeFiles/main.dir/mouse_cursor_icon.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.dir/mouse_cursor_icon.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/mouse_cursor_icon.c.o -MF CMakeFiles/main.dir/mouse_cursor_icon.c.o.d -o CMakeFiles/main.dir/mouse_cursor_icon.c.o -c /home/alan/ha/lvglsim/mouse_cursor_icon.c

CMakeFiles/main.dir/mouse_cursor_icon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/mouse_cursor_icon.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/mouse_cursor_icon.c > CMakeFiles/main.dir/mouse_cursor_icon.c.i

CMakeFiles/main.dir/mouse_cursor_icon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/mouse_cursor_icon.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/mouse_cursor_icon.c -o CMakeFiles/main.dir/mouse_cursor_icon.c.s

CMakeFiles/main.dir/ui.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ui.c.o: /home/alan/ha/lvglsim/ui.c
CMakeFiles/main.dir/ui.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/main.dir/ui.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/ui.c.o -MF CMakeFiles/main.dir/ui.c.o.d -o CMakeFiles/main.dir/ui.c.o -c /home/alan/ha/lvglsim/ui.c

CMakeFiles/main.dir/ui.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/ui.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/ui.c > CMakeFiles/main.dir/ui.c.i

CMakeFiles/main.dir/ui.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/ui.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/ui.c -o CMakeFiles/main.dir/ui.c.s

CMakeFiles/main.dir/cog.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/cog.c.o: /home/alan/ha/lvglsim/cog.c
CMakeFiles/main.dir/cog.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/main.dir/cog.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/cog.c.o -MF CMakeFiles/main.dir/cog.c.o.d -o CMakeFiles/main.dir/cog.c.o -c /home/alan/ha/lvglsim/cog.c

CMakeFiles/main.dir/cog.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/cog.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/cog.c > CMakeFiles/main.dir/cog.c.i

CMakeFiles/main.dir/cog.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/cog.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/cog.c -o CMakeFiles/main.dir/cog.c.s

CMakeFiles/main.dir/invisible.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/invisible.c.o: /home/alan/ha/lvglsim/invisible.c
CMakeFiles/main.dir/invisible.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/main.dir/invisible.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/invisible.c.o -MF CMakeFiles/main.dir/invisible.c.o.d -o CMakeFiles/main.dir/invisible.c.o -c /home/alan/ha/lvglsim/invisible.c

CMakeFiles/main.dir/invisible.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/invisible.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/invisible.c > CMakeFiles/main.dir/invisible.c.i

CMakeFiles/main.dir/invisible.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/invisible.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/invisible.c -o CMakeFiles/main.dir/invisible.c.s

CMakeFiles/main.dir/visible.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/visible.c.o: /home/alan/ha/lvglsim/visible.c
CMakeFiles/main.dir/visible.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/main.dir/visible.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/visible.c.o -MF CMakeFiles/main.dir/visible.c.o.d -o CMakeFiles/main.dir/visible.c.o -c /home/alan/ha/lvglsim/visible.c

CMakeFiles/main.dir/visible.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/visible.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/visible.c > CMakeFiles/main.dir/visible.c.i

CMakeFiles/main.dir/visible.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/visible.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/visible.c -o CMakeFiles/main.dir/visible.c.s

CMakeFiles/main.dir/LoraBold240.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/LoraBold240.c.o: /home/alan/ha/lvglsim/LoraBold240.c
CMakeFiles/main.dir/LoraBold240.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/main.dir/LoraBold240.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/LoraBold240.c.o -MF CMakeFiles/main.dir/LoraBold240.c.o.d -o CMakeFiles/main.dir/LoraBold240.c.o -c /home/alan/ha/lvglsim/LoraBold240.c

CMakeFiles/main.dir/LoraBold240.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/LoraBold240.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/LoraBold240.c > CMakeFiles/main.dir/LoraBold240.c.i

CMakeFiles/main.dir/LoraBold240.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/LoraBold240.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/LoraBold240.c -o CMakeFiles/main.dir/LoraBold240.c.s

CMakeFiles/main.dir/LoraBold40.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/LoraBold40.c.o: /home/alan/ha/lvglsim/LoraBold40.c
CMakeFiles/main.dir/LoraBold40.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/main.dir/LoraBold40.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/LoraBold40.c.o -MF CMakeFiles/main.dir/LoraBold40.c.o.d -o CMakeFiles/main.dir/LoraBold40.c.o -c /home/alan/ha/lvglsim/LoraBold40.c

CMakeFiles/main.dir/LoraBold40.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/LoraBold40.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/LoraBold40.c > CMakeFiles/main.dir/LoraBold40.c.i

CMakeFiles/main.dir/LoraBold40.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/LoraBold40.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/LoraBold40.c -o CMakeFiles/main.dir/LoraBold40.c.s

CMakeFiles/main.dir/LoraBold64.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/LoraBold64.c.o: /home/alan/ha/lvglsim/LoraBold64.c
CMakeFiles/main.dir/LoraBold64.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/main.dir/LoraBold64.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/LoraBold64.c.o -MF CMakeFiles/main.dir/LoraBold64.c.o.d -o CMakeFiles/main.dir/LoraBold64.c.o -c /home/alan/ha/lvglsim/LoraBold64.c

CMakeFiles/main.dir/LoraBold64.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/LoraBold64.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/LoraBold64.c > CMakeFiles/main.dir/LoraBold64.c.i

CMakeFiles/main.dir/LoraBold64.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/LoraBold64.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/LoraBold64.c -o CMakeFiles/main.dir/LoraBold64.c.s

CMakeFiles/main.dir/RobotoMedium40.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/RobotoMedium40.c.o: /home/alan/ha/lvglsim/RobotoMedium40.c
CMakeFiles/main.dir/RobotoMedium40.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/main.dir/RobotoMedium40.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/RobotoMedium40.c.o -MF CMakeFiles/main.dir/RobotoMedium40.c.o.d -o CMakeFiles/main.dir/RobotoMedium40.c.o -c /home/alan/ha/lvglsim/RobotoMedium40.c

CMakeFiles/main.dir/RobotoMedium40.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/RobotoMedium40.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alan/ha/lvglsim/RobotoMedium40.c > CMakeFiles/main.dir/RobotoMedium40.c.i

CMakeFiles/main.dir/RobotoMedium40.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/RobotoMedium40.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alan/ha/lvglsim/RobotoMedium40.c -o CMakeFiles/main.dir/RobotoMedium40.c.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/mouse_cursor_icon.c.o" \
"CMakeFiles/main.dir/ui.c.o" \
"CMakeFiles/main.dir/cog.c.o" \
"CMakeFiles/main.dir/invisible.c.o" \
"CMakeFiles/main.dir/visible.c.o" \
"CMakeFiles/main.dir/LoraBold240.c.o" \
"CMakeFiles/main.dir/LoraBold40.c.o" \
"CMakeFiles/main.dir/LoraBold64.c.o" \
"CMakeFiles/main.dir/RobotoMedium40.c.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/mouse_cursor_icon.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/ui.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/cog.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/invisible.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/visible.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/LoraBold240.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/LoraBold40.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/LoraBold64.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/RobotoMedium40.c.o
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/build.make
/home/alan/ha/lvglsim/bin/main: lib/liblvgl.a
/home/alan/ha/lvglsim/bin/main: lib/liblvgl_examples.a
/home/alan/ha/lvglsim/bin/main: lib/liblvgl_demos.a
/home/alan/ha/lvglsim/bin/main: lib/liblvgl_thorvg.a
/home/alan/ha/lvglsim/bin/main: lib/liblvgl.a
/home/alan/ha/lvglsim/bin/main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alan/ha/lvglsim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable /home/alan/ha/lvglsim/bin/main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: /home/alan/ha/lvglsim/bin/main
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/alan/ha/lvglsim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alan/ha/lvglsim /home/alan/ha/lvglsim /home/alan/ha/lvglsim/build /home/alan/ha/lvglsim/build /home/alan/ha/lvglsim/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

