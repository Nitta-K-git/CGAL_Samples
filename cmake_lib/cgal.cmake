message("---CGAL setting---")
# set( CGALLIB_DIR C:/CGAL-4.13.1/include)
# # set( CGALLIB_DIR $ENV{CGALLIB_LIB})
# include_directories(${CGALLIB_DIR})
# message(${CGALLIB_DIR})

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package( CGAL REQUIRED COMPONENTS Core)

include(${CGAL_USE_FILE})

if (CGAL_FOUND)
#   add_definitions(-DQT_NO_KEYWORDS)
#   add_executable  ( min min.cpp  ${CGAL_Qt5_RESOURCE_FILES} ${CGAL_Qt5_MOC_FILES})
#   add_to_cached_list( CGAL_EXECUTABLE_TARGETS min )
#   target_link_libraries( min PRIVATE
#   CGAL::CGAL CGAL::CGAL_Qt5 Qt5::Gui )
#   include(${CGAL_MODULES_DIR}/CGAL_add_test.cmake)
#   cgal_add_compilation_test(min)
else()

  message(STATUS "NOTICE: This demo requires CGAL and Qt5, and will not be compiled.")

endif()


# message(${CGALLIB_DIR})
