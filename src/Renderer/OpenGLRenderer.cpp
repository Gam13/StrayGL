#include "StrayGL/Renderer/OpenGLRenderer.hpp"
#include "StrayGL/pch.h"

namespace Stray
{

    OpenGLRenderer::OpenGLRenderer(int majorVersion, int minorVersion)
        : glMajorVersion(majorVersion), glMinorVersion(minorVersion) {}

    bool OpenGLRenderer::initialize(void *window)
    {
        glWindow = static_cast<GLFWwindow *>(window);
        if (!glWindow)
        {
            std::cerr << "Invalid window handle provided to OpenGLRenderer" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(glWindow);

        // Configurações específicas do OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        // Configurações iniciais do OpenGL
        glViewport(0, 0, 800, 600);
        glClearColor(0.3f, 0.1f, 0.3f, 1.0f);

        return true;
    }

    void OpenGLRenderer::cleanup() {}

    void OpenGLRenderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderer::resize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

} // namespace Stray