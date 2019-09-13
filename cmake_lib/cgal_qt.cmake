message("---CGAL setting---")

set( CGALLIB_DIR C:/CGAL-4.13.1)
find_package(CGAL REQUIRED COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

if ( CGAL_FOUND )
  include( ${CGAL_USE_FILE} )
  # include( CGAL_CreateSingleSourceCGALProgram )
  # include_directories (BEFORE "../../include")

  # if(CGAL_Qt5_FOUND )
  #   target_link_libraries(draw_surface_mesh PUBLIC CGAL::CGAL_Qt5)
  # endif()
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
endif()
