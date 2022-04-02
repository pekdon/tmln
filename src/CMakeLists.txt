set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(common_INCLUDE_DIRS ${PROJECT_BINARY_DIR})
set(common_LIBRARIES "")

set(libtmln_SOURCES
	json11.cpp
	tmln_data.cc
	tmln_load_json.cc
	tmln_render.cc
	tmln_scale.cc
	tmln_selection.cc
	tmln_style.cc
	tmln_time.cc)

set(tmln_SOURCES
	main.cc)

if (HAVE_CAIRO)
	set(libtmln_SOURCES ${libtmln_SOURCES} tmln_draw_cairo.cc)
	set(common_INCLUDE_DIRS
		${common_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS})
	set(common_LIBRARIES
		${common_LIBRARIES} ${CAIRO_LDFLAGS})
endif (HAVE_CAIRO)

if (HAVE_FLTK)
	set(libtmln_SOURCES ${libtmln_SOURCES} tmln_fltk.cc)
	set(common_INCLUDE_DIRS
		${common_INCLUDE_DIRS} ${FLTK_INCLUDE_DIR})
	set(common_LIBRARIES
		${common_LIBRARIES} ${FLTK_LIBRARIES})
endif (HAVE_FLTK)

add_library(tmln STATIC ${libtmln_SOURCES})
target_include_directories(tmln PUBLIC ${common_INCLUDE_DIRS})
target_link_libraries(tmln ${common_LIBRARIES})
set_target_properties(tmln PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON)


add_executable(tmln-tool ${tmln_SOURCES})
target_include_directories(tmln-tool PUBLIC ${common_INCLUDE_DIRS})
target_link_libraries(tmln-tool tmln ${common_LIBRARIES})
set_target_properties(tmln-tool PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON)

install(TARGETS tmln DESTINATION lib)
install(TARGETS tmln-tool DESTINATION bin)
