/********************************************************************
**
**              GraphicKillReport.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicKillReport.hpp"
#include "Map.hpp"
#include "GraphicHelper.hpp"
#include "GraphicConfiguration.hpp"

namespace VoidClashGraphics
{
	// Kill report propery
	#define KILL_REPORT_X (GCONFIG.winX) // Kills are displayed from right to left
	#define KILL_REPORT_Y 0.0f // Kills are display from top to bottom
	#define KILL_ENTRY_GAP 40.0f
	#define KILL_WEAPON_SIZE 30.0f
	#define KILL_ENTRY_SCALE 0.4f
	#define KILL_ENTRY_GAP_X 4.0f

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicKillReport::GraphicKillReport(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicKillReport::~GraphicKillReport(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::start(ogl::Camera *camera)
	{
		_camera = camera;

		// Create shader
		_shader = S_Shader->addShader("MapQuadWorldColor");

		// Create textures
		S_Texture->addTexture("WeaponBullet");
		S_Texture->addTexture("WeaponBomb");
		S_Texture->addTexture("WeaponGravity");
		S_Texture->addTexture("WeaponTurret");
		S_Texture->addTexture("WeaponShield");
		S_Texture->addTexture("Crane");

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::update(float deltatime)
	{
		// Check for new kills
		getNewKillEntry();

		// Update kills
		updateEntries(deltatime);

		// Display kills
		drawEntries();

		// Reset OpenGL
		clearOpenGLStates();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Create a new kill entry
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::getNewKillEntry(void)
	{
		std::list<t_killHandler *> &kills(HudRessources::getInstance()->getKillsReport());

		auto it = kills.begin();
		auto end = kills.end();

		while (it != end)
		{
			// Check if the entry is new
			if ((*it)->_isNew == true)
			{
				// Create a new entry
				killEntry *entry = createNewEntry(*it);

				// Push the new entry into container
				_kills.push_back(entry);

				// Mark the entry as created
				(*it)->_isNew = false;
			}
			++it;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Create a new entry
	/////////////////////////////////////////////////////////////////////

	killEntry* GraphicKillReport::createNewEntry(t_killHandler *kill)
	{
		killEntry *entry = new killEntry();

		// Get the player reference
		Player *killer = dynamic_cast<Player*>
			(S_Map->findObjectWithID(kill->_killer->getId()).get());
		Player *killed = dynamic_cast<Player*>
			(S_Map->findObjectWithID(kill->_killed->getId()).get());
		entry->pKiller = killer;
		entry->pKilled = killed;

		// Get players nickname
		std::string killerNick =
			(killer == NULL) ? ("Unknown") : (killer->getNick());
		std::string killedNick =
			(killed == NULL) ? ("Unknown") : (killed->getNick());

		// Create ogl::Text
		std::string font("ExoRegular.otf");

		entry->killer = new ogl::Text();
		entry->killer->create(S_Font->getFont(font), killerNick);
		entry->killed = new ogl::Text();
		entry->killed->create(S_Font->getFont(font), killedNick);

		// Create ogl::Quad
		entry->quad = new ogl::Quad();
		entry->quad->create();

		// Get weapon used
		if (killer == killed)
			entry->weapon = NONE;
		else
			entry->weapon = kill->_hitterType;

		// Time displayed on screen
		entry->time = 5.0f; // Seconds

		return entry;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update all entries
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::updateEntries(float deltatime)
	{
		auto it = _kills.begin();
		auto end = _kills.end();

		while (it != end)
		{
			(*it)->time -= deltatime;

			// Remove old entry
			if ((*it)->time < 0)
			{
				// Delete OpenGL ressources
				(*it)->killer->destroy();
				(*it)->killed->destroy();
				(*it)->quad->destroy();

				delete (*it)->killer;
				delete (*it)->killed;
				delete (*it)->quad;

				it = _kills.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Display kills
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::drawEntries(void)
	{
		unsigned int id = 0;
		auto it = _kills.begin();
		auto end = _kills.end();

		while (it != end)
		{
			const killEntry *e = (*it);
			float offsetX = KILL_ENTRY_GAP_X;

			/////// Draw killed player ///////

			e->killed->setFont(getEntryFont(e->pKilled));
			e->killed->setColor(getEntryColor(e->pKilled, e->time));
			e->killed->resetTransform();
			e->killed->setScale(KILL_ENTRY_SCALE, -KILL_ENTRY_SCALE);
			e->killed->setPosition(
				KILL_REPORT_X - e->killed->getSize().x - offsetX,
				KILL_REPORT_Y + KILL_ENTRY_GAP * id + e->killed->getSize().y +
				KILL_WEAPON_SIZE * 0.25f);
			e->killed->draw();

			offsetX += e->killed->getSize().x + KILL_ENTRY_GAP_X;

			/////// Draw weapon icon ///////

			// Set quad propery
			e->quad->bind();
			e->quad->resetTransform();
			e->quad->scale(KILL_WEAPON_SIZE, -KILL_WEAPON_SIZE);
			e->quad->translate(
				KILL_REPORT_X - KILL_WEAPON_SIZE * 0.5f - offsetX,
				KILL_REPORT_Y + KILL_ENTRY_GAP * id + e->killed->getSize().y);

			// Update shader
			_shader->bind();
			_shader->setParameter("Model", e->quad->getTransform());
			_shader->setParameter("Camera", _camera->getFinal());
			_shader->setParameter("Color", getEntryColor(NULL, e->time));

			// Bind texture
			bindTexture(e->weapon);

			// Draw
			e->quad->draw();
			S_Texture->unbindTexture();
			_shader->unbind();

			offsetX += KILL_WEAPON_SIZE + KILL_ENTRY_GAP_X;

			/////// Draw killer player ///////

			e->killer->setFont(getEntryFont(e->pKiller));
			e->killer->setColor(getEntryColor(e->pKiller, e->time));
			e->killer->resetTransform();
			e->killer->setScale(KILL_ENTRY_SCALE, -KILL_ENTRY_SCALE);
			e->killer->setPosition(
				KILL_REPORT_X - e->killer->getSize().x - offsetX,
				KILL_REPORT_Y + KILL_ENTRY_GAP * id + e->killer->getSize().y +
				KILL_WEAPON_SIZE * 0.25f);
			e->killer->draw();

			++it;
			++id;
		}
	}

	ogl::Font* GraphicKillReport::getEntryFont(Player *player)
	{
		if (S_Map->getCurrentPlayer().get() == player)
			return S_Font->getFont("ExoBlack.otf");

		return S_Font->getFont("ExoRegular.otf");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Get kill entry color
	/////	Return (1, 1, 1, alpha) is player is NULL
	/////////////////////////////////////////////////////////////////////

	glm::vec4 GraphicKillReport::getEntryColor(Player *player, float currentTime)
	{
		glm::vec4 color(1, 1, 1, 1);
		float alpha = std::min(1.0f, std::pow(currentTime + 0.9f, 10.0f) / 5.0f);

		// Weapon icon
		if (player == NULL)
			return glm::vec4(1, 1, 1, alpha);

		color.a = alpha;

		/////// No team based ///////

		if (S_Map->getMode()->getProperty()->team == false &&
			player != S_Map->getCurrentPlayer().get())
		{
			color.r = 0.75;
			color.g = 0.75;
			color.b = 0.75;
			return color;
		}

		/////// Team based ///////

		// Main player
		if (player == S_Map->getCurrentPlayer().get())
			color = glm::vec4(1, 1, 0, alpha);
		// Team 1
		else if (S_Map->getMode()->getProperty()->team && true && player->getTeam() == 1)
			color = glm::vec4(0, 1, 0, alpha);
		// Team 2
		else if (S_Map->getMode()->getProperty()->team && true && player->getTeam() == 2)
			color = glm::vec4(1, 0, 0, alpha);

		// Fade to white

		color.r += 0.5f;
		color.g += 0.5f;
		color.b += 0.5f;

		color.r *= 0.75f;
		color.g *= 0.75f;
		color.b *= 0.75f;

		return color;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL States
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::bindTexture(eObjectType weapon)
	{
		switch (weapon)
		{
		case BULLET: S_Texture->bindTexture("WeaponBullet"); break;
		case BOMB: S_Texture->bindTexture("WeaponBomb"); break;
		case GRAVITY_FIELD: S_Texture->bindTexture("WeaponGravity"); break;
		case TURRET: S_Texture->bindTexture("WeaponTurret"); break;
		case SHIELD: S_Texture->bindTexture("WeaponShield"); break;

		// Suicide
		case NONE: S_Texture->bindTexture("Crane"); break;
		default: S_Texture->bindTexture("Crane"); break;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL States
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::clearOpenGLStates(void)
	{
		_shader->unbind();
		S_Texture->unbindTexture();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicKillReport::stop(void)
	{
		auto it = _kills.begin();
		auto end = _kills.end();

		while (it != end)
		{
			// Delete OpenGL ressources
			(*it)->killer->destroy();
			(*it)->killed->destroy();
			(*it)->quad->destroy();

			delete (*it)->killer;
			delete (*it)->killed;
			delete (*it)->quad;

			it = _kills.erase(it);
		}
	}
}
