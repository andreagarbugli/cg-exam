#ifndef ENGINE_MODEL_HPP
#define ENGINE_MODEL_HPP

#include <map>
#include <string>

#include "GLSLShader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Graphics
{

    class Model
    {
    private:

        std::map<std::string, Mesh*> _meshes;
        std::map<std::string, Texture*> _textures;

        GLSLShader* _shader;

        std::string _name;

        unsigned int _drawingOder;

    public:

        static bool sortByDrawingOrder(Model* m1, Model* m2);

        explicit Model(std::string name);

        void Draw();

        bool Load(std::string objfilePath);

        void AddTexture(std::string name, std::string filePath);


        unsigned int GetDrawingOrder() const;

        void SetDrawingOrder(unsigned int order);

        GLSLShader* GetShader();

        void SetShader(GLSLShader* shader);

        Texture* GetTexture(std::string name);
    };

}

#endif //ENGINE_MODEL_HPP
