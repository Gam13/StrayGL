cmake_minimum_required(VERSION 3.15)
project(StrayGL LANGUAGES C CXX)

set(STRAYGL_DEVELOPMENT = ON)

#==================================================
# Configuração do vcpkg com fallback
#==================================================
set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/build/vcpkg")
set(VCPKG_INSTALL_DIR "${CMAKE_SOURCE_DIR}/build/vcpkg_installed")

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

# Executa Makefile se o vcpkg não estiver pronto
if(NOT EXISTS "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "🔧 Configurando vcpkg automaticamente...")
    execute_process(
        COMMAND make configure
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE result
    )
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Falha na configuração automática. Execute manualmente:\n  make deps")
    endif()
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
message(STATUS "✅ Usando vcpkg em: ${VCPKG_DIR} (triplet: ${VCPKG_TARGET_TRIPLET})")
#==================================================
# Configurações de compilação
#==================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#==================================================
# Verificação do PCH
#==================================================
set(PCH_HEADER "${CMAKE_SOURCE_DIR}/include/StrayGL/pch.h")
if(NOT EXISTS ${PCH_HEADER})
    message(FATAL_ERROR "Arquivo PCH não encontrado em: ${PCH_HEADER}")
else()
    message(STATUS "PCH encontrado em: ${PCH_HEADER}")
endif()

#==================================================
# Dependências
#==================================================
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALL_DIR}/${VCPKG_TARGET_TRIPLET}")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


#==================================================
# Biblioteca Principal
#==================================================
set(CXX_SOURCES 
    src/app.cpp
    src/Logger.cpp
    src/Renderer/OpenGLRenderer.cpp
    src/Renderer/RendererFactory.cpp
)

add_library(StrayGL ${CXX_SOURCES})

# Configurações de instalação
target_include_directories(StrayGL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#==================================================
# Includes e PCH
#==================================================
if(MSVC)
    target_precompile_headers(StrayGL PRIVATE ${PCH_HEADER})
else()
    set_source_files_properties(${CXX_SOURCES} PROPERTIES
        COMPILE_FLAGS "-include ${PCH_HEADER}"
    )
endif()

#==================================================
# Link com bibliotecas do vcpkg
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
# Exemplos (somente em modo desenvolvimento)
#==================================================
if(STRAYGL_DEVELOPMENT)
    add_subdirectory(examples)
endif()