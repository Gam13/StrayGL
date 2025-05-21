cmake_minimum_required(VERSION 3.15)
project(StrayGL LANGUAGES C CXX)

#==================================================
# Configurações Básicas
#==================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definir diretórios de include globais
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3thparty/GLAD/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3thparty/GLFW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3thparty/SPDLOG/include
)

#==================================================
# Verificação do PCH
#==================================================
set(PCH_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/StrayGL/pch.h")
if(NOT EXISTS ${PCH_HEADER})
    message(FATAL_ERROR "Arquivo PCH não encontrado em: ${PCH_HEADER}")
else()
    message(STATUS "PCH encontrado em: ${PCH_HEADER}")
endif()

#==================================================
# Dependências Obrigatórias
#==================================================
add_subdirectory(3thparty/GLFW)
add_subdirectory(3thparty/SPDLOG)
set_target_properties(glfw PROPERTIES POSITION_INDEPENDENT_CODE ON)

#==================================================
# Biblioteca Principal (APENAS UMA DEFINIÇÃO)
#==================================================
set(C_SOURCES 3thparty/GLAD/src/glad.c)
set(CXX_SOURCES 
    src/app.cpp
    src/Renderer/OpenGLRenderer.cpp
    src/Renderer/RendererFactory.cpp
    src/Logger.cpp
)

# Definir a biblioteca principal UMA ÚNICA VEZ
add_library(StrayGL ${C_SOURCES} ${CXX_SOURCES})

# Configurar PCH para arquivos C++
if(MSVC)
    target_precompile_headers(StrayGL PRIVATE ${PCH_HEADER})
else()
    # Configuração para GCC/Clang
    set_source_files_properties(${CXX_SOURCES} PROPERTIES
        COMPILE_FLAGS "-include ${PCH_HEADER}"
    )
endif()

target_link_libraries(StrayGL PRIVATE glfw spdlog)

#==================================================
# ImGui (Somente em desenvolvimento)
#==================================================
if(STRAYGL_DEVELOPMENT)
    message(STATUS "Configurando ImGui para desenvolvimento")
    
    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3thparty/IMGUI)
    
    if(NOT EXISTS ${IMGUI_DIR}/imgui.h)
        message(FATAL_ERROR "Submodule ImGui não encontrado. Execute:\n  git submodule update --init --recursive")
    endif()
    
    # Configurar includes do ImGui explicitamente
    include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
    
    add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    target_link_libraries(imgui PRIVATE glfw)
    target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

    add_subdirectory(examples)
endif()

#==================================================
# Instalação (Somente em produção)
#==================================================
if(NOT STRAYGL_DEVELOPMENT)
    install(DIRECTORY include/StrayGL DESTINATION include)
    install(TARGETS StrayGL EXPORT StrayGLTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(EXPORT StrayGLTargets
        FILE StrayGLConfig.cmake
        NAMESPACE StrayGL::
        DESTINATION lib/cmake/StrayGL
    )
endif()