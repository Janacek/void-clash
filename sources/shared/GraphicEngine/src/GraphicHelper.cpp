/********************************************************************
**
**              GraphicHelper.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include <string>
#include <sstream>
#include "GraphicHelper.hpp"
#include "GraphicEngine.hpp"
#include "Map.hpp"
#include "ConfigParser.hpp"
#include "GraphicConfiguration.hpp"
#include "CheckError.hpp"
#include "Context.hpp"
#include "Log.hpp"
#include "GraphicDefines.hpp"

/* Globals */
extern t_config *G_conf;

namespace VoidClashGraphics
{
	// Global camera used for world/screen conversion
	// This camera follows the playerFollowed and use
	// the default zoom (S_Map->getZoom() * COEFF_ZOOM_MULT)
	ogl::Camera globalCamera;

	/////////////////////////////////////////////////////////////////////
	/////	Update the global camera
	/////////////////////////////////////////////////////////////////////

	void updateGlobalCamera(void)
	{
		static bool init = false;

		// Init the camera the first time
		if (init == false)
		{
			globalCamera.setupPerspective(
				45.0f,
				(float)GCONFIG.winX / (float)GCONFIG.winY,
				100.0f,
				50000.0f
			);
			init = true;
		}

		// Get player to follow
        std::shared_ptr<Player> player = S_Map->getPlayerFollowed();

        // Camera position
        glm::vec2 pos(0.0f, 0.0f);

        if (player != NULL)
        {
            pos.x = player->getX();
            pos.y = -player->getY();
        }

        // Update the camera
        globalCamera.lookAt(
            glm::vec3(pos.x, pos.y, S_Map->getZoom() * COEFF_ZOOM_MULT),
            glm::vec3(pos.x, pos.y, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

		globalCamera.update();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Get player color
	/////////////////////////////////////////////////////////////////////

	void getTeamColor(glm::vec4 &color, int currentTeam, bool isMainPlayer)
	{
		std::memset(&color, 0, sizeof(glm::vec4));

		color.a = 1;

		// No team - White
		if (currentTeam == 0)
		{
			color.r = 1;
			color.g = 1;
			color.b = 1;
			return;
		}

		// FFA - Survivor
		if (!S_Map->getMode()->getProperty()->team)
		{
			if (isMainPlayer)
			{
				color.b = 0;
				color.g = 1;
				color.r = 0;
			}
			else
			{
				color.b = 0;
				color.g = 0;
				color.r = 1;
			}
			return;
		}

		// Team 1 - Green
		if (currentTeam == 1)
		{
			color.g = 1;
		}

		// Team 2 - Red
		else if (currentTeam == 2)
		{
			color.r = 1;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert an enum to a string
	/////////////////////////////////////////////////////////////////////

	std::string toStdString(enum gObjectType type)
	{
		switch (type)
		{
		case G_NONE: return "G_NONE";
		case G_WALL: return "G_WALL";
		case G_PLAYER: return "G_PLAYER";
		case G_BULLET: return "G_BULLET";
		case G_BOMB: return "G_BOMB";
		case G_EXPLOSION: return "G_EXPLOSION";
		case G_SPEEDFIELD: return "G_SPEEDFIELD";
		case G_RESPAWN: return "G_RESPAWN";
		case G_FLAG: return "G_FLAG";
		case G_ROCKET: return "G_ROCKET";
		case G_TURRET: return "G_TURRET";
		case G_GRAVITYFIELD: return "G_GRAVITYFIELD";
		case G_BOT: return "G_BOT";
		case G_CAPTURE: return "G_CAPTURE";

		case G_FLASH: return "G_FLASH";

		case G_PARTICLE_SYSTEM: return "G_PARTICLE_SYSTEM";

		default: return "UKNOWN";
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert an adress to a std::string
	/////////////////////////////////////////////////////////////////////

	std::string toStdString(void *addr)
	{
		std::stringstream ss("");
		ss << std::hex << addr;
		return ss.str();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Check if the entity is in the screen
	/////	If coeff == 1 it will match exactly the screen size
	/////////////////////////////////////////////////////////////////////

	bool isInScreen(float x, float y, float coeff, ogl::Camera *camera)
	{
		// Project the world position
		glm::vec4 res(camera->getFinal() * glm::vec4(x, y, 0, 1.0f));

		// Normalize
		res.x /= S_Map->getZoom() * COEFF_ZOOM_MULT;
		res.y /= -S_Map->getZoom()* COEFF_ZOOM_MULT;

		// Does the position is in the screen ?
		if (std::abs(res.x) > coeff || std::abs(res.y) > coeff)
			return false;

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert world position into screen position
	/////////////////////////////////////////////////////////////////////

	void worldToScreen(float x, float y, float &oX, float &oY, ogl::Camera *camera)
	{
		const glm::mat4 &view(camera->getView());
		const glm::mat4 &proj(camera->getProjectionMatrix());

		glm::ivec2 ws = S_Context->getWindowSize();

		glm::vec3 screenCoord = glm::project(
			glm::vec3(x, y, 0.0f),
			view,
			proj,
			glm::vec4(0, 0, ws.x, ws.y)
			);

		oX = screenCoord.x;
		oY = ws.y - screenCoord.y;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert screen position into world position
	/////////////////////////////////////////////////////////////////////

	void screenToWorld(float x, float y, float &oX, float &oY, ogl::Camera *camera)
	{
		const glm::mat4 &view(camera->getView());
		const glm::mat4 &projection(camera->getProjectionMatrix());

		glm::ivec2 ws = S_Context->getWindowSize();

		glm::vec3 res = glm::unProject(glm::vec3(x, GCONFIG.winY - y, 0.0),
			view,
			projection,
			glm::vec4(0, 0, ws.x, ws.y));

		glm::vec3 res2 = glm::unProject(glm::vec3(x, ws.y - y, 1.0),
			view,
			projection,
			glm::vec4(0, 0, ws.x, ws.y));

		double f = res.z / (res2.z - res.z);
		oX = res.x - f * (res2.x - res.x);
		oY = res.y - f * (res2.y - res.y);
		oY = -oY;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Debug entity pop/depop
	/////////////////////////////////////////////////////////////////////

	void debugEntity(void *addr, gObjectType type, bool remove)
	{
		std::string add("ADD   ");
		if (remove == true)
			add = "DELETE ";

		VC_INFO(add + "\tAObject=" +
			toStdString(addr) + "\t\tType=" +
			toStdString(type));
	}

	void debugEntity(bool remove, const std::string &graphicType, const std::string &evenType)
	{
		std::string add("ADD   ");
		if (remove == true)
			add = "DELETE ";

		VC_INFO(add + "\tEvent=" +
			evenType + "\t\tType=" +
			graphicType);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Print driver property
	/////////////////////////////////////////////////////////////////////

	void printDriverProperty(void)
	{
		std::stringstream ss("");
		ss << std::endl << "<< --------OpenGL Driver property-------- >>" << std::endl;

		GLint val = 0;

		// OpenGL version
		ss << "--> OpenGL version [" << glGetString(GL_VERSION) << "]" << std::endl;
		// Shader versoin
		ss << "--> Shader version [" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "]" << std::endl;
		// Renderer
		ss << "--> Graphic Card [" << glGetString(GL_RENDERER) << "]" << std::endl;
		// Vendor
		ss << "--> Vendor [" << glGetString(GL_VENDOR) << "]" << std::endl;
		// Max uniform in vertex shader
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &val);
		// Max uniform in fragment shader
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &val); ss << "--> GL_MAX_FRAGMENT_UNIFORM_COMPONENTS [" << val << "]" << std::endl;
		// Max uniform location
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &val); ss << "--> GL_MAX_UNIFORM_LOCATIONS [" << val << "]" << std::endl;
		// Max varyings
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &val); ss << "--> GL_MAX_VARYING_FLOATS [" << val << "]" << std::endl;
		// Max vertex attribs
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val); ss << "--> GL_MAX_VERTEX_ATTRIBS [" << val << "]" << std::endl;
		// Max texture size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val); ss << "--> GL_MAX_TEXTURE_SIZE [" << val << "]" << std::endl;
		// Max 3D texture size
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &val); ss << "--> GL_MAX_3D_TEXTURE_SIZE [" << val << "]" << std::endl;
		// Max texture unit
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &val); ss << "--> GL_MAX_TEXTURE_UNITS [" << val << "]" << std::endl;
		// Max UBO number vertex
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &val); ss << "--> GL_MAX_VERTEX_UNIFORM_BLOCKS [" << val << "]" << std::endl;
		// Max UBO number fragment
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &val); ss << "--> GL_MAX_FRAGMENT_UNIFORM_BLOCKS [" << val << "]" << std::endl;
		// Max UBO size
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &val); ss << "--> GL_MAX_UNIFORM_BLOCK_SIZE [" << val << "]" << std::endl;
		// UBO alignement
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &val); ss << "--> GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT [" << val << "]" << std::endl;
		// Max transform feedback attribs
		glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &val); ss << "--> GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS [" << val << "]" << std::endl;
		// Max draw buffer (framebuffer)
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &val); ss << "--> GL_MAX_DRAW_BUFFERS [" << val << "]" << std::endl;

