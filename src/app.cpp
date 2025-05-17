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

    void App::createWindow(std::string title, int w, int h, StrayAPI api)
    {
        if (window)
        {
            cleanup();
        }

        width = w;
        height = h;

        // Obter as dicas específicas da API
        auto hints = RendererFactory::getRequiredWindowHints(api);

        // Aplicar as dicas antes de criar a janela
        for (const auto &hint : hints)
        {
            glfwWindowHint(hint.first, hint.second);
        }

        // Criar a janela
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

        // Criar e inicializar o renderer
        renderer = RendererFactory::createRenderer(api);
        if (!renderer->initialize(window))
        {
            throw std::runtime_error("Failed to initialize renderer");
        }

        // Configurar o callback de redimensionamento
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                       {
        auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->width = width;
        app->height = height;
        if (app->renderer) {
            app->renderer->resize(width, height);
        } });

        glfwMakeContextCurrent(window);
    }

    void App::run()
    {
        isRunning = true;

        try
        {
            initialize();

            // Se não houver janela criada, criar uma padrão
            if (!window)
            {
                createWindow("Default Window", width, height, StrayAPI::OpenGL);
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

            renderer->render();

            glfwSwapBuffers(window);
        }
    }

    void App::cleanup()
    {
        if (renderer)
        {
            renderer->cleanup();
            renderer.reset();
        }

        if (window)
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }
    }
} // namespace Stray