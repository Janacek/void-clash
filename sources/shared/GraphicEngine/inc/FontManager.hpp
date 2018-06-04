/********************************************************************
**
**              FontManager.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_FONT_MANAGER
#define VOID_CLASH_FONT_MANAGER

#include <map>
#include <string>
#include "SingletonBase.hpp"
#include "Font.hpp"

namespace VoidClashGraphics
{
	class CFontManager
	{
	public:
		CFontManager(void);
		~CFontManager(void);

		void start(void);
		void stop(void);

		ogl::Font* addFont(const std::string &name,
			ogl::Font *fontIN = NULL);

		ogl::Font* getFont(const std::string &name);
		void setRootDirectory(const std::string &root);

	private:
		std::string _root;
		std::map<std::string, ogl::Font*> _fonts;
	};
}

typedef SingletonBase<VoidClashGraphics::CFontManager> FontManagerSingleton;
#define S_Font FontManagerSingleton::self()

#endif /* end of include guard: VOID_CLASH_FONT_MANAGER */
