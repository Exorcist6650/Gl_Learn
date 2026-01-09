cmake_minimum_required(VERSION 4.2.1 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(PROJECT_NAME GL_learn)
project(${PROJECT_NAME})


add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Renderer/ShaderProgram.h
	src/Renderer/ShaderProgram.cpp
	src/Resources/ResourceManager.h
	src/Resources/ResourceManager.cpp
	src/Player/camera.h
	src/Player/camera.cpp
	src/Resources/stb_image.h)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm::glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)	

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

file(GLOB_RECURSE res_files "${CMAKE_SOURCE_DIR}/res/**")
target_sources(${PROJECT_NAME} PRIVATE ${res_files})
source_group(TREE "${CMAKE_SOURCE_DIR}/res" FILES ${res_files})