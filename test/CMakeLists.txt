set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(common_INCLUDE_DIRS ${PROJECT_BINARY_DIR}
			../src)

set(test_SOURCES test.cc)

add_executable(test ${test_SOURCES})
target_include_directories(test PUBLIC ${common_INCLUDE_DIRS})
target_link_libraries(test tmln)
set_target_properties(test PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON)
