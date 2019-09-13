message("---Qt setting---")
# set(Qt5_DIR $ENV{QTDIR})
set(Qt5_DIR C:/Qt2/5.12.3/msvc2017_64)
list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)

# Set Automatic MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
   set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

message("if necessary link Qt5::Widgets, Qt5::OpenGL, ...")

# target_link_libraries(${PROJECT_NAME}
#    Qt5::Widgets
#    Qt5::OpenGL
# )
