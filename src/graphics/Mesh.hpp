#ifndef ENGINE_VERTEXARRAY_HPP
#define ENGINE_VERTEXARRAY_HPP

#include "utils/ObjFileLoader.hpp"

namespace Graphics
{
    class Mesh
    {
    private:
        unsigned int _verticesNumber;
        unsigned int _uvsNumber;
        unsigned int _normalsNumber;
        unsigned int _indicesNumber;

        /*
         * VAO è un oggetto dove posso mettere data
         * è composto da diversi slot in cui posso mettere
         * questi dati (solitamente 16 slot) ad esempio
         * posso mettere vertici, uv, normal
         *
         * In realà metto un puntatore ai dati veri e propri
         */
        unsigned int _vertexArrayObject;

        /*
         * VBO è l'array vero e proprio che contiene i dati
         * e sono quindi quelli puntati dai VAO
         */
        unsigned int _vertexBufferObject;
        unsigned int _uvBufferObject;
        unsigned int _normalBufferObject;

        unsigned int _indexBufferObject;

        bool _hasIndices;

    public:

        explicit Mesh(Utils::RawMesh& model);

        ~Mesh();

        void Use();
    };

}

#endif //ENGINE_VERTEXARRAY_HPP
