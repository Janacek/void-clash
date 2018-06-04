/********************************************************************
**
**              GraphicWeaponSelection.cpp
**              Created by : Vial Joris
**
********************************************************************/

#include "GraphicWeaponSelection.hpp"
#include "GraphicHelper.hpp"
#include "GraphicConfiguration.hpp"
#include "Map.hpp"
#include "OpenGLContext.hpp"

namespace VoidClashGraphics
{
	// Weapon selection property (screen)

	#define WEAPON_SELECTION_SIZE 200.0f
	#define WEAPON_POSY (GCONFIG.winY - WEAPON_SELECTION_SIZE * 0.5f)
	#define WEAPON_POSX_1 (GCONFIG.winX - WEAPON_SELECTION_SIZE * 1.55f)
	#define WEAPON_POSX_2 (GCONFIG.winX - WEAPON_SELECTION_SIZE * 0.5f)

	// Weapon selection property (local)

	#define WEAPON_LOCAL_POSY 0.0f
	#define WEAPON_LOCAL_SIZE 0.5f
	#define COOLDOWN_LOCAL_SIZE (WEAPON_LOCAL_SIZE * 3.0f)
	#define WEAPON_LOCAL_LABEL_POSY (WEAPON_SELECTION_SIZE * 0.05f)
	#define WEAPON_LOCAL_LABEL_POSX (WEAPON_SELECTION_SIZE * 0.5f)
	#define WEAPON_LABEL_SIZE (0.5f * WEAPON_SELECTION_SIZE / 250.0f)

	/////////////////////////////////////////////////////////////////////
	/////	Constructor/ Destructor
	/////////////////////////////////////////////////////////////////////

	GraphicWeaponSelection::GraphicWeaponSelection(OpenGLContext *context = NULL) :
		GraphicHudEntity(context)
	{

	}

	GraphicWeaponSelection::~GraphicWeaponSelection(void)
	{

	}

