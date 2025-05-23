#include "StrayGL/Renderer/OpenGLRenderer.hpp"
#include "StrayGL/pch.h"

namespace Stray
{
    void OpenGLRenderer::clean(float r, float g, float b, float a){
        glClearColor(r, g, b, a);
    }

} // namespace Stray
