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

    void Model::Draw(BasicShader* shader)
    {
        for (auto const& meshIt : _meshes)
        {
            auto subModel = meshIt.second;

            _shader->SetModel(subModel);

            subModel->mesh->Use();
        }
    }

    bool Model::Load(std::string objfilePath)
    {
        Utils::ObjFileLoader loader;

        if (loader.LoadObj(objfilePath, true))
        {
            auto rawMeshes = loader.GetMeshes();
            auto materials = loader.GetMaterials();

            for (auto& material : materials)
            {
                _materials.emplace(material->name, material);
            }

            for (auto& rawMesh : rawMeshes)
            {
                cout << "Numero of vertices: " << rawMesh->vertices.size() << endl;
                auto mesh = new Mesh(*rawMesh);
                auto material = _materials[rawMesh->materialName];

                auto subModel = new SubModel{
                        this,
                        mesh,
                        material,
                        false
                };

                if (!material->mapKs.empty() &&
                    !material->mapKd.empty())
                {
                    _AddTexture(material->mapKd);
                    _AddTexture(material->mapKs);
                    subModel->hasTextures = true;
                }

                _meshes.emplace(rawMesh->name, subModel);
            }

        }
        else
        {
            cerr << "Failed to load model: " << objfilePath << endl;
            return false;
        }

        return true;
    }

    void Model::_AddTexture(std::string filePath)
    {
        auto texture = new Texture();
        texture->Load(filePath);
        _textures.emplace(filePath, texture);
    }

    unsigned int Model::GetDrawingOrder() const
    {
        return _drawingOder;
    }

    void Model::SetDrawingOrder(unsigned int order)
    {
        _drawingOder = order;
    }

    void Model::SetShader(BasicShader* shader)
    {
        _shader = shader;
    }

    Texture* Model::GetTexture(const string name)
    {
        return _textures[name];
    }

    const string& Model::GetName() const
    {
        return _name;
    }
}
