//
// Created by garbu on 23.08.19.
//

#include "GUIComponent.hpp"

#include <utility>

#include "engine/Game.hpp"

namespace Graphics
{
    GUIComponent::GUIQuad::GUIQuad()
    {
        float quadVertices[] = {
                -1.0f, 1.0f,  // top left
                -1.0f, -1.0f,  // bottom left
                1.0f, 1.0f,  // top right
                1.0f, -1.0f,  // bottom right
        };

        glGenVertexArrays(1, &_vertexArrayObject);
        glBindVertexArray(_vertexArrayObject);

        glGenBuffers(1, &_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                     &quadVertices, GL_STATIC_DRAW);
    }

    void GUIComponent::GUIQuad::Enable()
    {
        glBindVertexArray(_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);

        // positions
        glEnableVertexAttribArray(0);
        // index, size, type, normalized, stride, pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    }

    void GUIComponent::GUIQuad::Disable()
    {
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    GUIComponent::GUIComponent(Engine::Game* game, const std::string& name)
            : _game{game},
              _name{name},
              _position{0.0f, 0.0f},
              _scale{1.0f, 1.0f},
              _title{nullptr},
              _state{GUIComponentState::Visible}
    {
        _game->PushGUI(name, this);

        auto fontFileName = std::string("assets/Carlito-Regular.ttf");
        _font = _game->GetFont(fontFileName);
    }

    GUIComponent::~GUIComponent()
    {
        if (_title)
        {
            _title->Unload();
            delete _title;
        }
    }

    void GUIComponent::Draw(BasicShader* shader)
    {
        shader->Use();

        // Draw title (if exists)
        if (_title)
        {
            DrawTextTexture(shader, _title, _titlePosition);
        }
    }

    void GUIComponent::DrawTextTexture(BasicShader* shader, class Texture* texture,
                                       const glm::vec2& offset, float scale)
    {
        texture->Enable(0);

        auto scaleX = (texture->GetHeight() / (float) _game->GetWindow()
                                                           ->GetHeight()) * scale;
        auto scaleY = (texture->GetWidth() / (float) _game->GetWindow()
                                                          ->GetWidth()) * scale;

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(offset, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, 1.0f));

        shader->SetModelMatrix(modelMatrix);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    std::string GUIComponent::GetName() const
    {
        return _name;
    }

    const glm::vec2& GUIComponent::GetPosition() const
    {
        return _position;
    }

    void GUIComponent::SetPosition(const glm::vec2& position)
    {
        _position = position;
    }

    const glm::vec2& GUIComponent::GetScale() const
    {
        return _scale;
    }

    void GUIComponent::SetScale(const glm::vec2& scale)
    {
        _scale = scale;
    }

    void GUIComponent::SetTitle(const std::string& text,
                                const glm::vec3& color,
                                int pointSize)
    {
        // Clear out previous title texture if it exists
        if (_title)
        {
            _title->Unload();
            delete _title;
            _title = nullptr;
        }
        _title = _font->RenderText(text, color, pointSize);
    }

    GUIComponentState GUIComponent::GetState() const
    {
        return _state;
    }

    void GUIComponent::SetState(GUIComponentState state)
    {
        _state = state;
    }
}
