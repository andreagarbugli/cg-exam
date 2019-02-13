#include <fstream>
#include <iostream>

#include "ObjFileLoader.hpp"

using namespace std;

namespace Utils
{
    ObjFileLoader::ObjFileLoader() :
            _hasNormals{ false },
            _isIndexed{ false },
            _numberObjs{ 0 }
    {

    }

    bool ObjFileLoader::LoadObj(string fileName, bool indexed)
    {
        _isIndexed = indexed;

        if (!_ReadObjFile(fileName))
        {
            cerr << "Failed to read the obj file: " << fileName << endl;
            return false;
        }

        if (!_mtlFileName.empty())
        {
            _ReadMtlFile();
        }

        return true;
    }

    bool ObjFileLoader::_ReadObjFile(std::string fileName)
    {
        ifstream fileStream(fileName, ios::in);

        if (!fileStream.is_open())
        {
            cerr << "Failed to open the obj file: " << fileName << endl;
            return false;
        }

        string firstLineWord;
        string line;

        while (!fileStream.eof())
        {
            getline(fileStream, line);

            vector<string> params = _SplitLine(line, " ");

            if (params.empty())
                continue;

            string lineType = params[0];

            if (lineType == "#")
                continue;

            if (lineType == "v")
                _AddVertexCoord(params);

            if (lineType == "vt")
                _AddTexCoord(params);

            if (lineType == "vn")
                _AddNormal(params);

            if (lineType == "f")
                _AddFaceElement(params);

            if (lineType == "o")
                _AddObject(params);

//            if (lineType == "usemtl")
//                _UseNewMaterial(params);

            if (lineType == "mtllib")
                _mtlFileName = params[1];
        }

        _CompleteRawMesh();

        fileStream.close();

        return true;
    }

    vector<string> ObjFileLoader::_SplitLine(string str, string delim)
    {
        vector<string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == string::npos)
            {
                pos = str.length();
            }

