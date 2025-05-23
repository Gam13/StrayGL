#pragma once
#include "StrayGL/Renderer/IRenderer.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Stray
{
    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer(int majorVersion = 4, int minorVersion = 6);
        ~OpenGLRenderer() = default;

        bool initialize(void* window) override;
        void cleanup()override;
        void render() override;
        void resize(int width, int height) override;

        //Funções de Ferramenta
        void clean(float r, float g, float b, float a) override;

    private:
        GLFWwindow * glWindow = nullptr;
        int glMajorVersion;
        int glMinorVersion;
    };

} // namespace Stray
