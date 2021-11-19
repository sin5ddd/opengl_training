cmake_minimum_required(VERSION 3.20)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*)
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
		opengl32
		glfw3
		glew32
		)