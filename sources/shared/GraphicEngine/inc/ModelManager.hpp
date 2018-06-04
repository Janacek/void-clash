/********************************************************************
**
**              ModelManager.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_MODEL_MANAGER
#define VOID_CLASH_MODEL_MANAGER

#include <map>
#include <string>
#include "SingletonBase.hpp"
#include "Model.hpp"

namespace VoidClashGraphics
{
	class CModelManager
	{
	public:
		CModelManager(void);
		~CModelManager(void);

		void start(void);
		void stop(void);

		ogl::Model* addModel(const std::string &name, bool create = false);
		ogl::Model* getModel(const std::string &name);
		void unbindTexture(void);

		void setRootDirectory(const std::string &root);

	private:
		std::string _root;
		std::map<std::string, ogl::Model*> _mesh;
	};
}

typedef SingletonBase<VoidClashGraphics::CModelManager> ModelManagerSingleton;
#define S_Model ModelManagerSingleton::self()

#endif /* end of include guard: VOID_CLASH_MODEL_MANAGER */
