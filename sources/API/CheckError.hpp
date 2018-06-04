/********************************************************************
**
**              CheckError.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef OGL_CHECK_ERROR
#define OGL_CHECK_ERROR

namespace ogl
{
	void _check_gl_error(const char *file, int line);

	#if defined(_WIN32)
		#define checkError() _check_gl_error(__FUNCTION__,__LINE__)
	#else
		#define checkError() _check_gl_error(__PRETTY_FUNCTION__,__LINE__)
	#endif
}

#endif
