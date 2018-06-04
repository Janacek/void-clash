/********************************************************************
**
**              GraphicHudTop.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicHudTop.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "GraphicConfiguration.hpp"
#include "OpenGLContext.hpp"

/////// Game configuration ///////
extern t_config *G_conf;

namespace VoidClashGraphics
{
	/////// Define hud propery ///////

	#define TOP_HUD_WIDTH 600
	#define TOP_HUD_HEIGHT 70
	#define TOP_HUD_POS_X (GCONFIG.winX * 0.5f)
	#define TOP_HUD_POS_Y (TOP_HUD_HEIGHT * 0.5f)
	#define TOP_HUD_SCALE 1.0f
	#define HUD_MAX_SCORE 9999

	#define LABEL_POS_Y (TOP_HUD_HEIGHT * 0.6f)
	#define VALUE_POS_Y (TOP_HUD_HEIGHT * 0.18f)
	#define LABEL_SCALE_X 0.4f
	#define LABEL_SCALE_Y 0.4f
	#define VALUE_SCALE_X 0.45f
	#define VALUE_SCALE_Y 0.5f

	/////////////////////////////////////////////////////////////////////
	/////	Ctor / Dtor
	/////////////////////////////////////////////////////////////////////

	GraphicHudTop::GraphicHudTop(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicHudTop::~GraphicHudTop(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::start(ogl::Camera *camera)
	{
		_camera = camera;

		// Create OpenGL ressources

		_localCamera = new ogl::Camera();
		_localCamera->setupOrthographic(TOP_HUD_WIDTH, TOP_HUD_HEIGHT, false);
		_localCamera->update();

		_quad = new ogl::Quad();
		_quad->create();

		_render = new ogl::Framebuffer();
		_render->create(TOP_HUD_WIDTH, TOP_HUD_HEIGHT, false);

		// Keep reference on shaders and textures

		_shader = S_Shader->addShader("MapQuad");
		_shaderFinal = S_Shader->addShader("HudTop");
		_team = S_Texture->addTexture("ScoreTeam");
		_solo = S_Texture->addTexture("ScoreNoTeam");

		/////// Create text ///////
		std::string font("ExoSemiBold.otf");

		// Time

		_timeRoundLeftLabel = new ogl::Text();
		_timeRoundLeftLabel->create(S_Font->getFont(font), "", _localCamera);

		_timeRoundLeft = new ogl::Text();
		_timeRoundLeft->create(S_Font->getFont(font), "", _localCamera);

		// Score

		_score1 = new ogl::Text();
		_score1->create(S_Font->getFont(font), "", _localCamera);

		_score2 = new ogl::Text();
		_score2->create(S_Font->getFont(font), "", _localCamera);

		// Players alive

		_aliveLabel = new ogl::Text();
		_aliveLabel->create(S_Font->getFont(font), "En vie", _localCamera);

		_alive1 = new ogl::Text();
		_alive1->create(S_Font->getFont(font), "", _localCamera);

		_alive2 = new ogl::Text();
		_alive2->create(S_Font->getFont(font), "", _localCamera);

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::update(float deltatime)
	{
		(void)deltatime;

		// Get current mode
		updateMode();

		// Bind and clear framebuffer
		_render->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, TOP_HUD_WIDTH, TOP_HUD_HEIGHT);

		// Draw background
		drawBackground();

		// Draw time left or round left
		if (getMiddlePartType() == HUD_TOP_TIME || S_Map->getWarmupTime().asSeconds() >= 1)
			drawTimeLeft();
		else // HUD_TOP_ROUND
			drawRoundLeft();

		// Draw score
		if (S_Map->getMode()->getProperty()->team == true)
			drawScore();

		// Draw players alive
		drawPlayersAlive();

		// Final draw
		drawOnScreen();

		// Reset OpenGL states
		clearOpenGLStates();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw on screen
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawOnScreen(void)
	{
		// Unbind framebuffer
		_render->unbind();

		// Bind previously saved framebuffer (GraphicHud::update)
		_context->restorePreviousFramebuffer();

		glViewport(0, 0, GCONFIG.winX, GCONFIG.winY);

		// Set quad propery
		_quad->resetTransform();
		_quad->scale(
			TOP_HUD_WIDTH * TOP_HUD_SCALE,
			TOP_HUD_HEIGHT * TOP_HUD_SCALE);
		_quad->translate(TOP_HUD_POS_X, TOP_HUD_POS_Y);

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

	void GraphicHudTop::drawBackground(void)
	{
		// Set quad property
		_quad->resetTransform();
		_quad->scale(2.0f, 2.0f);

		// Bind texture according to game mode
		bindTexture();

		// Bind and update shader
		_shader->bind();
		_shader->setParameter("Texture", 0);
		_shader->setParameter("Model", _quad->getTransform());

		// Draw
		_quad->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Check whether we have to display time or round number left
	/////////////////////////////////////////////////////////////////////

	e_hudTopMiddlePart GraphicHudTop::getMiddlePartType(void)
	{
		if (S_Map->getMode()->getProperty()->respawnTime == -1)
			return HUD_TOP_ROUND;
		return HUD_TOP_TIME;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Setup time left
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawTimeLeft(void)
	{
		// Convert time to string (clamp at 99h59m59s)
		float time = 0.0f;

		// Warmup
		time = std::min(359999.0f, S_Map->getWarmupTime().asSeconds());
		// In game
		if (time <= 1)
			time = std::min(359999.0f, S_Map->getTimeRemaningOnMap().asSeconds());

		// Convert to string
		std::string timeStr = "Termin�";
		if (time > 0)
			timeStr = timeToString(std::max(0.0f, time));

		// Set string
		if (S_Map->getWarmupTime().asSeconds() >= 1)
			_timeRoundLeftLabel->setString("Echauffement");
		else
			_timeRoundLeftLabel->setString("Temps");
		_timeRoundLeft->setString(timeStr);

		// Set color
		if (time < 30)
			_timeRoundLeft->setColor(glm::vec4(1, 0, 0, 1));
		else if (time < 60)
			_timeRoundLeft->setColor(glm::vec4(1, 0.5, 0, 1));
		else
			_timeRoundLeft->setColor(glm::vec4(1, 1, 1, 1));

		// Set text property
		_timeRoundLeft->resetTransform();
		_timeRoundLeftLabel->resetTransform();

		// Draw
		drawTimeRoundLeft();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Setup round left
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawRoundLeft(void)
	{
		// Convert round number to string
		int roundLeft = std::max(0, G_conf->game->round_nb -
			S_Map->getMode()->getRoundNumber() + 1);
		roundLeft = std::min(HUD_MAX_SCORE, roundLeft);
		std::string roundStr = std::to_string(roundLeft);

		// Set string
		_timeRoundLeftLabel->setString("Manches restantes");
		if (roundLeft < 10)
			_timeRoundLeftLabel->setString("Manche restante");

		_timeRoundLeft->setString(roundStr);
		if (roundLeft == 1)
			_timeRoundLeft->setString("Dernier !");

		// Color
		_timeRoundLeft->setColor(glm::vec4(1, 1, 1, 1));
		if (roundLeft == 0)
			_timeRoundLeft->setColor(glm::vec4(1, 0, 0, 1));

		// Draw
		drawTimeRoundLeft();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Here happen all the draw stuff about time and rounds
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawTimeRoundLeft(void)
	{
		// Common
		_timeRoundLeft->resetTransform();
		_timeRoundLeftLabel->resetTransform();

		_timeRoundLeftLabel->setScale(LABEL_SCALE_X, LABEL_SCALE_Y);
		_timeRoundLeft->setScale(VALUE_SCALE_X, VALUE_SCALE_Y);

		if (_mode == HUD_TOP_SOLO)
		{
			// Label
			_timeRoundLeftLabel->setPosition(
				TOP_HUD_WIDTH * 0.25f - _timeRoundLeftLabel->getSize().x * 0.5f,
				LABEL_POS_Y);

			// Timer
			_timeRoundLeft->setPosition(
				TOP_HUD_WIDTH * 0.25f - _timeRoundLeft->getSize().x * 0.5f,
				VALUE_POS_Y);
		}
		else
		{
			// Label
			_timeRoundLeftLabel->setPosition(
				TOP_HUD_WIDTH * 0.5f - _timeRoundLeftLabel->getSize().x * 0.5f,
				LABEL_POS_Y);

			// Timer
			_timeRoundLeft->setPosition(
				TOP_HUD_WIDTH * 0.5f - _timeRoundLeft->getSize().x * 0.5f,
				VALUE_POS_Y);
		}

		// Draw
		_timeRoundLeftLabel->draw();
		_timeRoundLeft->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw score
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawScore(void)
	{
		/////// TEAM 1 ///////
		{
			// Convert scores to string
			std::string score1 =
				std::to_string(std::min(HUD_MAX_SCORE, S_Map->getScore().first));

			// Set string
			_score1->setString(score1);

			// Set text property
			_score1->resetTransform();
			_score1->setScale(0.60f, 0.75f);
			_score1->setPosition(
				TOP_HUD_WIDTH * 0.3f - _score1->getSize().x * 0.5f,
				TOP_HUD_HEIGHT * 0.5f - _score1->getSize().y * 0.5f);

			// Draw
			_score1->draw();
		}

		/////// TEAM 2 ///////
		{
			// Convert scores to string
			std::string score2 =
				std::to_string(std::min(HUD_MAX_SCORE, S_Map->getScore().second));

			// Set string
			_score2->setString(score2);

			// Set text property
			_score2->resetTransform();
			_score2->setScale(0.60f, 0.75f);
			_score2->setPosition(
				TOP_HUD_WIDTH * 0.7f - _score2->getSize().x * 0.5f,
				TOP_HUD_HEIGHT * 0.5f - _score2->getSize().y * 0.5f);

			// Draw
			_score2->draw();
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw players number alive
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::drawPlayersAlive(void)
	{
		/////// TEAM 1 ///////
		{
			// -1 stand for solo mods
			int team = (_mode == HUD_TOP_SOLO) ? (-1) : (1);

			// Convert players alive number to string
			std::string alive1 =
				std::to_string(std::min(HUD_MAX_SCORE, S_Map->getAlivePlayerNumber(team)));

			// Set string
			_alive1->setString(alive1);

			// Set text property
			_aliveLabel->resetTransform();
			_aliveLabel->setScale(LABEL_SCALE_X, LABEL_SCALE_Y);

			// SOLO
			if (_mode == HUD_TOP_SOLO)
			{
				// Label
				_aliveLabel->setPosition(
					TOP_HUD_WIDTH * 0.75f - _aliveLabel->getSize().x * 0.5f,
					LABEL_POS_Y);

				// Players number
				_alive1->resetTransform();
				_alive1->setScale(VALUE_SCALE_X, VALUE_SCALE_Y);
				_alive1->setPosition(
					TOP_HUD_WIDTH * 0.75f - _alive1->getSize().x * 0.5f,
					VALUE_POS_Y);
			}
			// TEAM
			else
			{
				// Label
				_aliveLabel->resetTransform();
				_aliveLabel->setScale(LABEL_SCALE_X, LABEL_SCALE_X);
				_aliveLabel->setPosition(
					TOP_HUD_WIDTH * 0.1f - _aliveLabel->getSize().x * 0.5f,
					LABEL_POS_Y);

				// Players number
				_alive1->resetTransform();
				_alive1->setScale(VALUE_SCALE_X, VALUE_SCALE_Y);
				_alive1->setPosition(
					TOP_HUD_WIDTH * 0.1f - _alive1->getSize().x * 0.5f,
					VALUE_POS_Y);
			}

			// Draw
			_aliveLabel->draw();
			_alive1->draw();
		}

		// Nothing more to display
		if (_mode == HUD_TOP_SOLO)
			return;

		// Set text property
		_aliveLabel->resetTransform();
		_aliveLabel->setScale(LABEL_SCALE_X, LABEL_SCALE_Y);

		/////// TEAM 2 ///////
		{
			// Convert players alive number to string
			std::string alive2 =
				std::to_string(std::min(HUD_MAX_SCORE, S_Map->getAlivePlayerNumber(2)));

			// Set string
			_alive2->setString(alive2);

			// Label
			_aliveLabel->setPosition(
				TOP_HUD_WIDTH * 0.9f - _aliveLabel->getSize().x * 0.5f,
				LABEL_POS_Y);

			// Players number
			_alive2->resetTransform();
			_alive2->setScale(VALUE_SCALE_X, VALUE_SCALE_Y);
			_alive2->setPosition(
				TOP_HUD_WIDTH * 0.9f - _alive2->getSize().x * 0.5f,
				VALUE_POS_Y);

			// Draw
			_aliveLabel->draw();
			_alive2->draw();
		}
	}

	///////////////////////////////////////////////	//////////////////////
	/////	Update mode
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::updateMode(void)
	{
		_mode = (S_Map->getMode()->getProperty()->team == true) ?
			(HUD_TOP_TEAM) : (HUD_TOP_SOLO);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Bind texture
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::bindTexture()
	{
		if (_mode == HUD_TOP_SOLO)
			_solo->bind();
		else
			_team->bind();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Clear OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::clearOpenGLStates()
	{
		_quad->unbind();
		_team->unbind();
		_shader->unbind();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicHudTop::stop(void)
	{
		_quad->destroy();
		_render->destroy();
		delete _localCamera;
		delete _quad;
		delete _render;

		_timeRoundLeft->destroy();
		_timeRoundLeftLabel->destroy();
		delete _timeRoundLeft;
		delete _timeRoundLeftLabel;

		_score1->destroy();
		_score2->destroy();
		delete _score1;
		delete _score2;

		_aliveLabel->destroy();
		_alive1->destroy();
		_alive2->destroy();
		delete _aliveLabel;
		delete _alive1;
		delete _alive2;
	}
}
