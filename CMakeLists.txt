cmake_minimum_required(VERSION 3.15)
project(StrayGL LANGUAGES C CXX)

set(STRAYGL_DEVELOPMENT ON)

#==================================================
# Configuração de diretórios
#==================================================
set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/build/vcpkg")
set(VCPKG_INSTALL_DIR "${CMAKE_SOURCE_DIR}/build/vcpkg_installed")

# Força vcpkg a instalar em build/
set(ENV{VCPKG_ROOT} "${VCPKG_DIR}")
set(ENV{VCPKG_INSTALLED_DIR} "${VCPKG_INSTALL_DIR}")

# Define triplet padrão se não especificado
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(VCPKG_TARGET_TRIPLET "x64-osx")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(VCPKG_TARGET_TRIPLET "x64-windows")
    endif()
endif()

#==================================================
# Baixa e configura o vcpkg se necessário
#==================================================
if(NOT EXISTS "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "🔧 Baixando e configurando vcpkg...")

    file(DOWNLOAD https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip
         ${CMAKE_BINARY_DIR}/vcpkg.zip
         STATUS download_status
         SHOW_PROGRESS)

    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "❌ Falha ao baixar o vcpkg.")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf vcpkg.zip
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

    file(GLOB extracted_dir "${CMAKE_SOURCE_DIR}/build/vcpkg-*")
    file(RENAME "${extracted_dir}" "${VCPKG_DIR}")

    execute_process(COMMAND ./bootstrap-vcpkg.sh
                    WORKING_DIRECTORY "${VCPKG_DIR}"
                    RESULT_VARIABLE bootstrap_result)

    if(NOT bootstrap_result EQUAL 0)
        message(FATAL_ERROR "❌ Falha ao compilar o vcpkg.")
    endif()
endif()

#==================================================
# Instala dependências (modo manifesto)
#==================================================
execute_process(COMMAND ./vcpkg install --x-install-root=${VCPKG_INSTALL_DIR}
                WORKING_DIRECTORY ${VCPKG_DIR}
                RESULT_VARIABLE install_result)

if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "❌ Falha ao instalar dependências com vcpkg.")
endif()

#==================================================
# Configura o Toolchain do vcpkg
#==================================================
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALL_DIR}/${VCPKG_TARGET_TRIPLET}")
message(STATUS "✅ Usando vcpkg em: ${VCPKG_DIR} (triplet: ${VCPKG_TARGET_TRIPLET})")

#==================================================
# Configuração de compilador
#==================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#==================================================
# Verificação do PCH
#==================================================
set(PCH_HEADER "${CMAKE_SOURCE_DIR}/include/StrayGL/pch.h")
if(NOT EXISTS ${PCH_HEADER})
    message(FATAL_ERROR "❌ Arquivo PCH não encontrado: ${PCH_HEADER}")
else()
    message(STATUS "📦 PCH encontrado em: ${PCH_HEADER}")
endif()

#==================================================
# Dependências (serão encontradas via vcpkg)
#==================================================
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

#==================================================
# Fonte principal
#==================================================
set(CXX_SOURCES 
    src/app.cpp
    src/Logger.cpp
    src/Renderer/OpenGLRenderer.cpp
    src/Renderer/RendererFactory.cpp
)

add_library(StrayGL ${CXX_SOURCES})

target_include_directories(StrayGL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#==================================================
# PCH
#==================================================
if(MSVC)
    target_precompile_headers(StrayGL PRIVATE ${PCH_HEADER})
else()
    set_source_files_properties(${CXX_SOURCES} PROPERTIES
        COMPILE_FLAGS "-include ${PCH_HEADER}"
    )
endif()

#==================================================
# Link com bibliotecas vcpkg
#==================================================
target_link_libraries(StrayGL PRIVATE 
    glfw
    glad::glad
    spdlog::spdlog
    imgui::imgui
)

#==================================================
# Instalação
#==================================================
install(TARGETS StrayGL EXPORT StrayGLTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT StrayGLTargets
    FILE StrayGLConfig.cmake
    NAMESPACE StrayGL::
    DESTINATION lib/cmake/StrayGL
)

install(DIRECTORY include/StrayGL DESTINATION include)

#==================================================
# Exemplos (modo desenvolvimento)
#==================================================
if(STRAYGL_DEVELOPMENT)
    add_subdirectory(examples)
endif()
