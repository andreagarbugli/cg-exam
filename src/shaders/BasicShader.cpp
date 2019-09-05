#include "BasicShader.hpp"

using namespace std;

namespace Graphics
{
    void BasicShader::Use()
    {
        _shaderProgram->Enable();
    }

    string BasicShader::GetName()
    {
        return _name;
    }

    ShaderProgram* BasicShader::GetShaderProgram()
    {
        return _shaderProgram;
    }

    void BasicShader::SetModel(SubModel* model)
    {

    }

    void BasicShader::SetModelMatrix(glm::mat4 modelMatrix)
    {
        _shaderProgram->SetUniformMatrix4("model", modelMatrix);
    }

    void BasicShader::UnUse()
    {
        _shaderProgram->Disable();
    }
}