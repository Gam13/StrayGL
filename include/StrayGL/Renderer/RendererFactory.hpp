// RendererFactory.hpp
#pragma once
#include "StrayGL/Renderer/IRenderer.hpp"
#include <memory>
#include <vector>
#include <utility>

namespace Stray {
    enum StrayAPI { None = 0, OpenGL };
    
    class RendererFactory {
    public:
        using WindowHint = std::pair<int, int>;
        using WindowHints = std::vector<WindowHint>;
        
        static WindowHints getRequiredWindowHints(StrayAPI api);
        static std::unique_ptr<IRenderer> createRenderer(StrayAPI api);
    };
}