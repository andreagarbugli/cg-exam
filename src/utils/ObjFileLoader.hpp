#ifndef ENGINE_OBJFILELOADER_HPP
#define ENGINE_OBJFILELOADER_HPP

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "math/Math.hpp"

namespace Utils
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;

        bool operator<(const Vertex that) const
        {
            return memcmp((void*) this, (void*) &that, sizeof(Vertex)) > 0;
        };
    };

    struct RawMesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        std::string name;
    };

    struct Face
    {
        int v[3];
        int vt[3];
        int vn[3];
    };

    struct Material
    {
        float ka; // ambient rgb
        float kd; // diffuse rgb
        float ks; // specular rgb
        float ns; // specular exponent 0 - 1000

        float d; // dissolved ovvero la trasparenza.

        int illum;

        std::string name;
    };

    class ObjFileLoader
    {
    private:
        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec2> _uvs;
        std::vector<glm::vec3> _normals;

        std::vector<unsigned int> _vertexIndices;
        std::vector<unsigned int> _uvIndices;
        std::vector<unsigned int> _normalIndices;

        std::vector<Face> _faces;

        std::vector<RawMesh*> _meshes;

        std::string _mtlFileName;
        std::vector<Material*> _materials;

        unsigned int _numberObjs;
        bool _isIndexed;
        bool _hasNormals;

    public:

        ObjFileLoader();

        void Clear();

        bool LoadObj(std::string fileName, bool indexed = false);

        std::vector<RawMesh*> GetMeshes();

    private:

        void _AddVertexCoord(std::vector<std::string> vectorParams);

        std::vector<std::string> _SplitLine(std::string str, std::string delim = " ");

        void _AddFaceElement(std::vector<std::string> params);

        void _AddTexCoord(std::vector<std::string> vector);

        void _AddNormal(std::vector<std::string> vector);

        void _AddObject(std::vector<std::string> params);

        void _CompleteRawMesh();

        void _GetRawMesh(RawMesh& mesh);

        void _GetRawMeshIndexed(RawMesh& mesh);

        bool _GetSimilarVertexIndex(Vertex& vertex, std::map<Vertex, unsigned int>& map, unsigned int& index);

        bool _ReadObjFile(std::string fileName);

        void _ReadMtlFile();

    };
}


#endif //ENGINE_OBJFILELOADER_HPP
