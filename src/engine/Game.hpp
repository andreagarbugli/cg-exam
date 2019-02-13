#ifndef ENGINE_APP_HPP
#define ENGINE_APP_HPP

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "Camera.hpp"
#include "GameObject.hpp"
#include "GlobalStateManager.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

#include "graphics/GLSLShader.hpp"
#include "graphics/Light.hpp"
#include "graphics/Model.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Skybox.hpp"
#include "graphics/Terrain.hpp"

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

const float FOV = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

namespace Engine
{
    class Game
    {
    private:
        Window _window{ };

        GlobalStateManager* _globalStateManager = GlobalStateManager::GetInstance();
        InputManager* _inputManager = InputManager::GetInstance();

        std::vector<class GameObject*> _gameObjects;
        std::vector<class GameObject*> _pendingGameObjects;

        Graphics::Skybox _skybox{ };

        std::map<std::string, Graphics::Texture*> _textures;
        std::map<std::string, Graphics::GLSLShader*> _shaders;
        std::vector<Graphics::Model*> _models;

        Camera* _currentCamera{ };
        Graphics::Terrain* _terrain;

        Graphics::DirectionalLight* _directionalLight{ };
        std::vector<Graphics::PointLight*> _pointLights;

        bool _updatingGameObjects = false;
        bool _isMouseCaptured = true;

    public:

        Game() = default;

        virtual ~Game() = default;

        void Run();

        bool AddGameObject(GameObject* gameObject);

        Camera* GetCurrentCamera();

        Graphics::DirectionalLight* GetDirectionalLight();

        std::vector<Graphics::PointLight*> GetPointLight();

        glm::mat4 GetProjection();

    protected:

        virtual bool Init();

        virtual void Update(double elapsedTime);

        virtual void ProcessInput();

        virtual void Render();

        virtual void StartUp();

        virtual void ShutDown();

    private:

        void _CreateCamera();

        void _CreateTextures();

        GameObject* _GetGameObjectByName(const std::string& gameObjectName);

        void _InitGLEW();

        bool _InitSDLImage() const;

        bool _InitSDLVideo() const;

        void _SetupOpenGLAttributes() const;

        void _CreateLights();
    };

} // namespace Engine

#endif //ENGINE_APP_HPP
