#==================================================
# Configuração do GLFW (obrigatório)
#==================================================
if(NOT TARGET glfw)
    message(STATUS "Configurando GLFW")
    
    # Desativa componentes desnecessários do GLFW
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    
    add_subdirectory(3thparty/GLFW)
    
    # Verifica se o target foi criado
    if(NOT TARGET glfw AND NOT TARGET glfw3)
        message(FATAL_ERROR "Falha ao configurar GLFW")
    endif()
    
    # Cria alias consistente
    if(TARGET glfw3)
        add_library(glfw ALIAS glfw3)
    endif()
endif()