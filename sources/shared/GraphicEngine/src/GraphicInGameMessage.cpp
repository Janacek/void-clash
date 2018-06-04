/********************************************************************
**
**              GraphicInGameMessage.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicInGameMessage.hpp"
#include "GraphicConfiguration.hpp"
#include "GraphicHelper.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Capture.hpp"
#include "Flag.hpp"

namespace VoidClashGraphics
{
	// Speed for scale and fade out
	#define INGAME_MESSAGE_ANIMATION_SPEED 2.0f

	// Message position
	#define IN_GAME_MESSAGE_X (GCONFIG.winX * 0.5f)
	#define IN_GAME_MESSAGE_Y 150.0f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicInGameMessage::GraphicInGameMessage(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicInGameMessage::~GraphicInGameMessage(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::start(ogl::Camera *camera)
	{
		(void)camera;

		// We want ogl::Text to create a camera
		_camera = NULL;

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::update(float deltatime)
	{
		// Display message only during game
		if (S_Map->getWarmupTime().asSeconds() > 1)
			return;

		// Block new messages by default
		_blockNewMessages = true;

		// If we don't have any messages for now, allow new ones
		if (_messages.size() == 0)
			_blockNewMessages = false;

		// Update messages
		updateMessages(deltatime);

		// Check events
		checkEvents();

		// Display messages
		drawMessages();

		// Reset OpenGL states
		clearOpenGLStates();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Add a new message to display
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::addMessage(const std::string &message)
	{
		std::string font("LektonBold.ttf");

		inGameMessage *ctnr = new inGameMessage;

		ctnr->animated = false;
		ctnr->time = 0;
		ctnr->text = new ogl::Text();
		ctnr->text->create(S_Font->getFont(font), message, _camera);

		_messages.push_back(ctnr);
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update messages
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::updateMessages(float deltatime)
	{
		auto it = _messages.begin();
		auto end = _messages.end();

		while (it != end)
		{
			// Update time
			(*it)->time += deltatime * INGAME_MESSAGE_ANIMATION_SPEED;

			// Run animation
			if ((*it)->animated == false && (*it)->time > 1.0f)
			{
				// If the message is going to disapear, allow new messages
				_blockNewMessages = false;
				(*it)->animated = true;
			}

			// Delete message
			if ((*it)->time > 1.5f)
			{
				// Delete OpenGL ressources
				(*it)->text->destroy();
				delete (*it)->text;

				// Delete message
				delete (*it);
				it = _messages.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Kill streak
	/////////////////////////////////////////////////////////////////////

	bool GraphicInGameMessage::createKillStreakMessage(std::pair<eventType, s_event> &ev)
	{
		// Get players
		Player *killer = (Player *)ev.second.trigger.get();
		Player *killed = (Player *)ev.second.data;

		// Corrupted event
		if (killer == NULL)
			return true;

		// Generate message
		std::string message("");

		// Kill type
		if (killer == killed)
			message = killer->getNick() + " s'est suicid� 0_o";
		else if (killer->getKillsStreak() == 2)
			message = killer->getNick() + " a fait un double kill";
		else if (killer->getKillsStreak() == 3)
			message = killer->getNick() + " a fait un triple kill";
		else if (killer->getKillsStreak() == 4)
			message = killer->getNick() + " a fait un quad kill";
		else if (killer->getKillsStreak() >= 5)
			message = killer->getNick() + " est en feu";

		// Add the message
		addMessage(message);

		// Allow other messages on this frame
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Game start
	/////////////////////////////////////////////////////////////////////

	bool GraphicInGameMessage::createGameStartMessage(std::pair<eventType, s_event> &ev)
	{
		(void)ev;

		// Generate message
		std::string message("Fight !");

		// Add the message
		addMessage(message);

		// Block other messages on this frame
		return false;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Catpure zone
	/////////////////////////////////////////////////////////////////////

	bool GraphicInGameMessage::createCaptureZoneMessage(std::pair<eventType, s_event> &ev)
	{
		// Get capture zone
		Capture *c = (Capture*)ev.second.trigger.get();;

		// Corrupted event
		if (c == NULL)
			return true;

		// Generate message
		std::string message("");

		// A team is capturing a zone
		if (c->getTeamControlling() == 0)
		{
			int *curTeamCapturing = (int*)ev.second.data;

			if (*curTeamCapturing == 1)
				message += "L'�quipe verte est en train de capturer une zone";
			else if (*curTeamCapturing == 2)
				message += "L'�quipe rouge est en train de capturer une zone";
		}
		// A team captures a zone
		else if (c->getTeamControlling() == 1)
			message += "L'�quipe verte a captur� une zone";
		else if (c->getTeamControlling() == 2)
			message += "L'�quipe rouge a captur� une zone";

		// Add the message
		addMessage(message);

		// Allow other messages on this frame
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Flag
	/////////////////////////////////////////////////////////////////////

	bool GraphicInGameMessage::createFlagMessage(std::pair<eventType, s_event> &ev)
	{
		bool ret = true;

		// Get flag
		Flag *f = (Flag*)ev.second.trigger.get();;

		// Corrupted event
		if (f == NULL)
			return true;

		// Generate message
		std::string message("");

		// Team
		if (f->getTeam() == 1)
			message += "Vert : ";
		else if (f->getTeam() == 2)
			message += "Rouge : ";

		// Flag event type
		if (ev.first == ev_CAPTURE_FLAG)
		{
			// Team reversed in capture
			if (f->getTeam() == 1)
				message = "L'�qupe rouge ";
			else if (f->getTeam() == 2)
				message = "L'�quipe verte ";

			message += "marque !";

			// Block other messages on this frame
			ret = false;
		}
		else if (ev.first == ev_TOUCH_FLAG)
			message += "le drapeau a �t� pris";
		else if (ev.first == ev_DROP_FLAG)
			message += "le drapeau a �t� lach�";
		else if (ev.first == ev_RESPAWN_FLAG)
			message += "le drapeau a �t� retourn�";

		// Add the message
		addMessage(message);

		return ret;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw messages
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::drawMessages(void)
	{
		auto it = _messages.begin();
		auto end = _messages.end();

		while (it != end)
		{
			inGameMessage *m = (*it);
			ogl::Text *t = m->text;

			t->resetTransform();

			// Update animation
			if (m->animated == true)
			{
				t->setScale(m->time, -m->time);
				t->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f * (2.0f - m->time * 1.25f)));
			}
			else
			{
				t->setScale(1.0f, -1.0f);
			}

			// Place the message
			t->setPosition(
				IN_GAME_MESSAGE_X - t->getSize().x * 0.5f,
				IN_GAME_MESSAGE_Y - t->getSize().y * 0.5f);

			// Draw the message
			t->draw();

			++it;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Check if we need to add messages
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::checkEvents(void)
	{
		auto ev = Event::getMainEventList();

		if (ev)
		{
			for (auto it = ev->begin(); it != ev->end(); ++it)
			{
				// Game start
				if (it->first == ev_GAME_START)
				{
					if (createGameStartMessage(*it) == false)
						break;
				}
				// Flag
				if (it->first == ev_TOUCH_FLAG || it->first == ev_DROP_FLAG ||
					it->first == ev_RESPAWN_FLAG || it->first == ev_CAPTURE_FLAG)
				{
					if (createFlagMessage(*it) == false)
						break;
				}
				// Capture zone
				if (it->first == ev_ZONE_CAPTURED)
				{
					if (createCaptureZoneMessage(*it) == false)
						break;
				}
				// Kill streak only if no important messages are displayed
				if (_blockNewMessages == false && it->first == ev_KILL)
				{
					if (createKillStreakMessage(*it) == false)
						break;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Clear OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::clearOpenGLStates()
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicInGameMessage::stop(void)
	{
		auto it = _messages.begin();
		auto end = _messages.end();

		while (it != end)
		{
			// Delete OpenGL ressources
			(*it)->text->destroy();
			delete (*it)->text;

			// Delete message
			delete (*it);
			it = _messages.erase(it);
		}
	}
}
