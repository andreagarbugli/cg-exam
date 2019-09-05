//
// Created by garbu on 23.08.19.
//

#ifndef ENGINE_GUICOMPONENT_HPP
#define ENGINE_GUICOMPONENT_HPP

#include "Font.hpp"

#include "shaders/BasicShader.hpp"

namespace Engine
{
    class Game;
}

namespace Graphics
{
    enum GUIComponentState
    {
        Visible,
        Hidden,
    };

    class GUIComponent
    {
        class GUIQuad
        {
        private:

            unsigned int _vertexArrayObject;
            unsigned int _vertexBufferObject;

        public:

            GUIQuad();

            static GUIQuad* GetInstance()
            {
                static auto* _instance = new GUIQuad();
                return _instance;
            }

            void Enable();

            void Disable();

        };

    protected:

        Engine::Game* _game;

        std::string _name;

        Texture* _texture{};
        glm::vec2 _position;
        glm::vec2 _scale;

        Font* _font;
        Texture* _title;
        glm::vec2 _titlePosition{};

        GUIComponentState _state;

    public:

        GUIComponent(Engine::Game* game, const std::string& name);

        ~GUIComponent();

        static void EnableGUIQuad()
        {
            GUIQuad::GetInstance()->Enable();
        };

        static void DisableGUIQuad()
        {
            GUIQuad::GetInstance()->Disable();
        };

        virtual void Draw(BasicShader* shader);

        void DrawTextTexture(BasicShader* shader, class Texture* texture,
                             const glm::vec2& offset = glm::vec2(0.0f),
                             float scale = 1.0f);

        void SetTitle(const std::string& text,
                      const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
                      int pointSize = 30);

        std::string GetName() const;

        const glm::vec2& GetPosition() const;

        void SetPosition(const glm::vec2& position);

        const glm::vec2& GetScale() const;

        void SetScale(const glm::vec2& scale);

        GUIComponentState GetState() const;

        void SetState(GUIComponentState state);
    };
}

#endif //ENGINE_GUICOMPONENT_HPP
