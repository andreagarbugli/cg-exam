#ifndef ENGINE_SHADERPROGRAM_HPP
#define ENGINE_SHADERPROGRAM_HPP

#include <string>

#include <GL/gl3w.h>

#include <glm/glm.hpp>

namespace Graphics
{

    class ShaderProgram
    {
    private:
        unsigned int _vertexShader;
        unsigned int _fragShader;
        unsigned int _shaderProgram;

    public:
        ShaderProgram() = default;

        ~ShaderProgram();

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

        static bool _IsCompiled(unsigned int shader);

        bool _IsValidProgram();
    };

} // namespace Graphics

#endif //ENGINE_SHADERPROGRAM_HPP