		// Print out the result
		ss << "<< ------------------------------------ >>" << std::endl;
		VC_INFO_CRITICAL(ss.str());

		// Empty possible errors (we don't care here)
		while (glGetError() != GL_NO_ERROR);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert time to string
	/////////////////////////////////////////////////////////////////////

	std::string timeToString(int time)
	{
		int h = 0;
		h = time / 3600;
		time = time - h * 3600;

		int min = 0;
		min = time / 60;
		time = time - min * 60;

		int s = time;

		std::string timeStr("");
		if (h != 0)
		{
			if (h < 10)
				timeStr += "0";
			timeStr += std::to_string(h) + ":";
		}
		if (min < 10)
			timeStr += "0";
		timeStr += std::to_string(min) + ":";
		if (s < 10)
			timeStr += "0";
		timeStr += std::to_string(s);

		return timeStr;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Convert an eObjectType to a gObjectType
	/////////////////////////////////////////////////////////////////////

	gObjectType toGraphicType(eObjectType type)
	{
		switch (type)
		{
			case BULLET: return G_BULLET;
			case RESPAWN: return G_RESPAWN;
			case SPEED_FIELD: return G_SPEEDFIELD;
			case WALL: return G_WALL;
			case PLAYER: return G_PLAYER;
			case GRAVITY_FIELD: return G_GRAVITYFIELD;
			case FLAG: return G_FLAG;
			case BOMB: return G_BOMB;
			case CAPTURE: return G_CAPTURE;
			default: return G_NONE;
		}
	}
}
