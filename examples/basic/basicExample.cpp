#include <StrayGL/StrayGL.hpp>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

class BasicExample : public Stray::App
{
    bool showDemo = false;
    bool shouldClose = false;

public:
    void initialize() override
    {
        createWindow("Basic Example", 800, 600,Stray::StrayAPI::OpenGL);
    }

    void update(float deltaTime) override
    {
        if (shouldClose)
        {
            glfwSetWindowShouldClose(getWindow(), true);
        }
    }

    void terminate() override
    {

    }
};

STRAY_RUN(BasicExample)
