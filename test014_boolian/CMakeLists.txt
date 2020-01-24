cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package(CGAL)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
)

