/********************************************************************
**
**              GraphicRespawnCooldown.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicRespawnCooldown.hpp"
#include "GraphicConfiguration.hpp"
#include "Map.hpp"
#include "GraphicHelper.hpp"
#include "ConfigParser.hpp"
#include "Log.hpp"
#include "GUIManager.hpp"

// Global game configuration
extern t_config	*G_conf;

namespace VoidClashGraphics
{
	// Message attributes
	#define RESPAWN_MESSAGE_POSX (GCONFIG.winX * 0.5f)
	#define RESPAWN_MESSAGE_POSY (GCONFIG.winY * 0.85f)
	#define RESPAWN_MESSAGE_GAP (50.0f)
	#define RESPAWN_MESSAGE_SCALE (1.5f)
	#define RESPAWN_BACKGROUND_ALPHA (0.75f)

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicRespawnCooldown::GraphicRespawnCooldown(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicRespawnCooldown::~GraphicRespawnCooldown(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::start(ogl::Camera *camera)
	{
		std::string font("UnispaceRegular.ttf");
		_text = new ogl::Text();
		_text->create(S_Font->getFont(font), "", camera);

		_label = new ogl::Text();
		_label->create(S_Font->getFont(font), "", camera);
		_label->resetTransform();
		_label->setString("Réapparition");
		_label->setScale(RESPAWN_MESSAGE_SCALE, -RESPAWN_MESSAGE_SCALE);
		_label->setPosition(RESPAWN_MESSAGE_POSX - _label->getSize().x * 0.5f,
			RESPAWN_MESSAGE_POSY - RESPAWN_MESSAGE_GAP);

		// Create background
		_background = new ogl::Quad();
		_background->create();
		_background->resetTransform();
		_background->scale(2.0, 2.0);

		// Get shader
		_shader = S_Shader->addShader("ColoredBackground");

		// Reset attributes
		_color = glm::vec4(0, 0, 0, 0);
		_alpha = 0.0f;
		_ratio = 0.0f;

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::update(float deltatime)
	{
		(void)deltatime;

		// Get player followed
		Player *p = S_Map->getPlayerFollowed().get();
		if (p == NULL)
			return;

		// Don't display anything if the player is alive
		if (p->isRespawning() == false)
			return;

		// Calculate the current progress
		calculateRatio();

		// Calculate the current alpha
		calculateAlpha();

		// Calculate the current color
		calculateColor();

		// Background
		if (S_GUI->isDisplayed() == false)
			displayBackground();

		// Text
		displayLabel();
		displayProgression();

		// Clear OpenGL States
		clearOpenGLStates();
	}

	/////////////////////////////////////////////////////////////////////
	///// Display the main text ("Respawn")
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::displayLabel(void)
	{
		// Update color
		glm::vec4 color = _color;
		color.a *= 2.0f;
		_label->setColor(color);

		// Display label
		_label->draw();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	///// Display the text progresson the the respawn time left
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::displayProgression(void)
	{
		// Update text
		_text->setString(prepareText());

		glm::vec4 color = _color;
		color.a *= 2.0f;
		_text->setColor(color);

		// Update position
		_text->resetTransform();
		_text->setScale(RESPAWN_MESSAGE_SCALE, -RESPAWN_MESSAGE_SCALE);
		_text->setPosition(
			RESPAWN_MESSAGE_POSX - _text->getSize().x * 0.5f,
			RESPAWN_MESSAGE_POSY + RESPAWN_MESSAGE_GAP);

		// Draw the text
		_text->draw();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	///// Format the string
	/////////////////////////////////////////////////////////////////////

	std::string GraphicRespawnCooldown::prepareText(void)
	{
		// Progress (ratio * 20.0 explained in 'calculateRatio()')
		std::string progress = std::to_string((int)(_ratio * 20)) + "%";
		if (_ratio * 20 < 10)
			progress = "0" + progress;

		// Final message
		std::string finalStr = progress;
		return finalStr;
	}

	/////////////////////////////////////////////////////////////////////
	///// Display the background
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::displayBackground(void)
	{
		// Bind shader
		_shader->bind();

		// Set color (transparent black)
		_shader->setParameter("Color", 0.0f, 0.0f, 0.0f, _alpha);
		_shader->setParameter("Model", _background->getTransform());

		// Draw background
		_background->draw();

		// Clear OpenGL
		_shader->unbind();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	///// Calculate the alpha
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::calculateAlpha(void)
	{
		// Get player followed
		Player *p = S_Map->getPlayerFollowed().get();
		// Null check has been done before

		// Calculate transparency
		float maxAlpha = RESPAWN_BACKGROUND_ALPHA;
		_alpha = maxAlpha;

		// Convert to milliseconds
		float configRespawnTime = S_Map->getMode()->getProperty()->respawnTime * 1000;

		// Smooth transition
		if (-p->getTimeBeforeRespawn() < 500)
			_alpha = std::min(maxAlpha, -p->getTimeBeforeRespawn() * 0.001f * 2.0f);
		else if (-p->getTimeBeforeRespawn() > configRespawnTime - 500)
			_alpha = std::min(maxAlpha,
			(configRespawnTime - (-p->getTimeBeforeRespawn())) * 0.001f *  2.0f);
	}

	/////////////////////////////////////////////////////////////////////
	///// Calculate the current progress
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::calculateRatio(void)
	{
		// Get player followed
		Player *p = S_Map->getPlayerFollowed().get();
		// Null check has been done before

		// Don't display anything if the player is alive
		if (p->isRespawning() == false)
			return;

		// Get time before respawn if any
		float time = std::max(0.0f, -p->getTimeBeforeRespawn());

		// Calculate the caracter number to display on each side
		// ratio = [0 -> 100]
		_ratio = 100 - time / S_Map->getMode()->getProperty()->respawnTime * 0.1f;

		// ratioL = [0 -> 5]
		_ratio /= 20.0f;
	}

	/////////////////////////////////////////////////////////////////////
	///// Calculate the final color
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::calculateColor(void)
	{
		// Set color
		if (_ratio < 2.0f)
			_color = glm::vec4(1, 0, 0, _alpha);
		else if (_ratio < 4.0f)
			_color = glm::vec4(1, 0.5, 0, _alpha);
		else if (_ratio < 6.0f)
			_color = glm::vec4(0, 1, 0, _alpha);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::stop(void)
	{
		_text->destroy();
		safeDelete(_text);

		_label->destroy();
		safeDelete(_label);

		_background->destroy();
		safeDelete(_background);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicRespawnCooldown::clearOpenGLStates(void)
	{
		_shader->unbind();
	}
}
