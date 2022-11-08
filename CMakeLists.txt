# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Kalameister)

find_package(OpenGL REQUIRED)


add_subdirectory (external)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)


add_executable(opengl-test 
	opengl-test.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
	common/controls.cpp
	common/controls.hpp
)
target_link_libraries(opengl-test
	${ALL_LIBS}
)



add_custom_command(
   TARGET opengl-test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/opengl-test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/"
)


