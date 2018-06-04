/********************************************************************
**
**              ShaderManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ShaderManager.hpp"
#include "Log.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	CShaderManager::CShaderManager(void)
	{
	}

	CShaderManager::~CShaderManager(void)
	{
	}

	/////////////////////////////////////////////////////////////////////
	/////	Initialize
	/////////////////////////////////////////////////////////////////////

	void CShaderManager::start(void)
	{
		VC_INFO("ShaderManager::starting");

		// Turret
		addShader("TurretLife");
		addShader("TurretRange");
		addShader("TurretLocked");
		addShader("TurretAim");
		addShader("TurretMesh");

		// Player
		addShader("PlayerAim");
		addShader("PlayerShield");
		addShader("PlayerLevels");
		addShader("BombLoad");
		addShader("Model_Player");

		// Walls
		addShader("Model_Wall");

		// Skybox
		addShader("Model_Skybox");

		// 2D
		addShader("Flash");
		addShader("Bomb");
		addShader("Capture");
		addShader("GravityField");
		addShader("Light");
		addShader("Bullet");
		addShader("Respawn");
		addShader("Speedfield");
		addShader("CaptureZoneProgress");

		// Particles
		addShader("Particle");

		VC_INFO("ShaderManager::started");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void CShaderManager::stop(void)
	{
		VC_INFO("ShaderManager::stopping");

		std::map<std::string, ogl::Shader*>::iterator it = _shaders.begin();
		std::map<std::string, ogl::Shader*>::iterator end = _shaders.end();

		while (it != end)
		{
			it->second->destroy();
			delete(it->second);
			++it;
		}

		_shaders.clear();

		VC_INFO("ShaderManager::stopped");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Load shader if it doesn't exists and keep it into map
	/////////////////////////////////////////////////////////////////////

	ogl::Shader *CShaderManager::addShader(const std::string &name,
		ogl::Shader *shaderIN)
	{
		std::map<std::string, ogl::Shader*>::iterator it = _shaders.find(name);

		if (it == _shaders.end())
		{
			if (shaderIN == NULL)
			{
				std::string vert(_root + name + ".vert");
				std::string frag(_root + name + ".frag");
				ogl::Shader *shader = new ogl::Shader();
				shader->loadFromFile(vert.c_str(), frag.c_str());
				shader->finalize(name.c_str());
				_shaders[name] = shader;
				return shader;
			}
			else
			{
				_shaders[name] = shaderIN;
				return shaderIN;
			}
		}
		return _shaders[name];
	}

	/////////////////////////////////////////////////////////////////////
	/////	Bind the shader passed in parameter
	/////////////////////////////////////////////////////////////////////

	ogl::Shader* CShaderManager::bindShader(const std::string &name)
	{
		std::map<std::string, ogl::Shader*>::iterator it = _shaders.find(name);

		if (it == _shaders.end())
		{
			std::string warn("CShaderManager::trying to bind shader " + name + " but it doesn't exists");
			VC_WARNING_CRITICAL(warn);
			return NULL;
		}
		_shaders[name]->bind();

		return _shaders[name];
	}

	/////////////////////////////////////////////////////////////////////
	/////	Unbind any shader
	/////////////////////////////////////////////////////////////////////

	void CShaderManager::unbindShader(void)
	{
		// We don't care of the type, because unbind() just bind the 0 id texture
		std::map<std::string, ogl::Shader*>::iterator it = _shaders.begin();

		if (it != _shaders.end())
			it->second->unbind();
		else
			VC_WARNING("ShaderManager::cannot unbind shader because there are no shader in the manager");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Set the root path where assets will be loaded
	/////////////////////////////////////////////////////////////////////

	void CShaderManager::setRootDirectory(const std::string &root)
	{
		_root = root;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Return a pointer on the shader
	/////////////////////////////////////////////////////////////////////

	ogl::Shader* CShaderManager::getShader(const std::string &name)
	{
		std::map<std::string, ogl::Shader*>::iterator it = _shaders.find(name);
		if (it == _shaders.end())
			return NULL;
		return it->second;
	}
}
