/********************************************************************
**
**              ShaderManger.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_SHADER_MANAGER
#define VOID_CLASH_SHADER_MANAGER

#include <map>
#include <string>
#include "SingletonBase.hpp"
#include "Shader.hpp"

namespace VoidClashGraphics
{
	class CShaderManager
	{
	public:
		CShaderManager(void);
		~CShaderManager(void);

		void start(void);
		void stop(void);

		ogl::Shader* addShader(const std::string &name,
			ogl::Shader *shaderIN = NULL);
		ogl::Shader* bindShader(const std::string &name);
		void unbindShader(void);

		ogl::Shader* getShader(const std::string &name);
		void setRootDirectory(const std::string &root);

	private:
		std::string _root;
		std::map<std::string, ogl::Shader*> _shaders;
	};
}

typedef SingletonBase<VoidClashGraphics::CShaderManager> ShaderManagerSingleton;
#define S_Shader ShaderManagerSingleton::self()

#endif /* end of include guard: VOID_CLASH_SHADER_MANAGER */
