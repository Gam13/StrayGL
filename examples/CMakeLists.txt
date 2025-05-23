# Verifica modo desenvolvimento
if(NOT STRAYGL_DEVELOPMENT)
    return()
endif()

message(STATUS "Configurando exemplos...")

# Verifica dependências
if(NOT TARGET imgui::imgui)
    message(FATAL_ERROR "ImGui necessário para exemplos")
endif()

# Cria executável do exemplo
add_executable(StrayGL_BasicExample basic/basicExample.cpp)

# Configura includes
target_include_directories(StrayGL_BasicExample PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Linka com a biblioteca principal e dependências
target_link_libraries(StrayGL_BasicExample PRIVATE
    StrayGL  # Link direto com o alvo da biblioteca
    imgui::imgui
)

# Configura diretório de saída
set(EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/examples)
set_target_properties(StrayGL_BasicExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}
)

if(NOT EXISTS ${EXAMPLES_OUTPUT_DIR})
    file(MAKE_DIRECTORY ${EXAMPLES_OUTPUT_DIR})
endif()