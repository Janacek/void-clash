/********************************************************************
**
**              GraphicHelper.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_GRAPHIC_HELPER
#define VOIDCLASH_GRAPHIC_HELPER

#include <cstring>
#include <string>
#include "Camera.hpp"
#include "AObject.hpp"
#include "CheckError.hpp"

typedef long unsigned int VC_ADRESS;

namespace VoidClashGraphics
{
	// Define some color
	#define COLOR_WHITE glm::vec4(1, 1, 1, 1)
	#define COLOR_RED glm::vec4(1, 0, 0, 1)
	#define COLOR_BLUE glm::vec4(0, 0, 1, 1)
	#define COLOR_GREEN glm::vec4(0, 1, 0, 1)
	#define COLOR_CYAN glm::vec4(0, 1, 1, 1)

	/*
	** Graphic engine object type
	*/
	enum gObjectType
	{
		G_NONE,

		// Game objects
		G_WALL,
		G_PLAYER,
		G_BULLET,
		G_BOMB,
		G_ROCKET,
		G_EXPLOSION,
		G_SPEEDFIELD,
		G_RESPAWN,
		G_FLAG,
		G_TURRET,
		G_GRAVITYFIELD,
		G_BOT,
		G_CAPTURE,

		// Graphic effects
		G_FLASH,
		G_LIGHT,

		// Background stuff
		G_ENVIRONMENT,

		// Particles system
		G_PARTICLE_SYSTEM,
	};

	/////////////////////////////////////////////////////////////////////
	/////	Delete a pointer and set it to NULL
	/////////////////////////////////////////////////////////////////////

	template<class T> void safeDelete(T*& pVal)
	{
	    delete pVal;
	    pVal = NULL;
	}

	// Update the global camera
	void updateGlobalCamera(void);

	// Get the team color
	void getTeamColor(glm::vec4 &color, int currentTeam, bool isMainPlayer = false);

	// Convert an gObjectType to std::string
	std::string toStdString(enum gObjectType type);

	// Convert a pointer to std::string
	std::string toStdString(void *addr);

	// Convert a gObjectType to a eObjectType
	gObjectType toGraphicType(eObjectType type);

	// Check if the entity is in the screen
	bool isInScreen(float x, float y, float coeff, ogl::Camera *camera);

	// Convert screen to world (TO FIX)
	void screenToWorld(float x, float y, float &oX, float &oY, ogl::Camera *camera);

	// Convert world to screen
	void worldToScreen(float x, float y, float &oX, float &oY, ogl::Camera *camera);

	// Debug entity pop/depop
	void debugEntity(void *addr, gObjectType type, bool remove = false);
	void debugEntity(bool remove, const std::string &graphicType, const std::string &evenType);

	// Print driver property
	void printDriverProperty(void);

	// Convert time to string
	std::string timeToString(int time);
}

#define GL_CHECK ogl::checkError()
#define G_TO_DEGREE(X) (X / 3.141592f * 180.0f)
#define G_TO_SECONDS(X) (X * 0.000001f)

#endif /* end of include guard: VOIDCLASH_GRAPHIC_HELPER */
