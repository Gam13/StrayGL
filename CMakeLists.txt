cmake_minimum_required(VERSION 3.15)
project(StrayGL LANGUAGES C CXX)

#==================================================
# Configurações Básicas
#==================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#==================================================
# Modos de Operação
#==================================================
option(STRAYGL_DEVELOPMENT "Modo desenvolvimento (inclui exemplos e ImGui)" ON)
option(BUILD_SHARED_LIBS "Build como biblioteca compartilhada" ON)

#==================================================
# Dependências Obrigatórias (GLFW + GLAD)
#==================================================
# Configura GLFW
add_subdirectory(3thparty/GLFW)
set_target_properties(glfw PROPERTIES POSITION_INDEPENDENT_CODE ON)

#==================================================
# Biblioteca Principal
#==================================================
add_library(StrayGL
    src/app.cpp
    src/Renderer/OpenGLRenderer.cpp
    src/Renderer/RendererFactory.cpp
    3thparty/GLAD/src/glad.c  # GLAD incorporado
)

target_include_directories(StrayGL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3thparty/GLAD/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3thparty/GLFW/include> 
)

target_link_libraries(StrayGL PRIVATE glfw)

#==================================================
# ImGui (Somente em desenvolvimento)
#==================================================
if(STRAYGL_DEVELOPMENT)
    message(STATUS "Configurando ImGui para desenvolvimento")
    
    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3thparty/IMGUI)
    
    # Verifica se o submodule existe
    if(NOT EXISTS ${IMGUI_DIR}/imgui.h)
        message(FATAL_ERROR "Submodule ImGui não encontrado. Execute:\n  git submodule update --init --recursive")
    endif()
    
    # Configura ImGui manualmente
    add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
    )
    
    target_link_libraries(imgui PRIVATE glfw)
    target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

    #==================================================
    # Exemplos (Somente com ImGui)
    #==================================================
    add_subdirectory(examples)
else()
    message(STATUS "Modo produção: exemplos e ImGui desativados")
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
