#include <algorithm>
#include <chrono>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL_image.h>

#include "Game.hpp"

#include "game/Car.hpp"

#include "math/Math.hpp"

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
                _window.Clear();
                Render();
                _window.SwapBuffers();

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

            _window.Close();

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

        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_CaptureMouse(SDL_TRUE);

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

        return true;
    }

    void Game::_SetupOpenGLAttributes() const
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

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
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            cerr << "GLWE Error: " << glewGetErrorString(err) << endl;
        }

        glViewport(0, 0, _window.GetWidth(), _window.GetHeight());

        // cull face with with default settings
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    void Game::StartUp()
    {
        _CreateCamera();
        _CreateTextures();

        const string BASE_SHADER = "basicShader";

        // create shaders
        auto basicShader = new GLSLShader();
        basicShader->Load("shaders/basic.vert", "shaders/basic.frag");
        _shaders.emplace(BASE_SHADER, basicShader);

        auto terrainShader = new GLSLShader();
        terrainShader->Load("shaders/terrain.vert", "shaders/terrain.frag");
        _shaders.emplace("terrainShader", terrainShader);

        auto depthShader = new GLSLShader();
        depthShader->Load("shaders/basic.vert", "shaders/depth.frag");
        _shaders.emplace("depthShader", depthShader);

        // create models
        auto lamboModel = new Model("lambo");
        lamboModel->Load("models/lambo.obj");
        lamboModel->SetShader(basicShader);
        lamboModel->AddTexture("lambDiff", "textures/lamb_diff.jpeg");
        lamboModel->AddTexture("lambSpec", "textures/lamb_spec.jpeg");

        auto car = new RacingGame::Car(this, "lambo");
        car->SetModel(lamboModel);
        AddGameObject(car);

        _terrain = new Terrain(this, "terrain1");
        _terrain->SetShader(terrainShader);

        _CreateLights();

    }

    void Game::_CreateCamera()
    {
        auto startPosition = glm::vec3(0.0f, 5.0f, 5.0f);

        auto camera = new Camera(this, "camera1");

        camera->SetPosition(startPosition);
        camera->SetMovimentSpeed(10.0f);

        _currentCamera = camera;

        AddGameObject(camera);
    }

    void Game::_CreateTextures()
    {

        // create skybox
        vector<string> faces{
                "textures/skybox/right.jpg",
                "textures/skybox/left.jpg",
                "textures/skybox/top.jpg",
                "textures/skybox/bottom.jpg",
                "textures/skybox/front.jpg",
                "textures/skybox/back.jpg"
        };

        _skybox = Skybox(faces);

        auto plainTexture = new Texture();
        plainTexture->Load("textures/plain.png");
        _textures.emplace("plain", plainTexture);
    }

    void Game::_CreateLights()
    {
        _directionalLight = new DirectionalLight();
//        _directionalLight->direction = -glm::vec3(5.0f, -5.0f, 0.619f);
//        _directionalLight->ambient = glm::vec3(0.271f, 0.338f, 0.391f);
//        _directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 0.655f);
//        _directionalLight->specular = glm::vec3(1.0f);
        _directionalLight->direction = -glm::vec3(5.0f, -5.0f, 0.619f);
        _directionalLight->ambient = glm::vec3(0.6f);
        _directionalLight->diffuse = glm::vec3(0.8f);
        _directionalLight->specular = glm::vec3(1.0f);

        auto pointLight = new PointLight();
        pointLight->position = glm::vec3(0.0f, 6.0f, 0.0f);
        pointLight->ambient = glm::vec3(0.05f, 0.2f, 0.4f);
        pointLight->diffuse = glm::vec3(0.1f, 0.4f, 0.8f);
        pointLight->specular = glm::vec3(0.2f);

        pointLight->constant = 1.0f;
        pointLight->linear = 0.09f;
        pointLight->quadratic = 0.032f;

        _pointLights.push_back(pointLight);
    }

    void Game::ProcessInput()
    {
        // process input
        _inputManager->Update();

        if (_inputManager->QuitRequested() ||
            _inputManager->IsKeyDown(KEY_ESCAPE))
        {
            _globalStateManager->Stop();
            return;
        }

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_M))
        {
            if (_isMouseCaptured)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            _isMouseCaptured = !_isMouseCaptured;
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
        _terrain->Draw();

        for (auto gameObject : _gameObjects)
        {
            gameObject->Draw();
        }

         auto view = glm::mat4(glm::mat3(_currentCamera->GetViewMatrix()));
        _skybox.Use(view, GetProjection());

        glUseProgram(0);
    }

    void Game::ShutDown()
    {
        _window.Close();
    }


    bool Game::AddGameObject(GameObject* gameObject)
    {
        auto object = _GetGameObjectByName(gameObject->GetName());

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

    GameObject* Game::_GetGameObjectByName(const std::string& gameObjectName)
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

    Camera* Game::GetCurrentCamera()
    {
        return _currentCamera;
    }

    Graphics::DirectionalLight* Game::GetDirectionalLight()
    {
        return _directionalLight;
    }

    std::vector<Graphics::PointLight*> Game::GetPointLight()
    {
        return _pointLights;
    }

    glm::mat4  Game::GetProjection()
    {
        float ratio = _window.GetRatio();
        return glm::perspective(Math::DegToRad(FOV), ratio, NEAR_PLANE, FAR_PLANE);

    }


} // namespace Engine