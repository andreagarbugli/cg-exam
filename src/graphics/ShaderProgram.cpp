#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"


namespace Graphics
{
    bool ShaderProgram::Load(const std::string& vertName, const std::string& fragName)
    {
        if (!_CompileShader(vertName, GL_VERTEX_SHADER, _vertexShader) ||
            !_CompileShader(fragName, GL_FRAGMENT_SHADER, _fragShader))
        {
            return false;
        }

        _shaderProgram = glCreateProgram();

        glAttachShader(_shaderProgram, _vertexShader);
        glAttachShader(_shaderProgram, _fragShader);

        glLinkProgram(_shaderProgram);

        glDeleteShader(_fragShader);
        glDeleteShader(_vertexShader);

        return _IsValidProgram();
    }


    bool ShaderProgram::_CompileShader(const std::string& fileName, GLenum shaderType, unsigned int& shader)
    {
        std::ifstream shaderFile(fileName);

        if (shaderFile.is_open())
        {
            std::stringstream sstream;
            sstream << shaderFile.rdbuf();

            shaderFile.close();

            std::string contents = sstream.str();
            const char* contentsChar = contents.c_str();

            shader = glCreateShader(shaderType);

            glShaderSource(shader, 1, &(contentsChar), nullptr);
            glCompileShader(shader);

            if (!_IsCompiled(shader))
            {
                std::cerr << "Failed to compile shader: " << fileName << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "Shader file " << fileName << " not found!" << std::endl;
            return false;
        }

        return true;
    }

    bool ShaderProgram::_IsCompiled(unsigned int shader)
    {
        int status;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {
            int infoLogLenght;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLenght);

            char* infoLog = new char[infoLogLenght];
            glGetShaderInfoLog(shader, infoLogLenght, nullptr, infoLog);

            std::cerr << "Compile log: " << infoLog << std::endl;

            delete[] infoLog;

            return false;
        }

        return true;
    }

    bool ShaderProgram::_IsValidProgram()
    {
        int status;
        glValidateProgram(_shaderProgram);
        glGetProgramiv(_shaderProgram, GL_VALIDATE_STATUS, &status);

        if (status == GL_FALSE)
        {
            int infoLogLenght;
            glGetShaderiv(_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLenght);

            char* infoLog = new char[infoLogLenght];
            glGetProgramInfoLog(_shaderProgram, infoLogLenght, nullptr, infoLog);

            std::cerr << "Link log: " << infoLog << std::endl;

            delete[] infoLog;

            return false;
        }

        return true;
    }

    void ShaderProgram::Clear()
    {
        glDeleteProgram(_shaderProgram);
    }

    void ShaderProgram::Disable()
    {
        glUseProgram(_shaderProgram);
    }

    void ShaderProgram::Enable()
    {
        glUseProgram(_shaderProgram);
    }

    int ShaderProgram::GetUniformLocation(const std::string& uniformName)
    {
        return glGetUniformLocation(_shaderProgram, uniformName.c_str());
    }

    void ShaderProgram::SetUniformMatrix4(const std::string& name, glm::mat4 value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void ShaderProgram::SetUniformVector4(const std::string& name, glm::vec4 value)
    {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }

    void ShaderProgram::SetUniformVector3(const std::string& name, glm::vec3 value)
    {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }

    void ShaderProgram::SetUniformFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetUniformInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
}
