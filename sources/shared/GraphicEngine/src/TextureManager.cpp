/********************************************************************
**
**              TextureManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "TextureManager.hpp"
#include "Log.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	CTextureManager::CTextureManager(void)
	{
	}

	CTextureManager::~CTextureManager(void)
	{
	}

	/////////////////////////////////////////////////////////////////////
	/////	Initialize
	/////////////////////////////////////////////////////////////////////

	void CTextureManager::start(void)
	{
		VC_INFO("TextureManager::starting");

		addTexture("White");
		addTexture("PlayerAim", GL_MIRRORED_REPEAT);
		addTexture("BombLoad");
		addTexture("TurretLocked");
		addTexture("Speedfield", GL_MIRRORED_REPEAT);
		addTexture("Respawn");
		addTexture("Respawn2");
		addTexture("Gravity");

		addTexture("Particle");
		addTexture("Particle_explosion");

		VC_INFO("TextureManager::started");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void CTextureManager::stop(void)
	{
		VC_INFO("TextureManager::stopping");

		std::map<std::string, ogl::Texture*>::iterator it = _textures.begin();
		std::map<std::string, ogl::Texture*>::iterator end = _textures.end();

		while (it != end)
		{
			delete(it->second);
			++it;
		}

		_textures.clear();

		VC_INFO("TextureManager::stopped");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Load textue if it doesn't exists and keep it into map
	/////////////////////////////////////////////////////////////////////

	ogl::Texture* CTextureManager::addTexture(const std::string &name, int repeatMode)
	{
		std::map<std::string, ogl::Texture*>::iterator it = _textures.find(name);

		if (it == _textures.end())
		{
			std::string file(_root + name + ".png");
			ogl::Texture *texture = new ogl::Texture();
			texture->create(file.c_str(), repeatMode);
			_textures[name] = texture;

			return texture;
		}
		return _textures[name];
	}

	/////////////////////////////////////////////////////////////////////
	/////	Set the root path where assets will be loaded
	/////////////////////////////////////////////////////////////////////

	ogl::Texture* CTextureManager::bindTexture(const std::string &name)
	{
		std::map<std::string, ogl::Texture*>::iterator it = _textures.find(name);

		if (it == _textures.end())
		{
			std::string warn("CTextureManager::trying to bind texture " + name + " but it doesn't exists");
			VC_WARNING_CRITICAL(warn);
			return NULL;
		}

		_textures[name]->bind();
		return _textures[name];
	}

	/////////////////////////////////////////////////////////////////////
	/////	Unbind any texture
	/////////////////////////////////////////////////////////////////////

	void CTextureManager::unbindTexture(void)
	{
		// We don't care of the type, because unbind() just bind the 0 id texture
		std::map<std::string, ogl::Texture*>::iterator it = _textures.begin();

		if (it != _textures.end())
			it->second->unbind();
		else
			VC_WARNING("TextureManager::cannot unbind texture because there are no texture in the manager");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Set the root path where assets will be loaded
	/////////////////////////////////////////////////////////////////////

	void CTextureManager::setRootDirectory(const std::string &root)
	{
		_root = root;
	}
}
