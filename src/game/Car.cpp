#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "Car.hpp"

#include "engine/Game.hpp"

using namespace std;
using namespace Engine;

namespace RacingGame
{
    void Car::Draw()
    {
        GameObject::Draw();

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, _position);
        model = glm::rotate(model, Math::DegToRad(_facing), _up);

        auto shader = _model->GetShader();
        shader->Enable();

        auto camera = _game->GetCurrentCamera();
        auto projection = _game->GetProjection();

        shader->SetUniformMatrix4("model", model);
        shader->SetUniformMatrix4("view", camera->GetViewMatrix());
        shader->SetUniformMatrix4("projection", projection);

        auto color = glm::vec4(1.0f);
        shader->SetUniformVector4("objectColor", color);

        shader->SetUniformVector3("viewPos", camera->GetPosition());

        _model->GetTexture("lambDiff")->Enable(0);
        _model->GetTexture("lambSpec")->Enable(1);

        shader->SetUniformInt("material.diffuse", 0);
        shader->SetUniformInt("material.specular", 1);

        float shininess = 64.0;
        shader->SetUniformFloat("material.shininess", shininess);

        // dir light
        auto directionalLight = _game->GetDirectionalLight();

        shader->SetUniformVector3("dirLight.ambient", directionalLight->ambient);
        shader->SetUniformVector3("dirLight.diffuse", directionalLight->diffuse);
        shader->SetUniformVector3("dirLight.specular", directionalLight->specular);
        shader->SetUniformVector3("dirLight.direction", directionalLight->direction);

        for (int i = 0; i < _game->GetPointLight().size(); i++)
        {
            auto pointLight = _game->GetPointLight()[i];

            shader->SetUniformVector3("pointLigths[0].position", pointLight->position);

            shader->SetUniformVector3("pointLigths[0].ambient", pointLight->ambient);
            shader->SetUniformVector3("pointLigths[0].diffuse", pointLight->diffuse);
            shader->SetUniformVector3("pointLigths[0].specular", pointLight->specular);

            shader->SetUniformFloat("pointLigths[0].constant", pointLight->constant);
            shader->SetUniformFloat("pointLigths[0].linear", pointLight->linear);
            shader->SetUniformFloat("pointLigths[0].quadratic", pointLight->quadratic);
        }

        _model->Draw();

//        shader->SetUniformFloat("light.cutOff", Math::Cos(Math::DegToRad(12.5f)));
//        shader->SetUniformFloat("light.outerCutOff", Math::Cos(Math::DegToRad(17.5f)));

    }

    void Car::Update(double deltaTime)
    {
        GameObject::Update(deltaTime);


        float steerCos = Math::Cos(Math::DegToRad(_facing));
        float steerSin = Math::Sin(Math::DegToRad(_facing));

        glm::vec3 velocityVectorWorld = glm::vec3(steerCos * _velocityVector.x - steerSin * _velocityVector.z,
                                                  _velocityVector.y,
                                                  steerSin * _velocityVector.x + steerCos * _velocityVector.z);

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_J))
        {
            steerDir -= steerFactor;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_L))
        {
            steerDir += steerFactor;
        }
        steerDir *= steerFriction;

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_I))
        {
            velocityVectorWorld.z += accFactor * (float) deltaTime;;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_K))
        {
            velocityVectorWorld.z -= accFactor * (float) deltaTime;;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_SPACE))
        {
            if (!_isInAir)
            {
                velocityVectorWorld.y = 0.01f;
                _isInAir = true;
            }
        }

        velocityVectorWorld *= _frictionVector;
        velocityVectorWorld.y += GRAVITY * (float) deltaTime;

        _facing = _facing - (velocityVectorWorld.z * _grip) * steerDir;

        _velocityVector = glm::vec3(steerCos * velocityVectorWorld.x + steerSin * velocityVectorWorld.z,
                                    velocityVectorWorld.y,
                                    -steerSin * velocityVectorWorld.x + steerCos * velocityVectorWorld.z);

        if (_inputManager->IsKeyDown(KeyboardKey::KEY_P))
            printf("Velocity vector: (%f, %f, %f)\n", _velocityVector.x, _velocityVector.y, _velocityVector.z);

        _position += _velocityVector;

        if (_position.y < TERRAIN_HEIGHT)
        {
            _isInAir = false;
            _position.y = TERRAIN_HEIGHT;
        }
    }
}