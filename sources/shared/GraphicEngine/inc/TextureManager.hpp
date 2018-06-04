/********************************************************************
**
**              TextureManager.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_TEXTURE_MANAGER
#define VOID_CLASH_TEXTURE_MANAGER

#include <map>
#include <string>
#include "SingletonBase.hpp"
#include "Texture.hpp"

namespace VoidClashGraphics
{
	class CTextureManager
	{
	public:
		CTextureManager(void);
		~CTextureManager(void);

		void start(void);
		void stop(void);

		ogl::Texture* addTexture(const std::string &name, int repeatMode = GL_CLAMP_TO_EDGE);
		ogl::Texture* bindTexture(const std::string &name);
		void unbindTexture(void);

		void setRootDirectory(const std::string &root);

	private:
		std::string _root;
		std::map<std::string, ogl::Texture*> _textures;
	};
}

typedef SingletonBase<VoidClashGraphics::CTextureManager> TextureManagerSingleton;
#define S_Texture TextureManagerSingleton::self()

#endif /* end of include guard: VOID_CLASH_TEXTURE_MANAGER */