	/////////////////////////////////////////////////////////////////////
	/////	Start
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::start(ogl::Camera *camera)
	{
		_camera = camera;

		/////// Create OpenGL ressources ///////

		// Final rendering shader

		_final = S_Shader->addShader("MapQuadWorld");

		// Final rendering quad

		_quad = new ogl::Quad();
		_quad->create();

		// Local camera
		_localCamera = new ogl::Camera();
		_localCamera->setupOrthographic(WEAPON_SELECTION_SIZE, WEAPON_SELECTION_SIZE, false);
		_localCamera->update();

		// Weapon icons

		S_Texture->addTexture("WeaponBullet");
		S_Texture->addTexture("WeaponBomb");
		S_Texture->addTexture("WeaponShield");
		S_Texture->addTexture("WeaponGravity");
		S_Texture->addTexture("WeaponTurret");
		S_Texture->addTexture("WeaponEmpty");

		_weaponShader = S_Shader->addShader("MapQuadColor");

		// Cooldown

		S_Texture->addTexture("Cooldown");
		_cooldownShader = S_Shader->addShader("WeaponCooldown");

		_cooldown = new ogl::Quad();
		_cooldown->create();

		// Weapons name

		std::string font("ExoBold.otf");

		_label1 = new ogl::Text();
		_label1->create(S_Font->getFont(font), "", _localCamera);

		_label2 = new ogl::Text();
		_label2->create(S_Font->getFont(font), "", _localCamera);

		// Renderer

		_render1 = new ogl::Framebuffer();
		_render1->create(WEAPON_SELECTION_SIZE, WEAPON_SELECTION_SIZE, false);

		_render2 = new ogl::Framebuffer();
		_render2->create(WEAPON_SELECTION_SIZE, WEAPON_SELECTION_SIZE, false);

		// Key picture

		S_Shader->addShader("MapQuadWorld");
		S_Texture->addTexture("KeyWeapon");

		_key = new ogl::Quad();
		_key->create();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Update
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::update(float deltatime)
	{
		(void)deltatime;

		if (S_Map->getPlayerFollowed() == NULL)
			return;

		glClearColor(0, 0, 0, 0);

		/////// Weapon 1 ////////

		_render1->bind();
		glViewport(0, 0, WEAPON_SELECTION_SIZE, WEAPON_SELECTION_SIZE);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		drawIcon(true);
		drawCooldown(true);
		drawLabel(true);

		_render1->unbind();

		/////// Weapon 2 ////////

		_render2->bind();
		glViewport(0, 0, WEAPON_SELECTION_SIZE, WEAPON_SELECTION_SIZE);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		drawIcon(false);
		drawCooldown(false);
		drawLabel(false);

		_render2->unbind();

		/////// Final rendering ////////

		// Bind previously saved framebuffer (GraphicHud::update)
		_context->restorePreviousFramebuffer();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawOnScreen(true);
		drawOnScreen(false);

		/////// Display key ///////
		drawKey();

		clearOpenGLStates();

		GL_CHECK;
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw key
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::drawKey(void)
	{
		// Get texture
		S_Texture->bindTexture("KeyWeapon");

		// Setup quad
		_key->resetTransform();
		_key->scale(50.0f, -50.0f);
		_key->translate(GCONFIG.winX - WEAPON_SELECTION_SIZE * 1.02,
			GCONFIG.winY - 75.0f);

		// Update shader
		ogl::Shader *s = S_Shader->bindShader("MapQuadWorld");

		s->setParameter("Camera", _camera->getFinal());
		s->setParameter("Model", _key->getTransform());

		// Draw
		_key->draw();

		// Clean
		S_Shader->unbindShader();
		S_Texture->unbindTexture();

	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw weapon icon
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::drawIcon(bool primary)
	{
		// Get weapon
		t_weapon *w = NULL;

		if (S_Map->getPlayerFollowed()->getWeaponsManager())
			w = S_Map->getPlayerFollowed()->getWeaponsManager()->getWeaponSelected(primary);

		// Get texture
		(w == NULL) ? bindTexture("EMPTY") : bindTexture(w->category);

		// Setup quad
		_quad->resetTransform();
		_quad->scale(WEAPON_LOCAL_SIZE, WEAPON_LOCAL_SIZE);
		_quad->translate(0.0f, WEAPON_LOCAL_POSY);

		// Update shader
		_weaponShader->bind();
		_weaponShader->setParameter("Model", _quad->getTransform());
		_weaponShader->setParameter("Color", glm::vec4(1, 1, 1, 1));

		// Draw
		_quad->draw();

		// Clean
		_weaponShader->unbind();
		S_Texture->unbindTexture();
	}

	void GraphicWeaponSelection::bindTexture(const std::string &weapon)
	{
		if (weapon == "BULLET") S_Texture->bindTexture("WeaponBullet");
		else if (weapon == "BOMB") S_Texture->bindTexture("WeaponBomb");
		else if (weapon == "GRAVITY") S_Texture->bindTexture("WeaponGravity");
		else if (weapon == "TURRET") S_Texture->bindTexture("WeaponTurret");
		else if (weapon == "SHIELD") S_Texture->bindTexture("WeaponShield");
		// No weapon
		else if (weapon == "EMPTY") S_Texture->bindTexture("WeaponEmpty");
		else S_Texture->bindTexture("WeaponEmpty");
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw weapon cooldown
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::drawCooldown(bool primary)
	{
		// Get weapon
		WeaponManager *w = S_Map->getPlayerFollowed()->getWeaponsManager();
		if (w == NULL)
			return;

		// Get texture
		S_Texture->bindTexture("Cooldown");

		// Setup quad
		_quad->resetTransform();
		_quad->scale(COOLDOWN_LOCAL_SIZE, -COOLDOWN_LOCAL_SIZE);
		_quad->translate(0.0f, WEAPON_LOCAL_POSY);

		// Get current cooldown status
		float status = 0.0f;
		if (primary == true)
			status = w->getCurrentCooldownPrimary();
		else
			status = w->getCurrentCooldownSeconday();

		// Update shader
		_cooldownShader->bind();
		_cooldownShader->setParameter("Cooldown", status);
		_cooldownShader->setParameter("Model", _quad->getTransform());

		// Not enought energy, fade it out
		if (w->getWeapons(primary, false) &&
			S_Map->getPlayerFollowed()->getEnergy() < w->getWeapons(primary, false)->energy_cost)
		{
			_cooldownShader->setParameter("Color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else
		{
			_cooldownShader->setParameter("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// Draw
		_quad->draw();

		// Clean
		_cooldownShader->unbind();
		S_Texture->unbindTexture();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Draw weapon label
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::drawLabel(bool primary)
	{
		// Get label to draw
		ogl::Text *label = (primary == false) ? (_label1) : (_label2);

		// Get weapon
		t_weapon *w = NULL;
		if (S_Map->getPlayerFollowed() && S_Map->getPlayerFollowed()->getWeaponsManager())
			w = S_Map->getPlayerFollowed()->getWeaponsManager()->getWeaponSelected(primary);

		// Set name
		std::string empty = (primary == true) ? ("Arme 1 : Touche B") : ("Arme 2 : Touche B");
		label->setString((w == NULL) ? (empty) : (w->name));

		// Set property
		label->resetTransform();
		label->setScale(WEAPON_LABEL_SIZE, WEAPON_LABEL_SIZE);
		label->setPosition(
			WEAPON_LOCAL_LABEL_POSX - label->getSize().x * 0.5f,
			WEAPON_LOCAL_LABEL_POSY);

		// Draw
		label->draw();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Final rendering
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::drawOnScreen(bool primary)
	{
		glViewport(0, 0, GCONFIG.winX, GCONFIG.winY);

		float posx = (primary == true) ? (WEAPON_POSX_1) : (WEAPON_POSX_2);
		float posy = WEAPON_POSY;

		// Setup quad
		_quad->resetTransform();
		_quad->scale(WEAPON_SELECTION_SIZE, -WEAPON_SELECTION_SIZE);
		_quad->translate(posx, posy);

		// Update shader
		_final->setParameter("Model", _quad->getTransform());
		_final->setParameter("Camera", _camera->getFinal());

		// Bind texture
		(primary == true) ? (_render1->bindTexture()) : (_render2->bindTexture());

		// Draw
		_quad->draw();

		// Clean
		S_Texture->unbindTexture();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Reset OpenGL states
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::clearOpenGLStates(void)
	{
		glClearColor(0, 0, 0, 1);
		S_Texture->unbindTexture();
		S_Shader->unbindShader();
	}

	/////////////////////////////////////////////////////////////////////
	/////	Stop
	/////////////////////////////////////////////////////////////////////

	void GraphicWeaponSelection::stop(void)
	{
		_quad->destroy();
		safeDelete(_quad);

		_cooldown->destroy();
		safeDelete(_cooldown);

		_label1->destroy();
		_label2->destroy();
		safeDelete(_label1);
		safeDelete(_label2);

		_render1->destroy();
		_render2->destroy();
		safeDelete(_render1);
		safeDelete(_render2);

		_key->destroy();
		safeDelete(_key);
	}
}
