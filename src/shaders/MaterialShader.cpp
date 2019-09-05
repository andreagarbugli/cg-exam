#include <string>

#include "engine/Game.hpp"

#include "MaterialShader.hpp"

#include "graphics/Model.hpp"

#include "math/Math.hpp"

using namespace std;

namespace Graphics
{
    MaterialShader::MaterialShader(Engine::Game* game, string name) :
            BasicShader(game, name)
    {
        _shaderProgram = new ShaderProgram();
        _shaderProgram->Load("shaders/basic.vert", "shaders/basic.frag");
    }

    void MaterialShader::Use()
    {
        BasicShader::Use();

        auto camera = _game->GetCurrentCamera();
        auto projection = _game->GetProjection();

        _shaderProgram->SetUniformMatrix4("view", camera->GetViewMatrix());
        _shaderProgram->SetUniformMatrix4("projection", projection);

        _shaderProgram->SetUniformVector3("viewPos", camera->GetPosition());

        // dir light
        auto directionalLight = _game->GetDirectionalLight();

        _shaderProgram->SetUniformVector3("dirLight.ambient", directionalLight->ambient);
        _shaderProgram->SetUniformVector3("dirLight.diffuse", directionalLight->diffuse);
        _shaderProgram->SetUniformVector3("dirLight.specular", directionalLight->specular);
        _shaderProgram->SetUniformVector3("dirLight.direction", directionalLight->direction);

        auto pointLights = _game->GetPointLights();

        _shaderProgram->SetUniformInt("numPointLights", pointLights.size());

        for (int i = 0; i < pointLights.size(); i++)
        {
            auto pointLight = pointLights[i];

            string str = "pointLigths[" + to_string(i);

            _shaderProgram->SetUniformVector3(str + "].position", pointLight->position);

            _shaderProgram->SetUniformVector3(str + "].ambient", pointLight->ambient);
            _shaderProgram->SetUniformVector3(str + "].diffuse", pointLight->diffuse);
            _shaderProgram->SetUniformVector3(str + "].specular", pointLight->specular);

            _shaderProgram->SetUniformFloat(str + "].constant", pointLight->constant);
            _shaderProgram->SetUniformFloat(str + "].linear", pointLight->linear);
            _shaderProgram->SetUniformFloat(str + "].quadratic", pointLight->quadratic);
        }

        auto spotLights = _game->GetSpotLights();

        _shaderProgram->SetUniformInt("numSpotLights", spotLights.size());

        for (int i = 0; i < spotLights.size(); i++)
        {
            auto spotLight = spotLights[i];

            string str = "spotLigths[" + to_string(i);

            _shaderProgram->SetUniformVector3(str + "].direction", spotLight->direction);
            _shaderProgram->SetUniformVector3(str + "].position", spotLight->position);

            _shaderProgram->SetUniformVector3(str + "].ambient", spotLight->ambient);
            _shaderProgram->SetUniformVector3(str + "].diffuse", spotLight->diffuse);
            _shaderProgram->SetUniformVector3(str + "].specular", spotLight->specular);

            _shaderProgram->SetUniformFloat(str + "].constant", spotLight->constant);
            _shaderProgram->SetUniformFloat(str + "].linear", spotLight->linear);
            _shaderProgram->SetUniformFloat(str + "].quadratic", spotLight->quadratic);

            _shaderProgram->SetUniformFloat(str + "].cutOff", Math::Cos(Math::DegToRad(spotLight->cutOff)));
            _shaderProgram->SetUniformFloat(str + "].outerCutOff", Math::Cos(Math::DegToRad(spotLight->outerCutOff)));
        }
    }

    void MaterialShader::SetModel(Graphics::SubModel* model)
    {
        BasicShader::SetModel(model);

        auto material = model->material;

        if (model->hasTextures)
        {
            model->parent->GetTexture(material->mapKd)->Enable(0);
            model->parent->GetTexture(material->mapKd)->Enable(1);

            _shaderProgram->SetUniformInt("material.diffuseTex", 0);
            _shaderProgram->SetUniformInt("material.specularTex", 1);
            _shaderProgram->SetUniformInt("material.hasTextures", GL_TRUE);
        }
        else
        {
            _shaderProgram->SetUniformVector3("material.ambient", material->ka);
            _shaderProgram->SetUniformVector3("material.diffuse", material->kd);
            _shaderProgram->SetUniformVector3("material.specular", material->ks);
            _shaderProgram->SetUniformInt("material.hasTextures", GL_FALSE);
        }

        _shaderProgram->SetUniformFloat("material.shininess", material->ns);
    }
}
