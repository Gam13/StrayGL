#pragma once

namespace Stray
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        
        virtual bool initialize(void* window) = 0;
        virtual void cleanup() = 0;
        virtual void render() = 0;
        virtual void resize(int width, int height) = 0;

        virtual void clean(float r, float g, float b, float a) = 0;
    };
}