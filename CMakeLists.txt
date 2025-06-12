cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ArtDisplay)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find system packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Source files
set(SOURCES
    main.cpp
    src/glad.c
    src/cube.cpp
    src/ground.cpp
	src/skybox.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_impl_glfw.cpp
    external/imgui/imgui_impl_opengl3.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include               # For glad, stb_image.h, shaders.h, etc.
    ${CMAKE_SOURCE_DIR}/external/imgui
    /usr/include                              # fallback for GLM if not found
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    dl
    pthread
)

# GLM - try CMake find_package or fallback
find_package(glm QUIET)
if(glm_FOUND)
    target_link_libraries(${PROJECT_NAME} glm::glm)
    message(STATUS "GLM found via find_package.")
else()
    find_path(GLM_INCLUDE_DIR glm/glm.hpp)
    if(GLM_INCLUDE_DIR)
        target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR})
        message(STATUS "GLM not found via find_package, using manual include path.")
    else()
        message(FATAL_ERROR "GLM NOT FOUND")
    endif()
endif()