            string token = str.substr(prev, pos - prev);
            if (!token.empty())
            {
                tokens.push_back(token);
            }

            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());

        return tokens;
    }

    void ObjFileLoader::_AddVertexCoord(vector<string> vectorParams)
    {
        float x, y, z;

        x = std::stof(vectorParams[1]);
        y = std::stof(vectorParams[2]);
        z = std::stof(vectorParams[3]);

        glm::vec3 vertex{ x, y, z };
        _vertices.push_back(vertex);
    }

    void ObjFileLoader::_AddTexCoord(std::vector<std::string> texParams)
    {
        float u, v;

        u = std::stof(texParams[1]);
        v = std::stof(texParams[2]);

        // devo invertire la coordinata v
        // perchè viene rappresentata in modo inverso
        // da opengl
        glm::vec2 texCoord{ u, -v };
        _uvs.push_back(texCoord);
    }

    void ObjFileLoader::_AddNormal(std::vector<std::string> normalParams)
    {
        float x, y, z;

        x = std::stof(normalParams[1]);
        y = std::stof(normalParams[2]);
        z = std::stof(normalParams[3]);

        glm::vec3 normal{ x, y, z };
        _normals.push_back(normal);

    }

    void ObjFileLoader::_AddFaceElement(std::vector<std::string> params)
    {
        size_t prev = 0;

        Face face{ -1, -1, -1 };

        // Vertex normal indices without texture coordinate indices:
        // f v1//vn1 v2//vn2 v3//vn3
        if (params[1].find("//", prev) != string::npos)
        {
            for (int i = 1; i < params.size(); ++i)
            {
                vector<string> faceParams = _SplitLine(params[i], "//");

                int vertexIndex = stoi(faceParams[0]);
                int normalIndex = stoi(faceParams[1]);

                face.v[i - 1] = vertexIndex;
                face.vn[i - 1] = normalIndex;

                _vertexIndices.push_back(vertexIndex);
                _normalIndices.push_back(normalIndex);
            }
        }
            // Vertex normal indices: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            //  or
            // Vertex texture coordinate indices: f v1/vt1 v2/vt2 v3/vt3
        else if (params[1].find('/', prev) != string::npos)
        {
            for (int i = 1; i < params.size(); ++i)
            {
                vector<string> vertexParams = _SplitLine(params[i], "/");

                int vertexIndex = stoi(vertexParams[0]);
                int uvIndex = stoi(vertexParams[1]);
                int normalIndex = 0;

                // check if v1/vt1/vn1
                if (vertexParams.size() == 3)
                {
                    normalIndex = stoi(vertexParams[2]);
                    face.vn[i - 1] = normalIndex;
                    _hasNormals = true;
                }

                face.v[i - 1] = vertexIndex;
                face.vt[i - 1] = uvIndex;

                _vertexIndices.push_back(vertexIndex);
                _uvIndices.push_back(uvIndex);
                _normalIndices.push_back(normalIndex);
            }

        }
            // Vertex indices: f v1 v2 v3
        else
        {
            for (int i = 0; i < 3; ++i)
            {
                int vertexIndex = stoi(params[i + 1]);
                face.v[i] = stoi(params[1]);
                _vertexIndices.push_back(vertexIndex);
            }
        }
        _faces.push_back(face);
    }


    void ObjFileLoader::_AddObject(std::vector<std::string> params)
    {
        if (_numberObjs > 0)
        {
            _CompleteRawMesh();
        }

        auto mesh = new RawMesh();
        mesh->name = params[1];
        _meshes.push_back(mesh);

        _numberObjs++;
    }

    void ObjFileLoader::_CompleteRawMesh()
    {
        auto completeMesh = _meshes[_numberObjs - 1];

        if (_isIndexed)
        {
            _GetRawMeshIndexed(*completeMesh);
        }
        else
        {
            _GetRawMesh(*completeMesh);
        }

        Clear();
    }

    void ObjFileLoader::_ReadMtlFile()
    {

    }

    void ObjFileLoader::_GetRawMesh(RawMesh& mesh)
    {
        for (int i = 0; i < _vertexIndices.size(); ++i)
        {
            auto vertexIndex = _vertexIndices[i];
            auto uvIndex = _uvIndices[i];
            auto normalIndex = _normalIndices[i];

            auto vertex = _vertices[vertexIndex - 1];
            auto uv = _uvs[uvIndex - 1];
            auto normal = _normals[normalIndex - 1];

            mesh.vertices.push_back(vertex);
            mesh.uvs.push_back(uv);
            mesh.normals.push_back(normal);
        }
    }

    void ObjFileLoader::_GetRawMeshIndexed(RawMesh& mesh)
    {
        auto tmp = new RawMesh();
        _GetRawMesh(*tmp);

        std::map<Vertex, unsigned int> vertexToIndex;

        for (int j = 0; j < tmp->vertices.size(); ++j)
        {
            Vertex packed = {
                    tmp->vertices[j],
                    tmp->uvs[j],
                    tmp->normals[j]
            };

            unsigned int index = 0;
            bool found = _GetSimilarVertexIndex(packed, vertexToIndex, index);

            if (found)
            {
                mesh.indices.push_back(index);
            }
            else
            {
                mesh.vertices.push_back(tmp->vertices[j]);
                mesh.uvs.push_back(tmp->uvs[j]);
                mesh.normals.push_back(tmp->normals[j]);

                auto newIndex = static_cast<unsigned int>(mesh.vertices.size() - 1);
                mesh.indices.push_back(newIndex);

                vertexToIndex[packed] = newIndex;
            }
        }
    }

    bool ObjFileLoader::_GetSimilarVertexIndex(Vertex& vertex, std::map<Vertex, unsigned int>& map,
                                               unsigned int& index)
    {
        auto it = map.find(vertex);

        if (it == map.end())
        {
            return false;
        }
        else
        {
            index = it->second;
            return true;
        }
    }

    std::vector<RawMesh*> ObjFileLoader::GetMeshes()
    {
        return _meshes;
    }

    void ObjFileLoader::Clear()
    {
        _vertexIndices.clear();
        _uvIndices.clear();
        _normalIndices.clear();
    }

}
