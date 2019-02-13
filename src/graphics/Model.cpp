//
// Created by andrea on 08/02/19.
//

#include <iostream>

#include "Model.hpp"

using namespace std;

namespace Graphics
{
    bool Model::sortByDrawingOrder(Model* m1, Model* m2)
    {
        return m1->GetDrawingOrder() < m2->GetDrawingOrder();
    }

    Model::Model(string name) :
            _name{ name },
            _drawingOder{ 0 }
    {
    }

    void Model::Draw()
    {
        for (auto const& mesh : _meshes)
        {
            mesh.second->Use();
        }
    }

    bool Model::Load(std::string objfilePath)
    {
        Utils::ObjFileLoader loader;

        if (loader.LoadObj(objfilePath, true))
        {
            auto rawMeshes = loader.GetMeshes();

            for (auto& rawMesh : rawMeshes)
            {
                cout << "Numero of vertices: " << rawMesh->vertices.size() << endl;
                auto mesh = new Mesh(*rawMesh);
                _meshes.emplace(rawMesh->name, mesh);
            }
        }
        else
        {
            cerr << "Failed to load model: " << objfilePath << endl;
            return false;
        }
    }

    void Model::AddTexture(std::string name, std::string filePath)
    {
        auto texture = new Texture();
        texture->Load(filePath);
        _textures.emplace(name, texture);
    }

    unsigned int Model::GetDrawingOrder() const
    {
        return _drawingOder;
    }

    void Model::SetDrawingOrder(unsigned int order)
    {
        _drawingOder = order;
    }

    GLSLShader* Model::GetShader()
    {
        return _shader;
    }

    void Model::SetShader(GLSLShader* shader)
    {
        _shader = shader;
    }

    Texture* Model::GetTexture(const string name)
    {
        return _textures[name];
    }

}
