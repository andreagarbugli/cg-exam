#ifndef ENGINE_SHADER_HPP
#define ENGINE_SHADER_HPP

#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace Graphics
{

    class GLSLShader
    {
    private:
        unsigned int _vertexShader;
        unsigned int _fragShader;
        unsigned int _shaderProgram;

    public:
        GLSLShader() = default;

        ~GLSLShader();

        bool Load(const std::string& vertName, const std::string& fragName);

        void Clear();

        void Disable();

        void Enable();

        int GetUniformLocation(const std::string& uniformName);

        void SetUniformMatrix4(const std::string& name, glm::mat4 value);

        void SetUniformVector4(const std::string& name, glm::vec4 value);

        void SetUniformVector3(const std::string& name, glm::vec3 value);

        void SetUniformFloat(const std::string& name, float value);

        void SetUniformInt(const std::string& name, int value);

    private:
        bool _CompileShader(const std::string& fileName,
                            GLenum shaderType, unsigned int& shader);

        bool _IsCompiled(unsigned int shader);

        bool _IsValidProgram();
    };

}

#endif //ENGINE_SHADER_HPP
