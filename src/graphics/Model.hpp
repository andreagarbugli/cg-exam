#ifndef ENGINE_MODEL_HPP
#define ENGINE_MODEL_HPP

#include <map>
#include <string>

#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

#include "shaders/BasicShader.hpp"

namespace Graphics
{
    class Model;

    struct SubModel
    {
        Model* parent;
        Mesh* mesh;
        Utils::Material* material;
        bool hasTextures;
    };

    class Model
    {
    private:

        std::map<std::string, SubModel*> _meshes;
        std::map<std::string, Texture*> _textures;
        std::map<std::string, Utils::Material*> _materials;

        BasicShader* _shader;

        std::string _name;

        unsigned int _drawingOder;

    public:

        static bool sortByDrawingOrder(Model* m1, Model* m2);

        Model(std::string name);

        void Draw(BasicShader* shader);

        bool Load(std::string objfilePath);

        void AddTexture(std::string name, std::string filePath);

        void _AddTexture(std::string filePath);

        unsigned int GetDrawingOrder() const;

        void SetDrawingOrder(unsigned int order);

        BasicShader* GetShader();

        const std::string& GetName() const;

        void SetShader(BasicShader* shader);

        Texture* GetTexture(std::string name);
    };

}

#endif //ENGINE_MODEL_HPP
