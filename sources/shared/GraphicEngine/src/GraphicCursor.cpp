/********************************************************************
**
**              GraphicCursor.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicCursor.hpp"
#include "GUIManager.hpp"
#include "HudRessources.hpp"
#include "Map.hpp"
#include "GraphicHelper.hpp"

namespace VoidClashGraphics
{
	#define CURSOR_SIZE 17.5f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicCursor::GraphicCursor(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicCursor::~GraphicCursor(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::start(ogl::Camera *camera)
	{
		// Keep camera
		_camera = camera;

		// Load shader
		_shader = S_Shader->addShader("MapQuadWorldColor");

		// Create quad
		_quad = new ogl::Quad();
		_quad->create();

		// Load cursors
		loadCursors();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Load cursors
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::loadCursors(void)
	{
		// Game cursor
		S_Texture->addTexture("Cursor");
		_cursors[CURSOR_GAME] = "Cursor";
		_scales[CURSOR_GAME] = 1.0f;

		// Menu cursor
		S_Texture->addTexture("CursorMenu");
		_cursors[CURSOR_MENU] = "CursorMenu";
		_scales[CURSOR_MENU] = 2.5f;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::update(float deltatime)
	{
		(void)deltatime;

		// Update the cursor to display
		updateCurrentCursor();

		// Display it
		displayCursor();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update the current to display
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::updateCurrentCursor(void)
	{
		// Menu are opened
		if (S_GUI->isDisplayed() == true)
		{
			_currentCursor = CURSOR_MENU;
			return;
		}

		// Menu are closed
		_currentCursor = CURSOR_GAME;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Display the cursor
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::displayCursor(void)
	{
		// Bind the texture
		S_Texture->bindTexture(_cursors[_currentCursor]);

		// Get mouse position
		const sf::Vector2i &mouse(HudRessources::getInstance()->getMousePosition());

		// Update the quad
		_quad->resetTransform();
		_quad->scale(CURSOR_SIZE * _scales[_currentCursor],
			-CURSOR_SIZE * _scales[_currentCursor]);
		_quad->translate(mouse.x, mouse.y);

		// Update the shader
		_shader->bind();
		_shader->setParameter("Model", _quad->getTransform());
		_shader->setParameter("Camera", _camera->getFinal());

		_shader->setParameter("Color", 1.0f, 1.0f, 1.0f, 1.0f);
		if (S_Map->getCurrentPlayer() && S_Map->getCurrentPlayer()->getTeam() == 1)
			_shader->setParameter("Color", 0.0f, 1.0f, 0.0f, 1.0f);
		else if (S_Map->getCurrentPlayer() && S_Map->getCurrentPlayer()->getTeam() == 2)
			_shader->setParameter("Color", 1.0f, 0.0f, 0.0f, 1.0f);

		// Draw
		_quad->draw();

		// Clear OpenGL states
		clearOpenGLStates();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL States
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::clearOpenGLStates(void)
	{
		_shader->unbind();
		S_Texture->unbindTexture();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicCursor::stop(void)
	{
		_quad->destroy();
		safeDelete(_quad);

		// Delete cursors
		_cursors.clear();
		_scales.clear();
	}
}
