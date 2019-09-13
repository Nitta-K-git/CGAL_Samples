message("---glew setting---")

set( GLEW_DIR C:/glew/glew-2.1.0)
include_directories(${GLEW_DIR}/include)
message(${GLEW_DIR})
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} 
   ${GLEW_DIR}/lib/Release/x64/glew32s.lib
   ${GLEW_DIR}/lib/Release/x64/glew32.lib
   ${OPENGL_LIBRARIES}
)
