cmake_minimum_required(VERSION 3.16)

project(3thparty LANGUAGES C CXX)

# Opção para construir GLFW
option(BUILD_GLFW "Build GLFW from source" ON)

# Configurações de warnings
if(MSVC)
    set(THIRDPARTY_COMPILE_OPTIONS /W3)
else()
    set(THIRDPARTY_COMPILE_OPTIONS -Wall -Wextra)
endif()

## GLFW ##
if(BUILD_GLFW)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
    
    add_subdirectory(GLFW)
    
    add_library(glfw INTERFACE IMPORTED)
    target_link_libraries(glfw INTERFACE glfw)
    target_include_directories(glfw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include)
endif()

## GLAD ##
add_library(glad STATIC
    GLAD/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include
    ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include/KHR
)

target_compile_options(glad PRIVATE ${THIRDPARTY_COMPILE_OPTIONS})

## ImGui ##
set(IMGUI_SOURCES
    IMGUI/imgui.cpp
    IMGUI/imgui_draw.cpp
    IMGUI/imgui_tables.cpp
    IMGUI/imgui_widgets.cpp
    IMGUI/backends/imgui_impl_glfw.cpp
    IMGUI/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
    IMGUI/imconfig.h
    IMGUI/imgui.h
    IMGUI/imgui_internal.h
    IMGUI/imstb_rectpack.h
    IMGUI/imstb_textedit.h
    IMGUI/imstb_truetype.h
    IMGUI/backends/imgui_impl_glfw.h
    IMGUI/backends/imgui_impl_opengl3.h
)

add_library(imgui STATIC 
    ${IMGUI_SOURCES}
    ${IMGUI_HEADERS}
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/IMGUI
    ${CMAKE_CURRENT_SOURCE_DIR}/IMGUI/backends
)

target_compile_options(imgui PRIVATE ${THIRDPARTY_COMPILE_OPTIONS})
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui PRIVATE glfw glad)

find_package(OpenGL REQUIRED)
target_link_libraries(glad PRIVATE OpenGL::OpenGL)
target_link_libraries(imgui PRIVATE OpenGL::OpenGL)

# Aliases
add_library(3thparty::glfw ALIAS glfw)
add_library(3thparty::glad ALIAS glad)
add_library(3thparty::imgui ALIAS imgui)

# Export includes
target_include_directories(glad PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/IMGUI>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/IMGUI/backends>
    $<INSTALL_INTERFACE:include>
)