/********************************************************************
**
**              ModelManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "ModelManager.hpp"
#include "Log.hpp"
#include "ShaderManager.hpp"

namespace VoidClashGraphics
{
	#define VERTEX_POSITION_BINDING 0
	#define VERTEX_TEXTURE_BINDING 1
	#define VERTEX_NORMAL_BINDING 2

	/////////////////////////////////////////////////////////////////////
	/////	Ctor/Dtor
	/////////////////////////////////////////////////////////////////////

	CModelManager::CModelManager(void)
	{
	}

	CModelManager::~CModelManager(void)
	{
	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void CModelManager::start(void)
	{
		VC_INFO("ModelManager::starting");

		addModel("ShipConcept/ShipOne.obj");
		addModel("ShipTwo/ShipTwo.obj");
		addModel("TurretCanon/TurretCanon.obj");
		addModel("Wall/Wall.obj");
		addModel("Cube/Cube.obj", true);

		VC_INFO("ModelManager::started");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void CModelManager::stop(void)
	{
		VC_INFO("ModelManager::stopping");

		std::map<std::string, ogl::Model*>::iterator it = _mesh.begin();
		std::map<std::string, ogl::Model*>::iterator end = _mesh.end();

		while (it != end)
		{
			it->second->destroy();
			delete(it->second);
			++it;
		}

		_mesh.clear();

		VC_INFO("ModelManager::stopped");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Add a new model
	/////////////////////////////////////////////////////////////////////

	ogl::Model *CModelManager::addModel(const std::string &name, bool autoCreate)
	{
		std::string filenameToSearch(name);
		if (name.find_last_of('/') != name.npos)
		{
			int index = std::min(name.npos, name.find_last_of('/') + 1);
			filenameToSearch = name.substr(index);
		}

		auto it = _mesh.find(filenameToSearch);

		if (it != _mesh.end())
			return it->second;

		std::string fullPath(_root + name);

		ogl::Model *mesh = new ogl::Model;
		mesh->setPath(fullPath);
		mesh->setBufferIndexes(VERTEX_POSITION_BINDING,
			VERTEX_TEXTURE_BINDING, VERTEX_NORMAL_BINDING);

		if (autoCreate == true)
		{
			mesh->create(fullPath);
		}

		int index = std::min(name.npos, name.find_last_of('/') + 1);
		std::string filename = name.substr(index);

		_mesh[filename] = mesh;

		return mesh;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Set the root path
	/////////////////////////////////////////////////////////////////////

	void CModelManager::setRootDirectory(const std::string &root)
	{
		_root = root;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Get a model
	/////////////////////////////////////////////////////////////////////

	ogl::Model* CModelManager::getModel(const std::string &name)
	{
		std::string filename(name);
		if (name.find_last_of('/') != name.npos)
		{
			int index = std::min(name.npos, name.find_last_of('/') + 1);
			filename = name.substr(index);
		}

		std::map<std::string, ogl::Model*>::iterator it = _mesh.find(filename);

		if (it == _mesh.end())
			return NULL;

		return it->second;
	}
}
