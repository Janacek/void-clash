/********************************************************************
**
**              Quad.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_QUAD
#define OGL_QUAD

#include "Transformable.hpp"
#include "Instanced.hpp"

namespace ogl
{
    class Quad : public Transformable, public Instanced
    {
    public:
        Quad(void);
        virtual ~Quad(void);

        void setInstanced(bool instanced);
        void create(void);
        void draw(void);
        void destroy(void);
        void bind(void);
        void unbind(void);

    private:
        GLuint _vao;
        GLuint _quad;

        bool _instanced;
        bool _created;
        bool _binded;
    };
}

#endif /* end of include guard: OGL_QUAD */
