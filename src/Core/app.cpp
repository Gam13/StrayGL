#include "StrayGL/app.hpp"
#include "StrayGL/Logger.hpp"
#include "StrayGL/pch.h"

namespace Stray
{
    App::App()
    {
        if (!Logger::getCoreLogger()) {
            Logger::init("StrayGL.log");
        }

        STRAY_INFO("Initializing StrayGL Application Framework");
        STRAY_DEBUG("GLFW Version: {}.{}.{}", 
            GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

        if (!glfwInit())
        {
            const char* description;
            int code = glfwGetError(&description);
            STRAY_CRITICAL("Failed to initialize GLFW: ({}): {}", code, description);
            throw std::runtime_error("Failed to initialize GLFW");
        }

        STRAY_DEBUG("GLFW initialized successfully");
    }

    App::~App()
    {
        STRAY_DEBUG("Destroying application instance");
        cleanup();
        glfwTerminate();
        STRAY_INFO("Application terminated");
    }

    void App::createWindow(std::string title, int w, int h, StrayAPI api)
    {
        STRAY_INFO("Creating window: '{}' ({}x{}) with API: {}", 
                  title, w, h, static_cast<int>(api));

        if (window)
        {
            STRAY_WARN("Window already exists - cleaning up previous window");
            cleanup();
        }

        width = w;
        height = h;

        // Obter as dicas específicas da API
        auto hints = RendererFactory::getRequiredWindowHints(api);
        STRAY_DEBUG("Applying {} window hints", hints.size());

        // Aplicar as dicas antes de criar a janela
        for (const auto& hint : hints)
        {
            glfwWindowHint(hint.first, hint.second);
        }

        // Criar a janela
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            const char* description;
            int code = glfwGetError(&description);
            STRAY_ERROR("Failed to create GLFW window: ({}): {}", code, description);
            throw std::runtime_error("Failed to create GLFW window");
        }

        STRAY_DEBUG("Window created successfully");

        // Criar e inicializar o renderer
        STRAY_INFO("Creating renderer for API: {}", static_cast<int>(api));
        renderer = RendererFactory::createRenderer(api);
        
        if (!renderer->initialize(window))
        {
            STRAY_CRITICAL("Failed to initialize renderer");
            throw std::runtime_error("Failed to initialize renderer");
        }

        STRAY_INFO("Renderer initialized successfully");

        // Configurar o callback de redimensionamento
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            auto app = static_cast<App*>(glfwGetWindowUserPointer(window));
            app->width = width;
            app->height = height;
            if (app->renderer) {
                STRAY_TRACE("Window resized to {}x{}", width, height);
                app->renderer->resize(width, height);
            }
        });

        glfwMakeContextCurrent(window);
        STRAY_DEBUG("OpenGL context set to current");

        // Configurar callbacks de erro
        glfwSetErrorCallback([](int error, const char* description) {
            STRAY_ERROR("GLFW Error ({}): {}", error, description);
        });
    }

    void App::run()
    {
        isRunning = true;
        STRAY_INFO("Starting application main loop");

        try
        {
            STRAY_DEBUG("Calling initialize()");
            initialize();

            // Se não houver janela criada, criar uma padrão
            if (!window)
            {
                STRAY_WARN("No window created - creating default window");
                createWindow("Default Window", width, height, StrayAPI::OpenGL);
            }

            STRAY_DEBUG("Calling loadContent()");
            loadContent();
            
            STRAY_INFO("Entering main loop");
            mainLoop();
            
            STRAY_DEBUG("Calling unloadContent()");
            unloadContent();
        }
        catch (const std::exception& e)
        {
            STRAY_CRITICAL("Unhandled exception: {}", e.what());
            exit();
        }

        STRAY_DEBUG("Calling terminate()");
        terminate();
        STRAY_INFO("Application run cycle completed");
    }

    void App::mainLoop()
    {
        auto lastTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;
        float timeAccumulator = 0.0f;

        STRAY_DEBUG("Starting main loop with window {}", static_cast<void*>(window));

        while (!glfwWindowShouldClose(window) && isRunning)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // FPS calculation
            frameCount++;
            timeAccumulator += deltaTime;
            if (timeAccumulator >= 1.0f)
            {
                frameCount = 0;
                timeAccumulator = 0.0f;
            }

            glfwPollEvents();
            update(deltaTime);

            draw();
            renderer->render();

            glfwSwapBuffers(window);
        }

        STRAY_DEBUG("Exiting main loop");
    }

    void App::cleanup()
    {
        STRAY_DEBUG("Cleaning up resources");

        if (renderer)
        {
            STRAY_INFO("Cleaning up renderer");
            renderer->cleanup();
            renderer.reset();
        }

        if (window)
        {
            STRAY_INFO("Destroying window");
            glfwDestroyWindow(window);
            window = nullptr;
        }

        STRAY_DEBUG("Cleanup completed");
    }
} // namespace Stray