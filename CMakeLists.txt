cmake_minimum_required(VERSION 3.0)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCXXCompilerFlag)
include(FindPkgConfig)

project(tmln)

# Check for C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# FLTK FLTK_SKIP_OPENGL FLTK_SKIP_FORMS FLTK_SKIP_IMAGES FLTK_SKIP_FLUID)
find_package(FLTK)
if (FLTK_FOUND)
	set(HAVE_FLTK 1)
endif (FLTK_FOUND)
pkg_check_modules(CAIRO cairo)
if (CAIRO_FOUND)
	set(HAVE_CAIRO 1)
endif (CAIRO_FOUND)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

configure_file(${CMAKE_SOURCE_DIR}/CMake/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_subdirectory(test)
endif()
