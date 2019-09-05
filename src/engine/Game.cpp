#include <algorithm>
#include <iostream>

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL_image.h>
#include <utils/GCGraLib2.hpp>

#include "Game.hpp"

#include "game/Car.hpp"
#include "game/Menu.hpp"
#include "game/StaticObject.hpp"

#include "math/Math.hpp"

#include "shaders/MaterialShader.hpp"
#include "shaders/TerrainShader.hpp"
#include "shaders/GUIShader.hpp"

#include "utils/ObjFileLoader.hpp"

using namespace std;
using namespace Graphics;

namespace Engine
{
    void Game::Run()
    {
        if (Init())
        {
            _globalStateManager->Start();

            StartUp();

            const double MS_PER_UPDATE = 0.001;

            double previousFrameTime = SDL_GetPerformanceCounter() /
                                       (double) SDL_GetPerformanceFrequency();
            double lag = 0.0;

            double totalTime = 0.0;
            int totalFrames = 0;
            int totalUpdate = 0;

            // game loop
            while (_globalStateManager->IsRunning())
            {
                double currentFrameTime = SDL_GetPerformanceCounter() /
                                          (double) SDL_GetPerformanceFrequency();

                double elapsedTime = currentFrameTime - previousFrameTime;

                previousFrameTime = currentFrameTime;
                lag += elapsedTime;

                ProcessInput();

                while (lag >= MS_PER_UPDATE)
                {
                    Update(MS_PER_UPDATE);
                    lag -= MS_PER_UPDATE;
                    totalUpdate++;
                }

                // generate outputs
                _window->Clear();
                Render();
                _window->SwapBuffers();

                totalTime += elapsedTime;
                totalFrames++;

                if (totalTime >= 1.0)
                {
                    cout << "FPS: " << totalFrames << endl;
                    cout << "Total Update in 1s: " << totalUpdate << endl;
                    totalFrames = 0;
                    totalUpdate = 0;
                    totalTime = 0.0;
                }
            }

            ShutDown();

            _window->Close();

            SDL_Quit();
        }
    }

    bool Game::Init()
    {
        if (!_InitSDLVideo() || !_InitSDLImage())
        {
            return false;
        }

        _SetupOpenGLAttributes();

        _window = Window::CreateWindow("Window", WINDOW_WIDTH, WINDOW_HEIGHT);

        _InitGLEW();

        return true;
    }

