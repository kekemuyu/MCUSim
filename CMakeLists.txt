#
# This file is part of MCUSim, an XSPICE library with microcontrollers.
#
# Copyright (C) 2017-2019 MCUSim Developers, see AUTHORS.txt for contributors.
#
# MCUSim is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# MCUSim is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Configuration file of the MCUSim build system.
#
# NOTE: Verbose output of the build process can be activated
# using 'make VERBOSE=1'.
cmake_minimum_required(VERSION 3.2)
project(MCUSim C)

set(MSIM_VERSION "0.2-current")
set(MCUSIM "mcusim")
set(MCUSIM_LIB_NAME "msim")
set(MCUSIM_LIB "lib${MCUSIM_LIB_NAME}")

add_definitions(-DMSIM_VERSION="${MSIM_VERSION}")

unset(build_date CACHE)
unset(build_time CACHE)
string(TIMESTAMP build_date "%Y-%m-%d" UTC)
string(TIMESTAMP build_time "%H:%M:%S" UTC)
add_definitions(-DMSIM_BUILD_DATE="${build_date}")
add_definitions(-DMSIM_BUILD_TIME="${build_time}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Filename.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Subdirlist.cmake)
include(CheckFunctionExists)
include(CheckIncludeFiles)

# -----------------------------------------------------------------------------
# Paths to install MCUSim to
# -----------------------------------------------------------------------------
if (NOT DEFINED MSIM_BIN_DIR)
	set(MSIM_BIN_DIR "bin")
endif()
if (NOT DEFINED MSIM_LIB_DIR)
	set(MSIM_LIB_DIR "lib")
endif()
if (NOT DEFINED MSIM_SLIB_DIR)
	set(MSIM_SLIB_DIR "lib")
endif()
if (NOT DEFINED MSIM_H_DIR)
	set(MSIM_H_DIR "include")
endif()
if (NOT DEFINED MSIM_CONF_DIR)
	set(MSIM_CONF_DIR "etc/mcusim")
endif()
if (NOT DEFINED MSIM_SCRIPT_DIR)
	set(MSIM_SCRIPT_DIR "share/mcusim/scripts")
endif()
if (NOT DEFINED MSIM_EXAMPLE_DIR)
	set(MSIM_EXAMPLE_DIR "share/mcusim/examples")
endif()

string(ASCII 27 Esc)
set(CRst "${Esc}[0m")		# Reset color
set(CBld "${Esc}[1m")		# Bold color

message(STATUS "Install to: \t\t${CMAKE_INSTALL_PREFIX}")
message(STATUS "    binaries: \t${CMAKE_INSTALL_PREFIX}/${MSIM_BIN_DIR}")
message(STATUS "    libs (shared): \t${CMAKE_INSTALL_PREFIX}/${MSIM_LIB_DIR}")
message(STATUS "    libs (static): \t${CMAKE_INSTALL_PREFIX}/${MSIM_SLIB_DIR}")
message(STATUS "    headers: \t${CMAKE_INSTALL_PREFIX}/${MSIM_H_DIR}")
message(STATUS "    config: \t\t${CMAKE_INSTALL_PREFIX}/${MSIM_CONF_DIR}")
message(STATUS "    scripts: \t${CMAKE_INSTALL_PREFIX}/${MSIM_SCRIPT_DIR}")
message(STATUS "    examples: \t${CMAKE_INSTALL_PREFIX}/${MSIM_EXAMPLE_DIR}")

# -----------------------------------------------------------------------------
# Set flags here
# -----------------------------------------------------------------------------
if (CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Building ${CBld}DEBUG${CRst} version: ${CBld}${MSIM_VERSION}${CRst}")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
	if (CMAKE_COMPILER_IS_GNUCC OR
			${CMAKE_C_COMPILER_ID} MATCHES "Clang")
		add_definitions("-g")

		add_definitions("-std=iso9899:1999")
		add_definitions("-Wall")
		add_definitions("-pedantic")
		add_definitions("-Wshadow")
		add_definitions("-Wpointer-arith")
		add_definitions("-Wcast-qual")
		add_definitions("-Wcast-align")
		add_definitions("-Wstrict-prototypes")
		add_definitions("-Wmissing-prototypes")
		add_definitions("-Wconversion")
		add_definitions("-Wsign-compare")
		add_definitions("-Wmissing-declarations")
		add_definitions("-Wnested-externs")
		add_definitions("-Wbad-function-cast")
		add_definitions("-Wold-style-definition")
		add_definitions("-Wunused")
		add_definitions("-Wuninitialized")
		add_definitions("-Wmissing-noreturn")
		add_definitions("-Wmissing-format-attribute")
		add_definitions("-Wredundant-decls")
		add_definitions("-Werror=implicit")
		add_definitions("-Werror=nonnull")
		add_definitions("-Werror=init-self")
		add_definitions("-Werror=main")
		add_definitions("-Werror=missing-braces")
		add_definitions("-Werror=sequence-point")
		add_definitions("-Werror=return-type")
		add_definitions("-Werror=trigraphs")
		add_definitions("-Werror=array-bounds")
		add_definitions("-Werror=write-strings")
		add_definitions("-Werror=address")
		add_definitions("-Werror=int-to-pointer-cast")
		add_definitions("-Werror=pointer-to-int-cast")
	endif()
else()
	message(STATUS "Building ${CBld}RELEASE${CRst} version: ${CBld}${MSIM_VERSION}${CRst}")

	if (CMAKE_COMPILER_IS_GNUCC OR
			${CMAKE_C_COMPILER_ID} MATCHES "Clang")
		add_definitions("-O3")

		add_definitions("-std=iso9899:1999")
		add_definitions("-Wall")
		add_definitions("-pedantic")
		add_definitions("-Wshadow")
		add_definitions("-Wpointer-arith")
		add_definitions("-Wcast-qual")
		add_definitions("-Wcast-align")
		add_definitions("-Wstrict-prototypes")
		add_definitions("-Wmissing-prototypes")
		add_definitions("-Wconversion")
		add_definitions("-Wsign-compare")
		add_definitions("-Wmissing-declarations")
		add_definitions("-Wnested-externs")
		add_definitions("-Wbad-function-cast")
		add_definitions("-Wold-style-definition")
		add_definitions("-Wunused")
		add_definitions("-Wuninitialized")
		add_definitions("-Wmissing-noreturn")
		add_definitions("-Wmissing-format-attribute")
		add_definitions("-Wredundant-decls")
		add_definitions("-Werror=implicit")
		add_definitions("-Werror=nonnull")
		add_definitions("-Werror=init-self")
		add_definitions("-Werror=main")
		add_definitions("-Werror=missing-braces")
		add_definitions("-Werror=sequence-point")
		add_definitions("-Werror=return-type")
		add_definitions("-Werror=trigraphs")
		add_definitions("-Werror=array-bounds")
		add_definitions("-Werror=write-strings")
		add_definitions("-Werror=address")
		add_definitions("-Werror=int-to-pointer-cast")
		add_definitions("-Werror=pointer-to-int-cast")
	endif()
endif()

# Compile with address sanitizer
if (WITH_ASAN)
	add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lasan")
	endif()
endif()

# -----------------------------------------------------------------------------
# Set sources here
# -----------------------------------------------------------------------------
set(MCUSIM_LIB_SOURCES
	src/avr/avr_simcore.c
	src/avr/avr_m8a.c
	src/avr/avr_m328p.c
	src/avr/avr_m328.c
	src/avr/avr_lua.c
	src/avr/avr_luaapi.c
	src/avr/avr_decoder.c
	src/avr/avr_gdb.c
	src/avr/avr_vcd.c
	src/avr/avr_timer.c
	src/avr/avr_wdt.c
	src/avr/avr_io.c
	src/msim_config.c
	src/msim_getopt.c
	src/msim_ihex.c
	src/msim_log.c
	src/msim_pty.c
	src/msim_tsq.c
)

# -----------------------------------------------------------------------------
# Look for libraries, packages, etc.
# -----------------------------------------------------------------------------
set(TARGET_LIBS "")

# Check pkg-config installed
find_package(PkgConfig REQUIRED)

# Check LuaJIT or Lua installed
unset(LUA_FOUND CACHE)
unset(LUA_INCLUDE_DIR CACHE)
unset(LUA_LIBRARY CACHE)

if (WITH_LUA)
	set(LUA_TYPE "Lua")
	find_package(Lua REQUIRED)
else()
	set(LUA_TYPE "LuaJIT")
	pkg_search_module(LUA REQUIRED luajit)
endif()

if (LUA_FOUND)
	message(STATUS "lua cflags: ${LUA_CFLAGS}")
	message(STATUS "lua ldflags: ${LUA_LDFLAGS}")
	set(TARGET_LIBS ${TARGET_LIBS} ${LUA_LDFLAGS})
endif()

# Check POSIX threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
if (CMAKE_USE_PTHREADS_INIT)
	set(TARGET_LIBS ${TARGET_LIBS} Threads::Threads)
endif()

# Check math library
if (NOT MSVC)
	check_function_exists(fmax RESULT)
	if (NOT RESULT)
		unset(RESULT)
		list(APPEND TARGET_LIBS -lm)
	endif()
endif()

# Check basic POSIX headers.
check_include_files(netdb.h HAVE_NETDB_H)
check_include_files("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(fcntl.h HAVE_FCNTL_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(errno.h HAVE_ERRNO_H)
check_include_files(poll.h HAVE_POLL_H)
check_include_files(netinet/in.h HAVE_NETINET_IN_H)
check_include_files(signal.h HAVE_SIGNAL_H)
if (NOT CYGWIN)
	check_include_files(netinet/tcp.h HAVE_NETINET_TCP_H)
	if (HAVE_NETDB_H AND HAVE_SYS_SOCKET_H AND HAVE_STRING_H AND
	    HAVE_FCNTL_H AND HAVE_UNISTD_H AND HAVE_ERRNO_H AND HAVE_POLL_H AND
	    HAVE_NETINET_IN_H AND HAVE_NETINET_TCP_H AND HAVE_SIGNAL_H AND
	    CMAKE_USE_PTHREADS_INIT)
		add_definitions(-DWITH_POSIX=1)
	else()
		message(STATUS "WITH_POSIX undefined!")
	endif()
else()
	# CMake cannot find netinet/tcp.h under CYGWIN and it's not
	# required. Let it be ignored silently.
	if (HAVE_NETDB_H AND HAVE_SYS_SOCKET_H AND HAVE_STRING_H AND
	    HAVE_FCNTL_H AND HAVE_UNISTD_H AND HAVE_ERRNO_H AND HAVE_POLL_H AND
	    HAVE_NETINET_IN_H AND CMAKE_USE_PTHREADS_INIT)
		add_definitions(-DWITH_POSIX=1)
		add_definitions(-DWITH_POSIX_CYGWIN=1)
	else()
		message(STATUS "WITH_POSIX undefined!")
		message(STATUS "WITH_POSIX_CYGWIN undefined!")
	endif()
endif()

# Check whether we will be able to use pseudo-terminals or not.
check_function_exists(posix_openpt HAVE_POSIX_OPENPT)
check_function_exists(grantpt HAVE_POSIX_GRANTPT)
check_function_exists(ptsname HAVE_POSIX_PTSNAME)
check_function_exists(unlockpt HAVE_POSIX_UNLOCKPT)
check_function_exists(open HAVE_POSIX_OPEN)
check_function_exists(close HAVE_POSIX_CLOSE)
check_function_exists(read HAVE_POSIX_READ)
check_function_exists(write HAVE_POSIX_WRITE)
if (HAVE_POSIX_OPENPT AND HAVE_POSIX_GRANTPT AND HAVE_POSIX_PTSNAME AND
    HAVE_POSIX_UNLOCKPT AND HAVE_POSIX_OPEN AND HAVE_POSIX_CLOSE AND
    HAVE_POSIX_READ AND HAVE_POSIX_WRITE)
	add_definitions(-DWITH_POSIX_PTY=1)
else()
	message(STATUS "WITH_POSIX_PTY undefined!")
endif()

# -----------------------------------------------------------------------------
# Parts of the project
# -----------------------------------------------------------------------------
add_subdirectory(scripts)	# Scripts and lua models
add_subdirectory(examples)	# Example circuits
add_subdirectory(tests)		# Simulation tests
add_subdirectory(misra)		# Configuration to check MISRA C rules
add_subdirectory(xspice)	# Compile MCUSim as XSPICE library

# -----------------------------------------------------------------------------
# Prepare headers
# -----------------------------------------------------------------------------
file(GLOB_RECURSE RAW_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}
	"${CMAKE_SOURCE_DIR}/include/*.h")
foreach(HEADER ${RAW_HEADERS})
	configure_file(${CMAKE_SOURCE_DIR}/${HEADER}
	               ${CMAKE_BINARY_DIR}/${HEADER} @ONLY)
endforeach()

# -----------------------------------------------------------------------------
# Define include files and directories here
# -----------------------------------------------------------------------------
include_directories("${CMAKE_BINARY_DIR}/include/")
if (LUA_FOUND)
	add_definitions(${LUA_CFLAGS})
endif()

# -----------------------------------------------------------------------------
# Compile MCUSim
# -----------------------------------------------------------------------------
add_library(objlib OBJECT ${MCUSIM_LIB_SOURCES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(${MCUSIM_LIB} SHARED $<TARGET_OBJECTS:objlib>)
add_library("${MCUSIM_LIB}-static" STATIC $<TARGET_OBJECTS:objlib>)
add_executable(${MCUSIM} src/msim_main.c)
set_target_properties(${MCUSIM_LIB} PROPERTIES OUTPUT_NAME ${MCUSIM_LIB_NAME})
set_target_properties("${MCUSIM_LIB}-static" PROPERTIES OUTPUT_NAME ${MCUSIM_LIB_NAME})

define_filename_for_sources(objlib)
define_filename_for_sources(${MCUSIM_LIB})
define_filename_for_sources("${MCUSIM_LIB}-static")
define_filename_for_sources(${MCUSIM})

# -----------------------------------------------------------------------------
# Link MCUSim
# -----------------------------------------------------------------------------
target_link_libraries(${MCUSIM_LIB} ${TARGET_LIBS})
target_link_libraries("${MCUSIM_LIB}-static" ${TARGET_LIBS})
target_link_libraries(${MCUSIM} ${MCUSIM_LIB})
if (APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND LUA_TYPE MATCHES "LuaJIT")
	# Add LuaJIT-specific flags for 64-bit build on macOS
	message(STATUS "Linking MCUSim with LuaJIT-specific flags on macOS with 64-bit build")
	target_link_libraries(${MCUSIM} "-pagezero_size 10000")
	target_link_libraries(${MCUSIM} "-image_base 100000000")
endif()

# -----------------------------------------------------------------------------
# Install MCUSim executable, library and headers
# -----------------------------------------------------------------------------
install(TARGETS ${MCUSIM} ${MCUSIM_LIB} "${MCUSIM_LIB}-static"
	RUNTIME DESTINATION ${MSIM_BIN_DIR}
	LIBRARY DESTINATION ${MSIM_LIB_DIR}
	ARCHIVE DESTINATION ${MSIM_SLIB_DIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/mcusim
	DESTINATION ${MSIM_H_DIR}
        FILES_MATCHING PATTERN "*.h"
	PATTERN "*/private*" EXCLUDE)
