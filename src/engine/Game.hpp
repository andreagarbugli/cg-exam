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

#include "game/Minimap.hpp"

#include "graphics/GUIComponent.hpp"
#include "graphics/Light.hpp"
#include "graphics/Model.hpp"
#include "graphics/Skybox.hpp"
#include "graphics/Terrain.hpp"
#include "graphics/Texture.hpp"

#include "shaders/BasicShader.hpp"

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
        Window* _window{};

        GlobalStateManager* _globalStateManager = GlobalStateManager::GetInstance();
        InputManager* _inputManager = InputManager::GetInstance();

        std::vector<class GameObject*> _gameObjects;
        std::vector<class GameObject*> _pendingGameObjects;

        Graphics::Skybox _skybox{};

        std::unordered_map<std::string, Graphics::BasicShader*> _shaders;

        std::unordered_map<std::string, Graphics::Font*> _fonts;
        std::unordered_map<std::string, Graphics::GUIComponent*> _guiStack;

        BasicCamera* _currentCamera{};
        Graphics::Terrain* _terrain{};

        RacingGame::Minimap* _minimap{};

        Graphics::DirectionalLight* _directionalLight{};

        std::vector<Graphics::PointLight*> _pointLights;
        std::vector<Graphics::SpotLight*> _spotLights;

        bool _updatingGameObjects = false;
        bool _isLineMode = false;
        int _cameraMode = 0;
        bool _isMouseCaptured = false;
        bool _isMenuVisible = true;
        bool _isMinimapActive = true;
        bool _isNight = true;
        bool _isWinner = false;
        bool _isPrintInfo = true;
        bool _isPointLightActive = true;
        bool _isSpotLightActive = true;

    public:

        Game() = default;

        virtual ~Game() = default;

        void Run();

        bool AddGameObject(GameObject* gameObject);

        BasicCamera* GetCurrentCamera();

        Graphics::DirectionalLight* GetDirectionalLight();

        std::unordered_map<std::string, Graphics::GUIComponent*> GetGUIStack();

        void PushGUI(const std::string& name, Graphics::GUIComponent* gui);

        Graphics::Font* GetFont(std::string& fileName);

        GameObject* GetGameObjectByName(const std::string& gameObjectName);

        std::vector<Graphics::PointLight*> GetPointLights();

        std::vector<Graphics::SpotLight*> GetSpotLights();

        glm::mat4 GetProjection();

        Window* GetWindow();

        bool IsNight();

        bool IsWinner() const;

        bool IsPrintInfo();

    protected:

        virtual bool Init();

        virtual void Update(double elapsedTime);

        virtual void ProcessInput();

        virtual void Render();

        virtual void StartUp();

        virtual void ShutDown();

    private:

        void _CreateCamera();

        void _CreateLights();

        void _CreateMaterials();

        void _CreateModels();

        void _CreateSkybox();

        void _InitGLEW();

        bool _InitSDLImage() const;

        bool _InitSDLVideo() const;

        void _SetupOpenGLAttributes() const;

        void _RenderMinimap();

        void _RenderGUI();

        void _RenderScene(unsigned int frameBuffer);

        void _CreateTrees(Graphics::Model* treeModel);
    };

} // namespace Engine

#endif //ENGINE_APP_HPP
