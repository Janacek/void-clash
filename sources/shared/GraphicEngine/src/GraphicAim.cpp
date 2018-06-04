/********************************************************************
**
**              GraphicAim.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicAim.hpp"
#include "GraphicConfiguration.hpp"
#include "GUIManager.hpp"
#include "Map.hpp"
#include "GraphicHelper.hpp"
#include "HudRessources.hpp"

namespace VoidClashGraphics
{
	// Global camera for coordinate conversion
	extern ogl::Camera globalCamera;

	#define AIM_SIZE 20.0f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicAim::GraphicAim(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicAim::~GraphicAim(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::start(ogl::Camera *camera)
	{
		_camera = camera;

		// Load texture
		S_Texture->addTexture("PlayerAim");

		// Load shader
		_shader = S_Shader->addShader("PlayerAim");

		// Create quad
		_aim = new ogl::Quad();
		_aim->create();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::update(float deltatime)
	{
		// Get the current player
		Player *player = S_Map->getCurrentPlayer().get();
		if (player == NULL)
			return;

		/////// Check if we need to display the aim ///////

		if (// Aim display disabled in graphic configuration
			GCONFIG.displayaim == false ||
			// Player is dead
			player->getLife() <= 0 ||
			// Player is spectator
			player->getTeam() == 0 ||
			// Menu are displayed
			S_GUI->isDisplayed() == true)
		{
			return;
		}

		// Update the aim
		updateAim();

		// Display
		displayAim(deltatime);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update aim shape
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::updateAim(void)
	{
		Player *player = S_Map->getCurrentPlayer().get();
		// NULL check already done before

		// Keep total time
		_totalTime = player->getTimeSinceCreation() * 0.000001f;

		// Get screen player position
		_playerPos.x = player->getX(); _playerPos.y = player->getY();
		float pX = 0.0f;
		float pY = 0.0f;
		worldToScreen(_playerPos.x, -_playerPos.y, pX, pY, &globalCamera);

		// Calculate the size of a player on the screen
		float sX = 0.0f;
		float sY = 0.0f;
		worldToScreen(
			player->getX() + player->getRadius(),
			-player->getY() + player->getRadius(),
			sX, sY, &globalCamera);
		sX = sX - pX;

		// Get mouse position
		const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());

		// Calculate lenght
		float lenght = 0.0f;
		lenght = std::sqrt(std::pow(mouse.x - pX, 2.0f) + std::pow(mouse.y - pY, 2.0));
		lenght *= 0.90f;
		_scale = 5.0f * (lenght / 256.0f);

		// Don't display aim if the lenght is too short
		if (lenght < sX)
		{
			_aim->resetTransform();
			return;
		}

		// Calculate rotation angle
		float rotation = 0.0f;
		rotation = -G_TO_DEGREE(std::atan2(mouse.x - pX, mouse.y - pY)) - 270;

		// Transform aim
		_aim->resetTransform();
		_aim->translate(0.5f, 0.0f);
		_aim->scale(lenght, AIM_SIZE);
		_aim->translate(sX, 0.0f);
		_aim->rotate(rotation, ogl::Zaxis);
		_aim->translate(pX, pY);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Display aim
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::displayAim(float deltatime)
	{
		(void)deltatime;

		// Update shader
		_shader->bind();

		_shader->setParameter("Model", _aim->getTransform());
		_shader->setParameter("HudCamera", _camera->getFinal());

		_shader->setParameter("PlayerPosition", _playerPos);
		_shader->setParameter("Scale", _scale);
		_shader->setParameter("TotalTime", _totalTime);

		if (S_Map->getCurrentPlayer()->getTeam() == 2)
			_shader->setParameter("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		else
			_shader->setParameter("Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		// Bind texture
		S_Texture->bindTexture("PlayerAim");

		// Draw the aim
		_aim->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Clear OpenGL States
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::clearOpenGLStates(void)
	{
		_shader->unbind();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicAim::stop(void)
	{
		_aim->destroy();
		safeDelete(_aim);
	}
}
