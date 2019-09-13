cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

include(../cmake_lib/cgal_qt.cmake)
# include(../cmake_lib/Qt.cmake)
include(../cmake_lib/vs.cmake)
# include(../cmake_lib/OpenGL.cmake)
include(../cmake_lib/general.cmake)

# include_directories(${HEADER_PATH})

set(SOURCE_FILES
   main.cpp
   # widget.h
   # widget.cpp
#   ${VCGLIB_DIR}/wrap/gui/trackball.h
)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# Add Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}) # https://stackoverflow.com/questions/28893450/how-to-get-debug-postfix-in-executable-name/29023980

target_link_libraries(${PROJECT_NAME} 
   CGAL::CGAL
   CGAL::CGAL_Qt5
#    Qt5::Widgets
#    Qt5::OpenGL
#    ${OPENGL_LIBRARIES}
#    # debug ${VCGLIB_DIR}/lib/plylibd.lib
#    # optimized ${VCGLIB_DIR}/lib/plylib.lib
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
