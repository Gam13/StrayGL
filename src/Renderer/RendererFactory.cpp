// RendererFactory.cpp
#include "StrayGL/Renderer/RendererFactory.hpp"
#include "StrayGL/Renderer/OpenGLRenderer.hpp"
#include <stdexcept>

namespace Stray {
    RendererFactory::WindowHints RendererFactory::getRequiredWindowHints(StrayAPI api) {
        switch(api) {
            case StrayAPI::OpenGL:
                return {
                    {GLFW_CONTEXT_VERSION_MAJOR, 4},
                    {GLFW_CONTEXT_VERSION_MINOR, 6},
                    {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE}
                };
            case StrayAPI::None:
            default:
                return {};
        }
    }

    std::unique_ptr<IRenderer> RendererFactory::createRenderer(StrayAPI api) {
        switch(api) {
            case StrayAPI::OpenGL: return std::make_unique<OpenGLRenderer>();
            case StrayAPI::None:
            default: throw std::runtime_error("Unsupported renderer API");
        }
    }
}