#include <StrayGL/StrayGL.hpp>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

class BasicExample : public Stray::App
{
    float bgColor[3] = {0.1f, 0.2f, 0.3f};
    bool showDemo = false;
    bool shouldClose = false;

public:
    void initialize() override
    {
        createWindow("Basic Example", 800, 600);
    }

    void update(float deltaTime) override
    {
        if (shouldClose)
        {
            glfwSetWindowShouldClose(getWindow(), true);
        }
    }

    void render() override
    {
        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(getWindow());
    }

    void terminate() override
    {

    }
};
int main()
{
    BasicExample app;
    app.run();
    return 0;
}
