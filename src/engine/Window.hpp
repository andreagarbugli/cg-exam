#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include <string>

#include <SDL2/SDL_video.h>
#include <SDL_render.h>

namespace Engine
{
    class Window
    {
    private:
        SDL_Window* _sdlWindow;
        SDL_GLContext _glContext;
        SDL_Renderer* _sdlRenderer;

        int _width;
        int _height;

    public:
        Window() = default;

        static Window* CreateWindow(const std::string& name, int width, int height);

        void SwapBuffers();

        void Close();

        void Clear();

        int GetHeight();

        float GetRatio();

        int GetWidth();

        SDL_Renderer* GetSdlRenderer() const;

    private:
        Window(SDL_Window* window, SDL_GLContext context, int width, int height) :
                _sdlWindow{window},
                _glContext{context},
                _width{width},
                _height{height}
        {
            _sdlRenderer = SDL_GetRenderer(_sdlWindow);
        }
    };
} // namespace Engine

#endif //ENGINE_WINDOW_HPP
