cmake_minimum_required(VERSION 3.16)

# Configuração básica do projeto
project(StrayGL
    VERSION 1.0.0
    DESCRIPTION "Projeto usando GLFW e GLAD"
    LANGUAGES CXX
)

# Definir como projeto de biblioteca
set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0076 NEW)



# Opção para build estático/dinâmico
option(BUILD_SHARED_LIBS "Build as shared library" OFF)

# Configuração de padrão C++ moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configurações específicas de plataforma
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX AND NOT APPLE)
    add_definitions(-D_LINUX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Configuração de warnings
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Configuração de diretórios de saída
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Processar dependências de terceiros
add_subdirectory(3thparty)

# Encontrar fontes
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
)

file(GLOB_RECURSE HEADERS 
    "include/*.hpp" 
)

# Verificar se há fontes
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src directory!")
endif()

# Criar alvo principal
add_library(${PROJECT_NAME} 
    ${SOURCES}
    ${HEADERS}
)

# Configurações específicas do alvo
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Incluir diretórios (forma moderna preferida)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3thparty>
)

# Linkar com dependências
target_link_libraries(${PROJECT_NAME} PUBLIC
    3thparty::glfw
    3thparty::glad
    3thparty::imgui
)

# Configuração para instalação
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Configuração de exportação para uso em outros projetos
export(EXPORT ${PROJECT_NAME}Targets
    FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
)