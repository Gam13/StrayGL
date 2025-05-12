#include "StrayGL/app.hpp"
#include <glad/glad.h>
#include <iostream>
#include <chrono>


namespace Stray
{
    bool App::mainLoop() {

        onInit();
        auto lastTime = std::chrono::high_resolution_clock::now();
        while(!glfwWindowShouldClose(m_window))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime-lastTime).count();
            lastTime = currentTime;

            onUpdate(deltaTime);

            onRender();
            glfwSwapBuffers(m_window);
        }

        onClose();
        return true;
    }



    void App::createWindow(std::string title, int width, int height) {

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }
        

    }


    bool App::init() {
        if (!glfwInit()) return false;
        return mainLoop();
    }

    void App::run() {
        if (!init()) {
            throw std::runtime_error("Ocorreu uma falha na execução");
        }
    }

    App::~App() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }
} // namespace Stray