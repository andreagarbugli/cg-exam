//
// Created by garbu on 24.08.19.
//

#ifndef ENGINE_MENU_HPP
#define ENGINE_MENU_HPP

#include <utility>
#include <sstream>

#include "graphics/GUIComponent.hpp"

namespace RacingGame
{
    class Menu : public Graphics::GUIComponent
    {
    private:

        const std::string SPEED_TEXT = "Speed: ";

        const glm::vec3 _brightTextColor = glm::vec3(0.9f, 0.9f, 0.9f);
        const glm::vec3 _darkTextColor = glm::vec3(0.1f, 0.1f, 0.1f);

        const int _fontSize = 20;

        Graphics::Texture* _gameGoalTexture;

    public:
        Menu(Engine::Game* game, const std::string& name)
                : GUIComponent(game, name)
        {
            glm::vec3 color = _darkTextColor;

            _gameGoalTexture = _font->RenderText("Cerca il cartello con la foto!",
                                                 color, 12);

            if (_game->IsNight())
            {
                color = _brightTextColor;
            }

            SetTitle(SPEED_TEXT, color, _fontSize);
        }

        void Draw(Graphics::BasicShader* shader) override
        {
            shader->Use();

            glm::vec3 color = _darkTextColor;

            if (_game->IsNight())
            {
                color = _brightTextColor;
            }

            std::string goalText = "Cerca il cartello con la foto!";
            int goalTextSize = 12;
            auto goalTextPosition = glm::vec2(-0.75f, 0.75f);

            if (_game->IsWinner())
            {
                goalText = "Hai Vinto!";
                goalTextSize = 20;
                goalTextPosition = glm::vec2(0.0f, 0.75f);
            }

            if ( _game->IsPrintInfo())
            {
                _gameGoalTexture = _font->RenderText(goalText.c_str(), color, goalTextSize);
                DrawTextTexture(shader, _gameGoalTexture, goalTextPosition, 0.6);
            }

            _titlePosition = _position;
            auto car = dynamic_cast<Car*>(_game->GetGameObjectByName("lambo"));
            auto speed = std::to_string(int(car->GetVelocity() * 3000));

            SetTitle(SPEED_TEXT + speed + "km/h", color, _fontSize);

            GUIComponent::Draw(shader);
        }
    };
}

#endif //ENGINE_MENU_HPP
