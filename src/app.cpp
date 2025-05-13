#include "StrayGL/app.hpp"
#include <iostream>

namespace Stray
{
    App::App()
    {
        if (!glfwInit())
        {
            const char *description;
            int code = glfwGetError(&description);
            if (code != GLFW_NO_ERROR)
            {
                std::cerr << "Failed to initialize GLFW: (" << code << "): " << description << std::endl;
                throw std::runtime_error("Failed to initialize GLFW");
            }
        }
    }

    App::~App()
    {
        cleanup();
        glfwTerminate();
    }

    void App::createWindow(std::string title, int w, int h)
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }

        width = w;
        height = h;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            const char *description;
            int code = glfwGetError(&description);
            if (code != GLFW_NO_ERROR)
            {
                std::cerr << "GLFW Error (" << code << "): " << description << std::endl;
            }
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwDestroyWindow(window);
            window = nullptr;
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    void App::run()
    {
        isRunning = true;

        try
        {
            initialize();

            if (!window)
            {
                createWindow("Default Window", 800, 600);
            }

            loadContent();
            mainLoop();
            unloadContent();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            exit();
        }

        terminate();
    }

    void App::mainLoop()
    {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(window) && isRunning)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            glfwPollEvents();
            update(deltaTime);
            render();
            glfwSwapBuffers(window);
        }
    }

    void App::cleanup()
    {
        if (window)
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }
    }
} // namespace Stray