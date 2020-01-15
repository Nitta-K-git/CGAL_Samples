cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

message("---CGAL setting---")

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package(CGAL REQUIRED COMPONENTS Qt5)

set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)

if(CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

if ( CGAL_FOUND )
  include( ${CGAL_USE_FILE} )
  find_package(Eigen3 3.2.0) #(requires 3.2.0 or greater)
  if (EIGEN3_FOUND)
	include( ${EIGEN3_USE_FILE} )
	message("found eigen3")
  endif(EIGEN3_FOUND)
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
endif()

set(SOURCE_FILES
   main.cpp
)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# Add Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}) # https://stackoverflow.com/questions/28893450/how-to-get-debug-postfix-in-executable-name/29023980

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
#    Qt5::Widgets
#    Qt5::OpenGL
)

