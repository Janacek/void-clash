/********************************************************************
**
**              EntityFlash.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOID_CLASH_ENTITY_FLASH
#define VOID_CLASH_ENTITY_FLASH

#include "AEntity.hpp"

namespace VoidClashGraphics
{
	class EntityFlash : public AEntity
	{
	public:
		EntityFlash(void);
		virtual ~EntityFlash(void);

		virtual void start(void);
		virtual void update(float deltatime);
		virtual void stop(void);

		// Copy the content of the AEntity passed in parameter to this
		virtual void set(AEntity *entity);

	private:
		void updateColor(void);

	private:
		// Attributes
		eventType _eventType;
		float _currentLifetime;
		float _lifetime;
		float _initialSize;
		glm::vec3 _position;
	};
}

#endif /* end of include guard: VOID_CLASH_ENTITY_FLASH */
