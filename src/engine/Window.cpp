//
// Created by andrea on 22/10/18.
//

#include <iostream>
#include <GL/gl.h>

#include "Window.hpp"

namespace Engine
{
    Window Window::CreateWindow(std::string name, int width, int height)
    {
        int dispNumber = SDL_GetNumVideoDisplays() - 1;

        auto sdlWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED_DISPLAY(dispNumber),
                                          SDL_WINDOWPOS_CENTERED_DISPLAY(dispNumber),
                                          width, height,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (sdlWindow == nullptr)
        {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            throw std::invalid_argument("received negative value");
        }

        // Create our opengl context and attach it to our window
        auto glContext = SDL_GL_CreateContext(sdlWindow);

        return { sdlWindow, glContext, width, height };
    }

    void Window::SwapBuffers()
    {
        SDL_GL_SwapWindow(_sdlWindow);
    }

    void Window::Close()
    {
        SDL_GL_DeleteContext(_glContext);
        SDL_DestroyWindow(_sdlWindow);
    }

    void Window::Clear()
    {
        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    int Window::GetHeight()
    {
        return _height;
    }

    float Window::GetRatio()
    {
        return _width / (float) _height;
    }

    int Window::GetWidth()
    {
        return _width;
    }

} // namespace Engine
