#ifndef ENGINE_SKYBOX_HPP
#define ENGINE_SKYBOX_HPP

#include <vector>
#include <string>

#include "GLSLShader.hpp"
#include "CubeMap.hpp"

namespace Graphics
{
    class Skybox
    {

    private:
        CubeMap* _cubemap;
        GLSLShader* _shader;

        unsigned int _skyboxVAO;
        unsigned int _skyboxVBO;


    public:
        Skybox() = default;

        explicit Skybox(const std::vector<std::string>& fileNames);

        void Use(glm::mat4 view, glm::mat4 projection);
    };

}

#endif //ENGINE_SKYBOX_HPP
