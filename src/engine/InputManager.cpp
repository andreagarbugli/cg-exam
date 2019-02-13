#include <iostream>

#include "InputManager.hpp"
#include "Game.hpp"
#include "GlobalStateManager.hpp"

namespace Engine
{
    void InputManager::Init()
    {
        if (!_initializated)
        {
            SDL_GetMouseState(&_mouseX, &_mouseY);
            _initializated = true;
        }
    }

    void InputManager::Update()
    {
        for (auto i = 0; i < KEYBOARD_SIZE; i++)
        {
            _keyDown[i] = false;
            _keyUp[i] = false;
        }

        for (auto i = 0; i < MOUSE_BUTTON_NUMBER; i++)
        {
            _mouseDownButtons[i] = false;
            _mouseUpButtons[i] = false;
        }

        SDL_Event event;
        bool moving = false;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    _quit = true;
                    break;

                case SDL_TEXTINPUT:
                    // WHAT
                    break;

                case SDL_KEYDOWN:
                {
                    _keyboard = SDL_GetKeyboardState(nullptr);
                    auto index = event.key.keysym.scancode;
                    _keyDown[index] = true;
                    break;
                }
                case SDL_KEYUP:
                {
                    _keyboard = SDL_GetKeyboardState(nullptr);
                    auto index = event.key.keysym.scancode;
                    _keyUp[index] = true;
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    moving = true;

                    _xChange = event.motion.xrel;
                    _yChange = -event.motion.yrel;

                    _lastX = _mouseX;
                    _lastY = _mouseY;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                    _mouse = SDL_GetMouseState(&(_mouseX), &(_mouseY));

                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        _mouseDownButtons[MouseButton::MOUSE_LEFT] = true;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        _mouseDownButtons[MouseButton::MOUSE_RIGHT] = true;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    _mouse = SDL_GetMouseState(&(_mouseX), &(_mouseY));

                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        _mouseUpButtons[MouseButton::MOUSE_LEFT] = true;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        _mouseUpButtons[MouseButton::MOUSE_RIGHT] = true;
                    }
                    break;

                    // Brand new SDL2 event.
                case SDL_MOUSEWHEEL:
                    // event.x; // Ammount scrolled horizontally
                    // // If negative, scrolled to the right
                    // // If positive, scrolled to the left

                    // event.y; // Ammount scrolled vertically
                    // // If negative, scrolled down
                    // // If positive, scrolled up
                    break;

                default:
                    break;
            }
        }

        if (!moving)
        {
            _xChange = 0.0f;
            _yChange = 0.0f;
        }
    }

    bool InputManager::IsKeyDown(KeyboardKey key)
    {
        if (_locked)
            return false;

        if (key < 0 || key >= KEYBOARD_SIZE)
            return false;

        return (_keyDown[key]);
    }

    bool InputManager::IsKeyUp(KeyboardKey key)
    {
        if (_locked)
            return false;

        if (key < 0 || key >= KEYBOARD_SIZE)
            return false;

        return (_keyUp[key]);
    }

    bool InputManager::IsKeyPressed(KeyboardKey key)
    {
        if (_locked)
            return false;

        if (!(_keyboard))
            return false;

        int SDLKey = static_cast<int>(key);

        if (_keyboard[SDLKey])
            return true;

        return false;
    }

    bool InputManager::IsMouseDown(MouseButton mouseButton)
    {
        if (_locked)
            return false;

        if (mouseButton == MOUSE_BUTTON_NUMBER)
            return false;

        return _mouseDownButtons[mouseButton];
    }

    bool InputManager::IsMouseUp(MouseButton mouseButton)
    {
        if (_locked)
            return false;

        if (mouseButton == MOUSE_BUTTON_NUMBER)
            return false;

        return _mouseUpButtons[mouseButton];
    }

    bool InputManager::IsMousePressed(MouseButton mouseButton)
    {
        if (_locked)
            return false;

        switch (mouseButton)
        {
            case MOUSE_LEFT:
                if (_mouse & SDL_BUTTON(1))
                    return true;
                break;

            case MOUSE_MIDDLE:
                if (_mouse & SDL_BUTTON(2))
                    return true;
                break;

            case MOUSE_RIGHT:
                if (_mouse & SDL_BUTTON(3))
                    return true;
                break;

            default:
                break;
        }

        return false;
    }


} // namespace Engine