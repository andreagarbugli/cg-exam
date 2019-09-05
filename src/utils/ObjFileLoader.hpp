#ifndef ENGINE_OBJFILELOADER_HPP
#define ENGINE_OBJFILELOADER_HPP

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
        std::string materialName;
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
        glm::vec3 ka; // ambient rgb
        glm::vec3 kd; // diffuse rgb
        glm::vec3 ks; // specular rgb
        float ns; // specular exponent 0 - 1000
        std::string mapKd;
        std::string mapKs;
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
        unsigned int _currentObjIndex;
        unsigned int _numberObjs;

        std::string _mtlFileName;
        std::vector<Material*> _materials;
        unsigned int _numberMaterials;

        bool _isIndexed;
        bool _hasNormals;

    public:

        ObjFileLoader();

        bool LoadObj(const std::string& fileName, bool indexed = false);

        std::vector<RawMesh*> GetMeshes();

        std::vector<Material*> GetMaterials();

    private:

        void _AddVertexCoord(std::vector<std::string> vectorParams);

        static std::vector<std::string> _SplitLine(const std::string& str, const std::string& delim = " ");

        void _AddFaceElement(std::vector<std::string> params);

        void _AddTexCoord(std::vector<std::string> vector);

        void _AddNormal(std::vector<std::string> vector);

        void _AddObject(std::string& objectName);

        void _CompleteRawMesh(bool isSubMesh = false);

        void _GetRawMesh(RawMesh& mesh);

        void _GetRawMeshIndexed(RawMesh& mesh);

        static bool _GetSimilarVertexIndex(Vertex& vertex, std::map<Vertex, unsigned int>& map, unsigned int& index);

        static glm::vec3  _ParseMaterialColor(std::vector<std::string> params);

        bool _ReadObjFile(const std::string& fileName);

        bool _ReadMtlFile();

        void _UseNewMaterial(std::string& materialName);
    };
}


#endif //ENGINE_OBJFILELOADER_HPP
