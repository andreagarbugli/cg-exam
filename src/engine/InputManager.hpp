//
// Created by andrea on 01/10/18.
//

#ifndef ENGINE_INPUTMANAGER_HPP
#define ENGINE_INPUTMANAGER_HPP

#include <SDL2/SDL_events.h>

#include "InputDefinitions.hpp"
#include "GlobalStateManager.hpp"

namespace Engine
{
class InputManager
{
  private:
    const uint8_t *_keyboard = nullptr;
    unsigned int _mouse;

    bool _mouseFirstMoved = true;

    bool _initializated = false;

    int _lastX = 0;
    int _lastY = 0;
    float _xChange = 0;
    float _yChange = 0;
    int _mouseX = 0;
    int _mouseY = 0;

    bool _keyDown[KEYBOARD_SIZE];
    bool _keyUp[KEYBOARD_SIZE];

    bool _mouseDownButtons[MOUSE_BUTTON_NUMBER];
    bool _mouseUpButtons[MOUSE_BUTTON_NUMBER];

    bool _quit = false;
    bool _locked = false;

    GlobalStateManager *_globalStateManager = GlobalStateManager::GetInstance();

  public:
    static InputManager *GetInstance()
    {
        static auto *_instance = new InputManager();
        return _instance;
    }

    void Init();

    void Update();

    bool IsKeyDown(KeyboardKey key);

    bool IsKeyUp(KeyboardKey key);

    bool IsKeyPressed(KeyboardKey key);

    bool IsMouseDown(MouseButton mouseButton);

    bool IsMouseUp(MouseButton mouseButton);

    bool IsMousePressed(MouseButton mouseButton);

    bool QuitRequested()
    {
        return _quit;
    }

    int GetMouseX()
    {
        return _mouseX;
    }

    int GetMouseY()
    {
        return _mouseY;
    }

    int GetLastX() const
    {
        return _lastX;
    }

    int GetLastY() const
    {
        return _lastY;
    }

    float GetXChange() const
    {
        return _xChange;
    }

    float GetYChange() const
    {
        return _yChange;
    }

    void Lock()
    {
        _locked = true;
    }

    void Unlock()
    {
        _locked = false;
    }

  private:
    InputManager() = default;
};

} // namespace Engine

#endif //ENGINE_INPUTMANAGER_HPP
