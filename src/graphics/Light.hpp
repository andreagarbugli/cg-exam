#ifndef ENGINE_LIGHT_HPP
#define ENGINE_LIGHT_HPP

#include <glm/glm.hpp>

namespace Graphics
{
    struct DirectionalLight
    {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };
}

#endif //ENGINE_LIGHT_HPP
