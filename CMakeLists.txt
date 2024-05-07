cmake_minimum_required(VERSION 3.11)
project(OpenGLTempl)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCE_FILES "src/*")

# Add glad
include_directories(lib/glad/include)
list(APPEND SOURCE_FILES "lib/glad/src/glad.c")

# Including GLFW
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/0.9.9.8
)

FetchContent_MakeAvailable(glm)

# # Include GLM
# add_subdirectory(lib/glm)
# include_directories(lib/glm/glm)
# 
# # Add IMGUI
# include_directories(lib/imgui)
# file(GLOB LIB_FILES "lib/imgui/*.cpp" "lib/imgui/backends/imgui_impl_opengl3.cpp" "lib/imgui/backends/imgui_impl_glfw.cpp")
# list(APPEND SOURCE_FILES ${LIB_FILES})
# 

# Include STB (My Image Loader)
include_directories(lib/stb)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# file(COPY src/shader DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Linking GLFW, GLM and OpenGL
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC assimp glfw glm::glm ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
