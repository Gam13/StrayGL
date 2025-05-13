#pragma once
#ifndef STRAY_APP_HPP
#define STRAY_APP_HPP
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <chrono>

namespace Stray
{
    class App
    {
    protected:
        // Métodos virtuais que podem ser sobrescritos
        virtual void initialize() {}       // Equivalente ao onInit original
        virtual void loadContent() {}      // Novo: para carregar assets
        virtual void update(float deltaTime) {}  // Equivalente ao onUpdate
        virtual void render() {}           // Equivalente ao onRender
        virtual void unloadContent() {}    // Novo: para liberar recursos
        virtual void terminate() {}        // Equivalente ao onClose

        // Utilitários de acesso
        GLFWwindow* getWindow() const { return window; }
        float getAspectRatio() const { return width / static_cast<float>(height); }

    private:
        GLFWwindow* window = nullptr;
        int width = 800;
        int height = 600;
        bool isRunning = false;

        // Métodos internos
        void mainLoop();
        void cleanup();

    public:
        App();
        virtual ~App();

        // Interface pública
        void createWindow(std::string title, int width, int height);
        void run();
        void exit() { isRunning = false; }
    };

    // Macro para criar o ponto de entrada automaticamente
    #define STRAY_RUN_APP(APP_CLASS) \
        int main() { \
            APP_CLASS app; \
            app.run(); \
            return 0; \
        }

} // namespace Stray

#endif // STRAY_APP_HPP