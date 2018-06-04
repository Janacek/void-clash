/********************************************************************
**
**              EntityFlash.cpp
**              Created by : Vial Joris
**
********************************************************************/

// TODO - Fix me (flickering)

#include "EntityFlash.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraphicDefines.hpp"
#include "PhysicEngine.hpp"

namespace VoidClashGraphics
{
	/////////////////////////////////////////////////////////////////////
	/////	Ctor/Dtor
	/////////////////////////////////////////////////////////////////////

	EntityFlash::EntityFlash(void) : AEntity()
	{

	}

	EntityFlash::~EntityFlash(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Copy entity into this
	/////////////////////////////////////////////////////////////////////

	void EntityFlash::set(AEntity *entity)
	{
		EntityFlash *flash = dynamic_cast<EntityFlash*>(entity);

		if (flash == NULL)
		{
			_good = false;
			return;
		}

		_position = flash->_position;
		_currentLifetime = flash->_currentLifetime;
		_lifetime = flash->_lifetime;
		_initialSize = flash->_initialSize;
		_eventType = flash->_eventType;

		_good = true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void EntityFlash::start(void)
	{
		AEntity::start();

		_currentLifetime = 0;

		if (_event == NULL)
		{
			_good = false;
			return;
		}

		_eventType = _event->first;

		std::shared_ptr<AObject> trigger = _event->second.trigger;
		if (trigger == NULL)
		{
			_good = false;
			return;
		}

		// ev_EXPLOSTION
		if (_eventType == ev_EXPLOSION)
		{
			debugEntity(false, "G_FLASH", "ev_EXPLOSION");

			_lifetime = 0.5f;
			_initialSize = 5000.0f;
			_position = glm::vec3(trigger->getX(), -trigger->getY(), Z_FLASH);
		}

		// ev_WALL_COLLISION
		else if (_eventType == ev_WALL_COLLISION)
		{
			debugEntity(false, "G_FLASH", "ev_WALL_COLLISION");

			_lifetime = 0.5f;
			_initialSize = 1000.0f;

			t_impact *impact = (t_impact*)_event->second.data;
			if (impact == NULL)
			{
				_good = false;
				return;
			}
			_position = glm::vec3(impact->pos.first, -impact->pos.second, Z_FLASH);
		}

		// Everything is fine
		_good = true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void EntityFlash::stop(void)
	{
		AEntity::stop();

		if (_eventType == ev_EXPLOSION)
		{
			debugEntity(true, "G_FLASH", "ev_EXPLOSION");
		}
		else  if (_eventType == ev_WALL_COLLISION)
		{
			debugEntity(true, "G_FLASH", "ev_WALL_COLLISION");
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void EntityFlash::update(float deltatime)
	{
		AEntity::update(deltatime);

		// Life
		_currentLifetime += deltatime;
		if (_currentLifetime > _lifetime)
		{
			_good = false;
			return;
		}

		// Calculate size
		float size = _initialSize * (1.0 - (_currentLifetime / _lifetime)) * 2.0f;

		// Update the Geometry
		resetTransform();
		scale(size, size);
		translate(_position.x, _position.y, _position.z);

		// Update attributes
		updateColor();
		_matrix = getTransform();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update the color
	/////////////////////////////////////////////////////////////////////

	void EntityFlash::updateColor(void)
	{
		_vec4 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		_vec4.a *= 1.0 - (_currentLifetime / _lifetime);
	}
}
