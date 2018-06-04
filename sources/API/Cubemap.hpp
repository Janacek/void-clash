/********************************************************************
**
**              Cubemap.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_CUBEMAP
#define OGL_CUBEMAP

#include "VoidClash_OpenGL.h"

namespace ogl
{
    class Cubemap
    {
    public:
        Cubemap(void);
        ~Cubemap(void);

        void create(const std::string &path);
        void bind(char unit = 0);
        void unbind(void);

        GLuint getId(void);

    private:
        void loadImage(GLenum target, const std::string &path,
            int &width, int &height);

    private:
        GLuint _id;
    };
}


#endif /* end of include guard: OGL_CUBEMAP */
