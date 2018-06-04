/********************************************************************
**
**              ChildManager.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "VoidClash_OpenGL.h"
#include "ChildManager.hpp"
#include "Log.hpp"
#include "Event.hpp"
#include "GraphicHelper.hpp"
#include "Templates.hpp"
#include "Map.hpp"

namespace VoidClashGraphics
{
    /////////////////////////////////////////////////////////////////////
    /////	Ctor/Dtor
    /////////////////////////////////////////////////////////////////////

    template<class T>
    ChildManager<T>::ChildManager(void)
    {
    }

    template<class T>
    ChildManager<T>::~ChildManager(void)
    {

    }

    /////////////////////////////////////////////////////////////////////
    /////	Start
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::start(void)
    {
        _nextSlot = 0;
		lookForGameObjects();
    }

	/////////////////////////////////////////////////////////////////////
	/////	Find already created game objects
	/////////////////////////////////////////////////////////////////////

	template<class T>
	void ChildManager<T>::lookForGameObjects(void)
	{
		// Everything but walls
		auto elems = S_Map->getElems();

		if (elems != NULL)
		{
			auto it = elems->begin();
			auto end = elems->end();

			while (it != end)
			{
				if (isConcernedBy(*it) == true)
				{
					addChild(*it);
				}
				++it;
			}
		}

		// Walls
		auto walls = S_Map->getWalls();
		if (walls != NULL)
		{
			auto it = walls->begin();
			auto end = walls->end();

			while (it != end)
			{
				if (isConcernedBy(*it) == true)
				{
					addChild(*it);
				}
				++it;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
    /////	Stop
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::stop(void)
    {
        for (auto it = _childs.begin(); it != _childs.end(); ++it)
        {
            it->stop();
        }

        _childs.clear();
        _nextSlot = 0;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Update
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::update(void)
    {
        // Check for broken entities
        unsigned int current = 0;

        while (current != _nextSlot && current < _childs.size())
        {
            if (_childs[current].good() == false)
            {
                removeChildAt(current);
            }
            else
            {
                ++current;
            }
        }

		// ----------- Module based on events ---------- //

		if (_evTypes.size() != 0)
		{
			if (Event::getMainEventList() != NULL)
			{
				auto ev = Event::getMainEventList()->begin();
				auto end = Event::getMainEventList()->end();

				while (ev != end)
				{
					if (isConcernedBy(ev->first) == true)
					{
						addChild(&(*ev));
					}
					++ev;
				}
			}

			// We are done
			return;
		}

		// ----------- Module based on game objects ---------- //

		// Check for delete events
        if (Event::getEventByType(ev_DELETE) != NULL)
        {
            auto ev = Event::getEventByType(ev_DELETE)->begin();
            auto end = Event::getEventByType(ev_DELETE)->end();

            while (ev != end)
            {
                if (isConcernedBy(ev->second.trigger) == true)
                {
                    removeChild(ev->second.trigger);
                }
                ++ev;
            }
        }

		// Check for start events
        if (Event::getEventByType(ev_START) != NULL)
        {
            auto ev = Event::getEventByType(ev_START)->begin();
            auto end = Event::getEventByType(ev_START)->end();

            while (ev != end)
            {
                if (isConcernedBy(ev->second.trigger) == true)
                {
                    addChild(ev->second.trigger);
                }
                ++ev;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Check events trigger type
    /////////////////////////////////////////////////////////////////////

    template<class T>
    bool ChildManager<T>::isConcernedBy(std::shared_ptr<AObject> gameObject)
    {
        if (gameObject == NULL)
        {
            return false;
        }

        gObjectType gameType = toGraphicType(gameObject->getType());
        for (auto it = _types.begin(); it != _types.end(); ++it)
        {
            if (*it == gameType)
            {
                return true;
            }
        }

        return false;
    }

	template<class T>
	bool ChildManager<T>::isConcernedBy(eventType type)
	{
		for (auto it = _evTypes.begin(); it != _evTypes.end(); ++it)
		{
			if (*it == type)
			{
				return true;
			}
		}

		return false;
	}

	/////////////////////////////////////////////////////////////////////
    /////	Add child
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::addChild(std::shared_ptr<AObject> gameObject)
    {
        if (gameObject == NULL)
        {
            VC_WARNING_CRITICAL("ChildManager::Trying to add an entity with a NULL gameObject, ignoring");
            return;
        }

        // Check if we still have free slot
        if (_nextSlot >= _childs.size())
        {
            VC_WARNING_CRITICAL("ChildManager::Limits for this container reached");
            return;
        }

        // Set the gameObject handler
        _childs[_nextSlot].setGameObject(gameObject);
        _childs[_nextSlot].setCamera(_aModuleCamera);

        // Start the new entity
        _childs[_nextSlot].start();

        // Check if everything is fine
        if (_childs[_nextSlot].good() == true)
        {
            debugEntity(gameObject.get(), toGraphicType(gameObject->getType()), false);
            _nextSlot += 1;
            return;
        }
        else
        {
            VC_INFO_CRITICAL("ChildManager::Created the object, but initialization failed");
        }
    }

	template<class T>
	void ChildManager<T>::addChild(std::pair<eventType, s_event> *event)
	{
		// Check if we still have free slot
		if (_nextSlot >= _childs.size())
		{
			VC_WARNING_CRITICAL("ChildManager::Limits for this container reached");
			return;
		}

		// Set the gameObject handler
		_childs[_nextSlot].setEvent(event);
		_childs[_nextSlot].setCamera(_aModuleCamera);

		// Start the new entity
		_childs[_nextSlot].start();

		// Check if everything is fine
		if (_childs[_nextSlot].good() == true)
		{
			_nextSlot += 1;
			return;
		}
		else
		{
			VC_INFO_CRITICAL("ChildManager::Created the object, but initialization failed");
		}
	}

	/////////////////////////////////////////////////////////////////////
    /////	Remove child based on AObject
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::removeChild(std::shared_ptr<AObject> gameObject)
    {
        if (gameObject == NULL)
        {
            VC_WARNING_CRITICAL("ChildManager::Trying to remove an entity with a NULL gameObject, ignoring");
            return;
        }

        // Find the entity to remove
        unsigned int toRemove = 0;

        while (toRemove < _nextSlot && toRemove < _childs.size())
        {
            AEntity &entity(_childs[toRemove]);

            // We've found the entity
            if (entity.getGameObject() == gameObject)
            {
                // Stop the entity
                entity.stop();

                if (_nextSlot > 0)
                {
                    // Copy the last entity to its slot
                    _childs[toRemove].set(&(_childs[_nextSlot - 1]));

                    // Stop the copied entity
                    _childs[_nextSlot - 1].stop();

                    if (_childs[toRemove].getGameObject() != NULL)
                    {
                        debugEntity(gameObject.get(), toGraphicType(gameObject->getType()), true);
                    }

                    --_nextSlot;
                }
                return;
            }
            ++toRemove;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Remove child based on index
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::removeChildAt(unsigned int index)
    {
        if (index < _nextSlot && index < _childs.size())
        {
            // Stop the entity
            _childs[index].stop();

            if (_nextSlot > 0)
            {
                // Copy the last entity into the free slot
                _childs[index].set(&(_childs[_nextSlot - 1]));

                // Stop the copied entity
                _childs[_nextSlot - 1].stop();

                if (_childs[index].getGameObject() != NULL)
                {
                    debugEntity(_childs[index].getGameObject().get(),
                        toGraphicType(_childs[index].getGameObject()->getType()), true);
                }

                --_nextSlot;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////	Wrappers
    /////////////////////////////////////////////////////////////////////

    template<class T>
    void ChildManager<T>::resize(unsigned int size)
    {
        _childs.resize(size);
    }

    template<class T>
    void ChildManager<T>::addChildType(gObjectType type)
    {
		if (_evTypes.size() != 0)
		{
			VC_WARNING_CRITICAL("ChildManager::Trying to mix AObject and eventType, ignoring");
			return;
		}
		_types.push_back(type);
    }

    template<class T>
    std::vector<gObjectType>& ChildManager<T>::getChildType(void)
    {
        return _types;
    }

	template<class T>
	void ChildManager<T>::addEventType(eventType type)
	{
		if (_types.size() != 0)
		{
			VC_WARNING_CRITICAL("ChildManager::Trying to mix AObject and eventType, ignoring");
			return;
		}
		_evTypes.push_back(type);
	}

	template<class T>
	std::vector<eventType>& ChildManager<T>::getEventType(void)
	{
		return _evTypes;
	}

	template<class T>
    std::vector<T>& ChildManager<T>::getChilds(void)
    {
        return _childs;
    }

    template<class T>
    unsigned int ChildManager<T>::getNextSlot(void)
    {
        return this->_nextSlot;
    }

    template<class T>
    void ChildManager<T>::setAModuleCamera(ogl::Camera *camera)
    {
        _aModuleCamera = camera;
    }

    /////////////////////////////////////////////////////////////////////
    /////	Template intances
    /////////////////////////////////////////////////////////////////////

    template class ChildManager<EntityBullet>;
    template class ChildManager<EntityRespawn>;
    template class ChildManager<EntitySpeedfield>;
    template class ChildManager<EntityWall>;
    template class ChildManager<EntityPlayer>;
    template class ChildManager<EntityGravity>;
    template class ChildManager<EntityFlag>;
    template class ChildManager<EntityBomb>;
    template class ChildManager<EntityLight>;
	template class ChildManager<EntityCapture>;
	template class ChildManager<EntityFlash>;
}
