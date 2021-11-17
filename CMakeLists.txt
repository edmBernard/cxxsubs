cmake_minimum_required(VERSION 3.21)


#**************************************************************************************************
# Project *****************************************************************************************
project(cxxsubs)

#**************************************************************************************************
# Find Package ************************************************************************************


#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cxxopts)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

#**************************************************************************************************
# Set compiler ************************************************************************************
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_options(/Oy /utf-8 /Zc:__cplusplus /DUNICODE /D_UNICODE)
else()
  add_compile_options(-fPIC -O3)
endif()

#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(example src/example.cpp)
add_executable(quickstart src/quickstart.cpp)

#**************************************************************************************************
# Custom Command for doc generation ***************************************************************
add_custom_command(
  OUTPUT html/index.html
  COMMAND doxygen config_file
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
)

add_custom_target(
  docs
  DEPENDS html/index.html
)

#**************************************************************************************************
# Test configuration ******************************************************************************
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
if(BUILD_UNIT_TESTS)
  enable_testing()
  SET(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
  )
  add_executable(unit_test ${TEST_SOURCES} ${SOURCES_HM})
  target_link_libraries(unit_test ${CPR_LIBRARIES})
  add_test(NAME "unit_test_default"
    COMMAND unit_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endif()
