# Verifica se estamos no modo desenvolvimento
if(NOT STRAYGL_DEVELOPMENT)
    message(STATUS "Exemplos desativados (modo produção)")
    return()
endif()

# Verifica dependências necessárias
if(NOT TARGET imgui)
    message(FATAL_ERROR "ImGui necessário para exemplos. Configure com -DSTRAYGL_DEVELOPMENT=ON")
endif()

# Verifica se o arquivo de exemplo existe
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/basic/basicExample.cpp)
    message(FATAL_ERROR "Arquivo do exemplo não encontrado: ${CMAKE_CURRENT_SOURCE_DIR}/basic/basicExample.cpp")
endif()

# Define o diretório de build dos exemplos
set(EXAMPLES_BUILD_DIR ${CMAKE_BINARY_DIR}/examples)

# Cria o executável do exemplo
add_executable(StrayGL_BasicExample
    basic/basicExample.cpp
)

# Linka com a biblioteca principal e o ImGui
target_link_libraries(StrayGL_BasicExample PRIVATE
    StrayGL        
    imgui
)

# Define o diretório de saída dos binários
set_target_properties(StrayGL_BasicExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_BUILD_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXAMPLES_BUILD_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXAMPLES_BUILD_DIR}
)

# Configura para que os builds sejam colocados na pasta especificada
if(NOT EXISTS ${EXAMPLES_BUILD_DIR})
    file(MAKE_DIRECTORY ${EXAMPLES_BUILD_DIR})
endif()
