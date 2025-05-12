#include <GLFW/glfw3.h>

namespace Stray
{
    class App
    {
    private:
        bool mainLoop();
        bool init();

        GLFWwindow* m_window;
    public:
        App() = default;
        ~App();

        void run();//Função com o proprosito de rodar o programa

        void createWindow(std::string title, int width, int height);

        //Funções expostas pra aplicações
        virtual void onInit(); 
        virtual void onUpdate(float deltaTime);
        virtual void onRender();
        virtual void onClose();
    };



} // namespace Stray
