/********************************************************************
**
**              GraphicMinimap.cpp
**              Created by : Vial Joris
**
********************************************************************/

// TODO - Fix Z position

#include "GraphicMinimap.hpp"
#include "GraphicConfiguration.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "GraphicDefines.hpp"
#include "OpenGLContext.hpp"

#include "Flag.hpp"
#include "Respawn.hpp"
#include "Capture.hpp"
#include "SpeedField.hpp"

namespace VoidClashGraphics
{
	// Define minimap size and zoom
	#define MINIMAP_SIZE 150.0f
	#define MINIMAP_RECOIL 8.0f

	// Entity size
	#define MINIMAP_PLAYER_SIZE 180.0f
	#define MINIMAP_PLAYER_FLAG 220.0f

	// Define minimap attributes
	#define MINIMAP_POS_X 0.0f
	#define MINIMAP_POS_Y 0.0f
	#define MINIMAP_SCALE 1.0f

	#define MINIMAP_BACKGROUND_ALPHA 0.70f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicMinimap::GraphicMinimap(OpenGLContext *context) :
		GraphicHudEntity(context)
	{

	}

	GraphicMinimap::~GraphicMinimap(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::start(ogl::Camera *camera)
	{
		_camera = camera;
		_totalTime = 0.0f;

		// Create OpenGL ressources

		_quad = new ogl::Quad();
		_quad->create();

		_render = new ogl::Framebuffer();
		_render->create(GCONFIG.winX, GCONFIG.winY, true);

		_gameCamera = new ogl::Camera();

		// Keep reference on shaders and textures

		_shader = S_Shader->addShader("MapQuadColor");
		_shaderFinal = S_Shader->addShader("Minimap");
		_entityShader = S_Shader->addShader("MapQuadWorldColor");

		_background = S_Texture->addTexture("Minimap_background");
		_foreground= S_Texture->addTexture("Minimap_foreground");
		_player = S_Texture->addTexture("Minimap_player");
		_flag = S_Texture->addTexture("Minimap_flag");
		_capture = S_Texture->addTexture("Default");
		_wall = S_Texture->addTexture("Default");
		_respawn = S_Texture->addTexture("Default");
		_speedfield = S_Texture->addTexture("Default");

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::update(float deltatime)
	{
		_totalTime += deltatime;

		// Bind and clear framebuffer
		_render->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		// Bind rendering quad
		_quad->bind();

		// Draw background
		glDisable(GL_DEPTH_TEST);
		drawBackground();

		// Draw entites
		glEnable(GL_DEPTH_TEST);
		drawEntities();
		glDisable(GL_DEPTH_TEST);

		// Draw foreground
		drawForeground();

		// Final draw
		drawOnScreen();

		// Reset OpenGL states
		clearOpenGLStates();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw on screen
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawOnScreen(void)
	{
		// Unbind framebuffer
		_render->unbind();

		// Bind previously saved framebuffer (GraphicHud::update)
		_context->restorePreviousFramebuffer();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set quad propery
		_quad->resetTransform();
		_quad->translate(0.5f, 0.5f);
		_quad->scale(2.0f, 2.0f);
		_quad->scale(MINIMAP_SIZE * MINIMAP_SCALE, MINIMAP_SIZE * MINIMAP_SCALE);
		_quad->translate(MINIMAP_POS_X, MINIMAP_POS_Y);

		// Bind texture
		_render->bindTexture();

		// Set shader params and bind it
		_shaderFinal->bind();
		_shaderFinal->setParameter("Model", _quad->getTransform());
		_shaderFinal->setParameter("Texture", 0);
		_shaderFinal->setParameter("Camera", _camera->getFinal());

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw background
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawBackground(void)
	{
		// Resize and place the quad
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);

		// Set shader params and bind it
		_shader->setParameter("Model", _quad->getTransform());
		_shader->setParameter("Color", glm::vec4(1, 1, 1, MINIMAP_BACKGROUND_ALPHA));

		// Bind texture
		_background->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw foreground
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawForeground(void)
	{
		// Set quad property
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);

		// Set shader params and bind it
		_shader->setParameter("Model", _quad->getTransform());
		_shader->setParameter("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Bind texture
		_foreground->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update the game camera
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::updateGameCamera(void)
	{
		_gameCamera->resetTransform();
		_gameCamera->setupPerspective(45.0f,
			(float)GCONFIG.winX / (float)GCONFIG.winX,
			100.0f,
			50000.0f);

		// Center the game on the player
		Player *p = S_Map->getPlayerFollowed().get();
		if (p != NULL)
		{
			//_gameCamera->translate(-p->getX(), -p->getY());
			_gameCamera->lookAt(glm::vec3(p->getX(), p->getY(), COEFF_ZOOM_MULT * MINIMAP_RECOIL),
				glm::vec3(p->getX(), p->getY(), 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
		}

		// Compute final matrix
		_gameCamera->update();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw entities
	/////////////////////////////////////////////////////////////////////

	// Can be optimized by drawing entities only shown on the minimap area

	void GraphicMinimap::drawEntities(void)
	{
		// Update game camera
		updateGameCamera();

		// Bind entity shader
		_entityShader->bind();
		_entityShader->setParameter("Texture", 0);
		_entityShader->setParameter("Camera", _gameCamera->getFinal());

		// Walls
		for (auto it = S_Map->getWalls()->begin(); it != S_Map->getWalls()->end(); ++it)
			drawWall((*it).get());

		// Other entities
		for (auto it = S_Map->getElems()->begin(); it != S_Map->getElems()->end(); ++it)
		{
			switch ((*it)->getType())
			{
			case FLAG: drawFlag((*it).get()); break;
			case RESPAWN: drawRespawn((*it).get()); break;
			case CAPTURE: drawCapture((*it).get()); break;
			case SPEED_FIELD: drawSpeedfield((*it).get()); break;
			default: break;
			}
		}

		// Players at the end :-)
		glDisable(GL_DEPTH_TEST);
		for (auto it = S_Map->getPlayers()->begin(); it != S_Map->getPlayers()->end(); ++it)
			drawPlayer((*it).get());
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw walls
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawWall(AObject *e)
	{
		// Get Wall pointer
		Wall *w = dynamic_cast<Wall*>(e);
		if (w == NULL)
			return;

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);
		_quad->scale(w->getLen() * 0.5f, 40.0f);
		_quad->translate(w->getLen() * 0.5, 20.0f, Z_WALL);
		_quad->rotate(w->getAngle(), ogl::Zaxis);
		_quad->translate(w->getX(), w->getY());

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", glm::vec4(0, 1.0f, 1.0f, 1.0f));

		// Bind texture
		_wall->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw players
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawPlayer(AObject *e)
	{
		// Get player
		Player *p = dynamic_cast<Player*>(e);
		if (p == NULL || p->getTeam() <= 0 || p->getLife() <= 0)
			return;

		// Set quad propery
		float size = MINIMAP_PLAYER_SIZE;

		// If we are drawing the player followed, scale it a bit more :)
		size = (p == S_Map->getPlayerFollowed().get()) ? (size + 20.0f) : (size);
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);
		_quad->scale(size, size);
		_quad->translate(p->getX(), p->getY(), Z_PLAYER);

		// Get player color
		Player* mainPlayer = S_Map->getPlayerFollowed().get();
		glm::vec4 c;
		getTeamColor(c, p->getTeam(), p == mainPlayer);

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", c.r, c.g, c.b, 1.0f);

		// Bind texture
		_player->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw flags
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawFlag(AObject *e)
	{
		// Do we have to display flags ?
		if (!S_Map->getMode()->getProperty()->flag)
			return;

		// Get flag
		Flag *f = dynamic_cast<Flag*>(e);
		if (e == NULL)
			return;

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);
		_quad->scale(MINIMAP_PLAYER_FLAG, MINIMAP_PLAYER_FLAG);
		_quad->rotate(_totalTime * 360.0f, ogl::Zaxis);
		_quad->translate(f->getX(), f->getY(), Z_FLAG);

		// Get flag color
		glm::vec4 c;
		getTeamColor(c, f->getTeam());

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", c.r, c.g, c.b, 1.0f);

		// Bind texture
		_flag->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw respawns
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawRespawn(AObject *e)
	{
		// Do we have to display respawn ?
		if (!S_Map->getMode()->getProperty()->team)
			return;

		// Get flag
		Respawn *r = dynamic_cast<Respawn*>(e);
		if (e == NULL)
			return;

		// Don't display default respawn
		if (r->getTeam() <= 0)
			return;

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(r->getWidth(), r->getHeight());
		_quad->translate(
			r->getX() + r->getWidth() * 0.5f,
			r->getY() + r->getHeight() * 0.5,
			Z_RESPAWN);

		// Get respawn color
		glm::vec4 c;
		getTeamColor(c, r->getTeam());
		c.r *= 0.5f; c.b *= 0.5f; c.g *= 0.5f;

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", c.r, c.g, c.b, 1.0f);

		// Bind texture
		_respawn->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw captures
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawCapture(AObject *e)
	{
		// Do we have to display capture ?
		if (!S_Map->getMode()->getProperty()->capture)
			return;

		// Get capture
		Capture *ca = dynamic_cast<Capture*>(e);
		if (ca == NULL)
			return;

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(ca->getWidth(), ca->getHeight());
		_quad->translate(
			ca->getX() + ca->getWidth() * 0.5f,
			ca->getY() + ca->getHeight() * 0.5,
			Z_CAPTURE);

		// Get capture color
		glm::vec4 c;

		// No team controlling
		if (ca->getTeamControlling() == 0)
		{
			c.r = 1;
			c.g = 1;
			c.b = 1;
		}
		// Get controlling team color
		else
		{
			getTeamColor(c, ca->getTeamControlling());
		}

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", c.r, c.g, c.b, 1.0f);

		// Bind texture
		_capture->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw speedfield
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::drawSpeedfield(AObject *e)
	{
		// Get capture
		SpeedField *s = dynamic_cast<SpeedField*>(e);
		if (s == NULL)
			return;

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(s->getWidth(), s->getHeight());
		_quad->translate(
			s->getX() + s->getWidth() * 0.5f,
			s->getY() + s->getHeight() * 0.5,
			Z_SPEEDFIELD);

		// Get speedfield color
		glm::vec4 c(0, 0.2, 0.2, 1);

		// Set shader uniforms
		_entityShader->setParameter("Model", _quad->getTransform());
		_entityShader->setParameter("Color", c.r, c.g, c.b, 1.0f);

		// Bind texture
		_speedfield->bind();

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::clearOpenGLStates(void)
	{
		_quad->unbind();
		_shaderFinal->unbind();
		_background->unbind();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicMinimap::stop(void)
	{
		// Delete OpenGL ressources
		_quad->destroy();
		_render->destroy();

		delete _quad;
		delete _render;
		delete _gameCamera;
	}
}
