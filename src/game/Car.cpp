#include <iostream>

#include "Car.hpp"

#include "engine/Game.hpp"

using namespace std;
using namespace Engine;

namespace RacingGame
{
    void Car::Draw(Graphics::BasicShader* shader)
    {
        GameObject::Draw(shader);

        _model->Draw(shader);
    }

    void Car::Update(double deltaTime)
    {
        GameObject::Update(deltaTime);

        float steerCos = Math::Cos(Math::DegToRad(_rotationAngle));
        float steerSin = Math::Sin(Math::DegToRad(_rotationAngle));

        _velocityVectorWorld = glm::vec3(steerCos * _velocityVector.x - steerSin * _velocityVector.z,
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
            _velocityVectorWorld.z += accFactor * (float) deltaTime;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_K))
        {
            _velocityVectorWorld.z -= accFactor * (float) deltaTime;
        }

        _velocityVectorWorld *= _frictionVector;

        _rotationAngle = _rotationAngle - (_velocityVectorWorld.z * _grip) * steerDir;

        _velocityVector = glm::vec3(steerCos * _velocityVectorWorld.x + steerSin * _velocityVectorWorld.z,
                                    _velocityVectorWorld.y,
                                    -steerSin * _velocityVectorWorld.x + steerCos * _velocityVectorWorld.z);

        _position += _velocityVector;

        for (int i = 0; i < _game->GetSpotLights().size(); ++i)
        {
            auto spotLight = _game->GetSpotLights()[i];

            auto facingRad = Math::DegToRad(_rotationAngle);
            spotLight->direction.x = Math::Sin(facingRad);
            spotLight->direction.z = Math::Cos(facingRad);

            glm::mat4 lightModel(1.0);
            lightModel = glm::rotate(lightModel, facingRad, _up);

            glm::vec3 relativePosition;
            if (i == 0)
            {
                relativePosition = lightModel * glm::vec4(lightRPosition, 1.0f);
            }
            else
            {
                relativePosition = lightModel * glm::vec4(lightLPosition, 1.0f);
            }

            spotLight->position = _position + relativePosition;
        }
    }

    float Car::GetVelocity()
    {
        return glm::length(_velocityVector);
    }
}