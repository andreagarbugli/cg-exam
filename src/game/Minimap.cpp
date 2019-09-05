#include "Minimap.hpp"

#include "engine/Game.hpp"

namespace RacingGame
{
    Minimap::Minimap(Engine::Game* game, const std::string& name)
            : GUIComponent(game, name)
    {
    }

    bool Minimap::CreateMinimapTarget()
    {
        auto window = _game->GetWindow();

        auto width = window->GetWidth() * _ratio;
        auto height = window->GetHeight() * _ratio;

        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        _texture = new Graphics::Texture();
        _texture->CreateForRendering(width, height, GL_RGB);

        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

        glGenRenderbuffers(1, &_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, _renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glDeleteFramebuffers(1, &_framebuffer);
            _texture->Unload();
            delete _texture;
            _texture = nullptr;

            return false;
        }

        return true;
    }

    void Minimap::Draw(Graphics::BasicShader* shader)
    {
        _texture->Enable(0);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(_position, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(_scale, 1.0f));

        shader->SetModelMatrix(modelMatrix);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    unsigned int Minimap::GetFrameBuffer()
    {
        return _framebuffer;
    }

    float Minimap::GetRatio() const
    {
        return _ratio;
    }

    void Minimap::SetRatio(float ratio)
    {
        _ratio = ratio;
    }
}