    bool Game::_InitSDLVideo() const
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            cerr << "SDL_Init(SDL_INIT_VIDEO) failed: " << SDL_GetError() << endl;
            return false;
        }

        IMG_Init(IMG_INIT_PNG);

        return true;
    }

    bool Game::_InitSDLImage() const
    {
        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        int initted = IMG_Init(flags);

        if ((initted & flags) != flags)
        {
            cerr << "IMG_Init: Failed to init required jpg and png support!" << endl;
            cerr << "IMG_Init: " << IMG_GetError() << endl;
            return false;
        }

        if (TTF_Init() != 0)
        {
            cerr << "TTF_Init: Failed to init text support!" << endl;
            cerr << "TTF_Init:: " << IMG_GetError() << endl;
            return false;
        }

        return true;
    }

    void Game::_SetupOpenGLAttributes() const
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    }

    void Game::_InitGLEW()
    {
        if (gl3wInit())
        {
            cerr << "failed to initialize OpenGL" << endl;
        }

        glViewport(0, 0, _window->GetWidth(), _window->GetHeight());

        // cull face with with default settings
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    void Game::StartUp()
    {
        _CreateSkybox();

        // create some materials
        _CreateMaterials();

        // create models
        _CreateModels();

        // create terrain
        _terrain = new Terrain(this, "terrain1");
        _terrain->SetShader(_shaders["terrainShader"]);

        _minimap = new RacingGame::Minimap(this, "minimap");
        _minimap->SetScale(glm::vec2(0.20f, 0.20f));
        _minimap->SetPosition(glm::vec2(0.75f, -0.75f));
        _minimap->SetRatio(0.25f);
        _minimap->CreateMinimapTarget();

        auto menu = new RacingGame::Menu(this, "speed-gui");
        menu->SetPosition(glm::vec2(-0.75f, -0.75f));

        _CreateCamera();

        _CreateLights();
    }

    void Game::_CreateCamera()
    {
        auto cameraStartPosition = glm::vec3(0.0f, 5.0f, 5.0f);

        auto camera = new Camera(this, "freeCamera");

        camera->SetPosition(cameraStartPosition);
        camera->SetMovimentSpeed(20.0f);

        AddGameObject(camera);

        auto minimpCamera = new FollowCamera(this, "minimapCamera");
        minimpCamera->SetPlayer(GetGameObjectByName("lambo"));
        minimpCamera->SetHorizontalDistance(1.0f);
        minimpCamera->SetVerticalDistance(80.0f);

        AddGameObject(minimpCamera);

        auto followCamera = new FollowCamera(this, "followCamera");
        followCamera->SetPlayer(GetGameObjectByName("lambo"));

        _currentCamera = followCamera;

        AddGameObject(followCamera);
    }

    void Game::_CreateLights()
    {
        _directionalLight = new DirectionalLight();
        _directionalLight->direction = glm::vec3(10.0f, -50.0f, 20.0f);
        _directionalLight->ambient = glm::vec3(0.15f, 0.15f, 0.15f);
        _directionalLight->diffuse = glm::vec3(0.15f, 0.15f, 0.15f);

        _directionalLight->specular = glm::vec3(1.1f);

        glm::vec3 direction(0.0f, 0.0f, -1.0f);
        glm::vec3 carLightColor(0.94902f, 0.96471f, 0.98824f);
        float constant = 1.0f;
        float linear = 0.045f;
        float quadratic = 0.0075f;
        float cutOff = 14.5f;
        float outerCutOff = 25.5f;

        auto carLightR = new SpotLight();
        carLightR->direction = direction;
        carLightR->position = glm::vec3(0.0f);
        carLightR->ambient = carLightColor;
        carLightR->diffuse = carLightColor;
        carLightR->specular = carLightColor;

        carLightR->constant = constant;
        carLightR->linear = linear;
        carLightR->quadratic = quadratic;

        carLightR->cutOff = cutOff;
        carLightR->outerCutOff = outerCutOff;

        auto carLightL = new SpotLight();
        carLightL->direction = direction;
        carLightL->position = glm::vec3(0.0f);
        carLightL->ambient = carLightColor;
        carLightL->diffuse = carLightColor;
        carLightL->specular = carLightColor;

        carLightL->constant = constant;
        carLightL->linear = linear;
        carLightL->quadratic = quadratic;

        carLightL->cutOff = cutOff;
        carLightL->outerCutOff = outerCutOff;

        _spotLights.push_back(carLightR);
        _spotLights.push_back(carLightL);
    }

    void Game::_CreateMaterials()
    {
        auto materialShader = new MaterialShader(this, "materialShader");
        auto terrainShader = new TerrainShader(this, "terrainShader");
        auto guiShader = new GUIShader(this, "guiShader");

        _shaders.emplace(materialShader->GetName(), materialShader);
        _shaders.emplace(terrainShader->GetName(), terrainShader);
        _shaders.emplace(guiShader->GetName(), guiShader);
    }

    void Game::_CreateModels()
    {
        srand(SDL_GetTicks());

        auto lamboModel = new Model("lambo");
        lamboModel->Load("models/lambo.obj");
        lamboModel->SetShader(_shaders["materialShader"]);

        auto car = new RacingGame::Car(this, "lambo");
        car->SetModel(lamboModel);
        AddGameObject(car);

        auto treeModel = new Model("Oak_Tree");
        treeModel->Load("models/Oak_Tree.obj");
        treeModel->SetShader(_shaders["materialShader"]);

        _CreateTrees(treeModel);

        treeModel = new Model("Fir_Tree");
        treeModel->Load("models/Fir_Tree.obj");
        treeModel->SetShader(_shaders["materialShader"]);

        _CreateTrees(treeModel);

        auto lampModel = new Model("lamp_spot");
        lampModel->Load("models/lamp_spot.obj");
        lampModel->SetShader(_shaders["materialShader"]);

        for (int i = 0; i < 5; ++i)
        {
            float x = rand() % 50 - 10;
            float z = rand() % 50 - 10;
            float angle = rand() % 360;

            auto lampPosition = glm::vec3(x, 0.0f, z);

            auto lamp = new RacingGame::StaticObject(this, "lamp" + std::to_string(i));
            lamp->SetModel(lampModel);
            lamp->SetPosition(lampPosition);
            lamp->SetRotation(angle, glm::vec3(0.0f, 1.0f, 0.0f));

            AddGameObject(lamp);

            auto pointLight = new PointLight();
            pointLight->position = lampPosition + glm::vec3(0.0f, 3.0f, 0.0f);

            pointLight->ambient = glm::vec3(drand48(), drand48(), drand48());
            pointLight->diffuse = glm::vec3(drand48(), drand48(), drand48());
            pointLight->specular = glm::vec3(drand48());

            pointLight->constant = 1.0f;
            pointLight->linear = 0.09f;
            pointLight->quadratic = 0.032f;

            _pointLights.push_back(pointLight);
        }

        auto signboardModel = new Model("signboard");
        signboardModel->Load("models/cartello.obj");
        signboardModel->SetShader(_shaders["materialShader"]);

        float x = rand() % 150 - 50;
        float z = rand() % 150 - 50;
        float angle = rand() % 360;

        auto signboard = new RacingGame::StaticObject(this, "signboard");
        signboard->SetModel(signboardModel);
        signboard->SetPosition(glm::vec3(x, 0.0f, z));
        signboard->SetRotation(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        signboard->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));

        AddGameObject(signboard);
    }

    void Game::_CreateTrees(Model* treeModel)
    {
        for (int i = 0; i < 60; ++i)
        {
            float x = rand() % 300 - 150;
            float z = rand() % 300 - 150;
            float angle = rand() % 360;

            float diff = 2.5f - 1.0f;
            float scale = (((float) rand() / RAND_MAX) * diff) + 1.0f;

            auto tree = new RacingGame::StaticObject(this, treeModel->GetName() + to_string(i));
            tree->SetModel(treeModel);
            tree->SetPosition(glm::vec3(x, 0.0f, z));
            tree->SetRotation(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            tree->SetScale(glm::vec3(scale, scale, scale));
            AddGameObject(tree);
        }
    }

    void Game::_CreateSkybox()
    {
        vector<string> faces{
                "textures/skybox/right.jpg",
                "textures/skybox/left.jpg",
                "textures/skybox/top.jpg",
                "textures/skybox/bottom.jpg",
                "textures/skybox/front.jpg",
                "textures/skybox/back.jpg"};

        _skybox = Skybox(faces);
    }

    Graphics::Font* Game::GetFont(std::string& fileName)
    {
        auto iter = _fonts.find(fileName);
        if (iter != _fonts.end())
        {
            return iter->second;
        }
        else
        {
            Font* font = new Graphics::Font();
            if (font->Load(fileName))
            {
                _fonts.emplace(fileName, font);
            }
            else
            {
                font->Unload();
                delete font;
                font = nullptr;
            }
            return font;
        }
    }

    void Game::ProcessInput()
    {
        _inputManager->Update();

        if (_inputManager->QuitRequested() ||
            _inputManager->IsKeyDown(KEY_ESCAPE))
        {
            _globalStateManager->Stop();
            return;
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_0))
        {
            _isMouseCaptured = !_isMouseCaptured;
            if (_isMouseCaptured)
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                SDL_CaptureMouse(SDL_TRUE);
            }
            else
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_CaptureMouse(SDL_FALSE);
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_1))
        {
            switch (_cameraMode)
            {
                case 1:_currentCamera = (BasicCamera*) GetGameObjectByName("freeCamera");
                    _cameraMode = 2;
                    break;
                case 2:_currentCamera = (BasicCamera*) GetGameObjectByName("followCamera");
                    _cameraMode = 0;
                    break;
                default:_currentCamera = (BasicCamera*) GetGameObjectByName("minimapCamera");
                    _cameraMode = 1;
                    break;
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_2))
        {
            _isLineMode = !_isLineMode;
            if (_isLineMode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_3))
        {
            _isMenuVisible = !_isMenuVisible;
            if (_isMenuVisible)
            {
                auto menu = _guiStack["speed-gui"];
                menu->SetState(GUIComponentState::Visible);
            }
            else
            {
                auto menu = _guiStack["speed-gui"];
                menu->SetState(GUIComponentState::Hidden);
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_M))
        {
            _isMinimapActive = !_isMinimapActive;
            if (_isMinimapActive)
            {
                _minimap->SetState(GUIComponentState::Visible);
            }
            else
            {
                _minimap->SetState(GUIComponentState::Hidden);
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_U))
        {
            _isSpotLightActive = !_isSpotLightActive;
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_N))
        {
            if (_isWinner)
            {
                _isNight = !_isNight;
                if (_isNight)
                {
                    _isPointLightActive = true;
                    _directionalLight->ambient = glm::vec3(0.15f, 0.15f, 0.15f);
                    _directionalLight->diffuse = glm::vec3(0.15f, 0.15f, 0.15f);
                }
                else
                {
                    _isPointLightActive = false;
                    _directionalLight->ambient = glm::vec3(0.8f, 0.8f, 0.8f);
                    _directionalLight->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
                }
            }
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_P))
        {
            _isPrintInfo = !_isPrintInfo;
        }
    }

    void Game::Update(double elapsedTime)
    {
        _updatingGameObjects = true;
        for (auto gameObject : _gameObjects)
        {
            gameObject->Update(elapsedTime);
        }
        _updatingGameObjects = false;

        auto car = GetGameObjectByName("lambo");
        auto signboard = GetGameObjectByName("signboard");

        if (!_isWinner && glm::distance(car->GetPosition(), signboard->GetPosition()) <= 2.5)
        {
            _isPrintInfo = true;
            _isWinner = true;
        }

        for (auto pendingObject : _pendingGameObjects)
        {
            _gameObjects.emplace_back(pendingObject);
        }
        _pendingGameObjects.clear();

        std::vector<GameObject*> removeGameObjects;
        for (auto gameObject : _gameObjects)
        {
            if (gameObject->GetState() == GameObjectState::Remove)
            {
                removeGameObjects.emplace_back(gameObject);
            }
        }

        for (auto gameObject : removeGameObjects)
        {
            delete gameObject;
        }
    }

    void Game::Render()
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_isMinimapActive)
        {
            _RenderMinimap();
        }

        _RenderScene(0);

        _RenderGUI();

        glUseProgram(0);
    }

    void Game::_RenderMinimap()
    {
        auto oldCamera = _currentCamera;

        _currentCamera = (BasicCamera*) GetGameObjectByName("minimapCamera");

        glBindFramebuffer(GL_FRAMEBUFFER, _minimap->GetFrameBuffer());

        glViewport(0, 0, WINDOW_WIDTH * _minimap->GetRatio(), WINDOW_HEIGHT * _minimap->GetRatio());

        _window->Clear();

        auto materialShader = _shaders["materialShader"];
        auto terrainShader = _shaders["terrainShader"];

        _terrain->Draw(terrainShader);

        for (auto gameObject : _gameObjects)
        {
            gameObject->Draw(materialShader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        _currentCamera = oldCamera;
    }

    void Game::_RenderScene(unsigned int frameBuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Draw mesh components
        // Enable depth buffering/disable alpha blend
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        auto materialShader = _shaders["materialShader"];
        auto terrainShader = _shaders["terrainShader"];

        _terrain->Draw(terrainShader);

        for (auto gameObject : _gameObjects)
        {
            gameObject->Draw(materialShader);
        }

        auto view = glm::mat4(glm::mat3(_currentCamera->GetViewMatrix()));
        _skybox.Use(view, GetProjection());
    }

    void Game::_RenderGUI()
    {
        auto guiShader = _shaders["guiShader"];

        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Graphics::GUIComponent::EnableGUIQuad();

        for (auto&[key, gui] : _guiStack)
        {
            if (gui->GetState() == GUIComponentState::Visible)
            {
                gui->Draw(guiShader);
            }
        }

        Graphics::GUIComponent::DisableGUIQuad();
    }

    void Game::ShutDown()
    {
        _window->Close();
    }

    bool Game::AddGameObject(GameObject* gameObject)
    {
        auto object = GetGameObjectByName(gameObject->GetName());

        if (object)
        {
            std::cerr << "GameObject with name " << gameObject->GetName() << " already exists!" << std::endl;
            return false;
        }

        if (_updatingGameObjects)
        {
            _pendingGameObjects.emplace_back(gameObject);
        }
        else
        {
            _gameObjects.emplace_back(gameObject);
        }

        return true;
    }

    GameObject* Game::GetGameObjectByName(const std::string& gameObjectName)
    {
        for (auto gameObject : _gameObjects)
        {
            if (gameObject->GetName() == gameObjectName)
            {
                return gameObject;
            }
        }

        for (auto pendingObject : _pendingGameObjects)
        {
            if (pendingObject->GetName() == gameObjectName)
            {
                return pendingObject;
            }
        }

        return nullptr;
    }

    BasicCamera* Game::GetCurrentCamera()
    {
        return _currentCamera;
    }

    Graphics::DirectionalLight* Game::GetDirectionalLight()
    {
        return _directionalLight;
    }

    std::unordered_map<std::string, Graphics::GUIComponent*> Game::GetGUIStack()
    {
        return _guiStack;
    }

    void Game::PushGUI(const std::string& name, Graphics::GUIComponent* gui)
    {
        _guiStack.emplace(name, gui);
    }

    std::vector<Graphics::PointLight*> Game::GetPointLights()
    {
        if (!_isPointLightActive)
            return std::vector<Graphics::PointLight*>(0);

        return _pointLights;
    }

    std::vector<Graphics::SpotLight*> Game::GetSpotLights()
    {
        if (!_isSpotLightActive)
            return std::vector<Graphics::SpotLight*>(0);

        return _spotLights;
    }

    glm::mat4 Game::GetProjection()
    {
        float ratio = _window->GetRatio();
        return glm::perspective(Math::DegToRad(FOV), ratio, NEAR_PLANE, FAR_PLANE);
    }

    Window* Game::GetWindow()
    {
        return _window;
    }

    bool Game::IsNight()
    {
        return _isNight;
    }

    bool Game::IsWinner() const
    {
        return _isWinner;
    }

    bool Game::IsPrintInfo()
    {
        return _isPrintInfo;
    }

} // namespace Engine