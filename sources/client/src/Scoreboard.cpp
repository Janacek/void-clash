/********************************************************************
**
**		Scoreboard.cpp
**		Created by : Joris VIal
**
********************************************************************/

#include "Scoreboard.hpp"
#include "Map.hpp"

// Globals
extern std::string G_ip;

// Shortcuts
#define SB_TEAM_MODE (S_Map->getMode()->getProperty()->team)

/////////////////////////////////////////////////////////////////////
/////	Scoreboard property
/////////////////////////////////////////////////////////////////////

#define SB_ENTRY_NBR 11 // For each team

#define SB_ROOT_X 0.30f
#define SB_ROOT_Y 0.15f
#define SB_ROOT_SX 0.40f
#define SB_ROOT_SY 0.75f

// Background
#define SB_ENTRY_X 0.01f
#define SB_ENTRY_SX 0.98f
#define SB_ENTRY_SY 0.03f
#define SB_ENTRY_GAP 0.0025f

// Team 1
#define SB_ENTRY_Y1 0.07f

// Team 2
#define SB_ENTRY_Y2 0.6325f

// Fields position X
#define SB_PING_X 0.010f
#define SB_NICK_X 0.155f
#define SB_KILLS_X 0.737f
#define SB_DEATHS_X 0.823f
#define SB_SCORE_X 0.911f

// Fields width
#define SB_PING_SX 0.07f
#define SB_NICK_SX 0.20f
#define SB_KILLS_SX 0.07f
#define SB_DEATHS_SX 0.07f
#define SB_SCORE_SX 0.07f

// Game info
#define SB_GAMEINFO_PX 0.735f
#define SB_GAMEINFO_PY 0.43f
#define SB_GAMEINFO_SX 0.256f
#define SB_GAMEINFO_SY 0.20f

#define SB_GAMEINFO_FIELD_PX (SB_GAMEINFO_PX + 0.015)
#define SB_GAMEINFO_FIELD_SY 0.025f
#define SB_GAMEINFO_FIELD_SX 0.225f

#define SB_TITLE_PY (SB_GAMEINFO_PY + 0.020f)
#define SB_SERVER_PY (SB_TITLE_PY + SB_GAMEINFO_FIELD_SY + 0.010f)
#define SB_MAP_PY (SB_SERVER_PY + SB_GAMEINFO_FIELD_SY)
#define SB_MODE_PY (SB_MAP_PY + SB_GAMEINFO_FIELD_SY)
#define SB_PLAYERNUMBER_PY (SB_MODE_PY + SB_GAMEINFO_FIELD_SY)
#define SB_SPECTATOR_PY (SB_PLAYERNUMBER_PY + SB_GAMEINFO_FIELD_SY)

// Team header
#define SB_TEAM_PX 0.008f
#define SB_TEAM_SY 0.099f
#define SB_TEAM_SX 0.729f
#define SB_HSCORE_PX (SB_TEAM_PX + 0.025f)
#define SB_HSCORE_SX (0.75)

// Team 1
#define SB_TEAM_PY1 0.43f
#define SB_SCORE_PY1 SB_TEAM_PY1

// Team 2
#define SB_TEAM_PY2 0.53f
#define SB_SCORE_PY2 SB_TEAM_PY2

/////////////////////////////////////////////////////////////////////
/////	Ctor/Dtor
/////////////////////////////////////////////////////////////////////

Scoreboard::Scoreboard(void)
{
	_type = GUI_SCORE_BOARD;
}

Scoreboard::~Scoreboard(void)
{
}

/////////////////////////////////////////////////////////////////////
/////	Start
/////////////////////////////////////////////////////////////////////

void Scoreboard::start(CEGUI::Window *root)
{
	_root = root;
	_refreshTime = 0.0;
	_active = true; // So it will be disabled after creation byt the GUIManager

	// Create ~SB_ENTRY_NBR entries for each team
	setupInitialState();
}

/////////////////////////////////////////////////////////////////////
/////	Update
/////////////////////////////////////////////////////////////////////

void Scoreboard::update(float deltatime)
{
	if (_active == false)
		return;

	// Update the scoreboard only every seconds
	_refreshTime += deltatime;
	if (_refreshTime > 1.0f)
	{
		updateScoreboard();
		_refreshTime = 0.0f;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Status management
/////////////////////////////////////////////////////////////////////

void Scoreboard::setActive(bool active, bool autofocus)
{
	(void)autofocus;

	// Is the state changing ?
	if (_active == active)
		return;

	// Hide entries
	setActive(active, _entries1);
	setActive(active, _entries2);

	// Update main window
	_mainWindow->setVisible(active);
	if (active == true)
	{
		_mainWindow->activate();
	}
	else
	{
		_mainWindow->deactivate();
	}

	_active = active;

	// If we display the score, refresh it now
	if (_active == true)
		updateScoreboard();
}

void Scoreboard::setActive(bool active, std::list<ScoreboardEntry*> &e)
{
	auto it = e.begin();
	auto end = e.end();

	while (it != end)
	{
		// Visibility
		(*it)->background->setVisible(active);

		// Input focus
		if (active == true)
			(*it)->background->activate();
		else
			(*it)->background->deactivate();

		++it;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Setup initial state
/////////////////////////////////////////////////////////////////////

void Scoreboard::setupInitialState(void)
{
	//////// Create main window ///////

	_mainWindow = CEGUI_WINDOW.createWindow("GlossySerpentFHD/StaticText", "ScoreboardWindow");
	if (_mainWindow == NULL)
		VC_ERROR("Scoreboard::setupInitialState, unable to create main window");

	_mainWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(SB_ROOT_X, 0.0f), CEGUI::UDim(SB_ROOT_Y, 0.0f)));
	_mainWindow->setSize(CEGUI::USize(CEGUI::UDim(SB_ROOT_SX, 0.0f), CEGUI::UDim(SB_ROOT_SY, 0.0f)));
	_mainWindow->setProperty("AutoRenderingSurface", "true");

	addButton("ScoreboardWindow", _mainWindow, false);

	// Add the window to root
	CEGUI_SYSTEM.getDefaultGUIContext().getRootWindow()->addChild(_mainWindow);

	/////// Create entries ///////

	// Team 1
	for (int i = 0; i < SB_ENTRY_NBR; ++i)
		createEntry(1, i);

	// Team 2
	for (int i = 0; i < SB_ENTRY_NBR; ++i)
		createEntry(2, i);

	//////// Headers ///////

	createLabels();
	createTeamHeaders();
	createGameInfosHeader();
}

/////////////////////////////////////////////////////////////////////
/////	Create labels
/////////////////////////////////////////////////////////////////////

void Scoreboard::createLabels(void)
{
	_labels = new ScoreboardEntry;

	// Background
	CEGUI::Window *w = createBackground(0, 0.01f, "Labels_Background");
	_labels->background = w;

	w->setSize(CEGUI::USize(CEGUI::UDim(SB_ENTRY_SX, 0.0f), CEGUI::UDim(SB_ENTRY_SY * 1.95f, 0.0f)));
	setImageButtonImages(w, "ScoreboardLabels");

	// Labels
	std::string name("Labels_");

	_labels->ping = createStaticText("GlossySerpentFHD/StaticText", name + "Ping",
		"CentreAligned", SB_PING_X, SB_ENTRY_SY - 0.0035f, SB_PING_SX, SB_ENTRY_SY, true);
	_labels->ping->setText("PING");

	_labels->nick = createStaticText("GlossySerpentFHD/StaticText", name + "Nick",
		"LeftAligned", SB_NICK_X, SB_ENTRY_SY - 0.0035f, SB_NICK_SX, SB_ENTRY_SY, true);
	_labels->nick->setText("NICKNAME");

	_labels->kills = createStaticText("GlossySerpentFHD/StaticText", name + "Kills",
		"CentreAligned", SB_KILLS_X, SB_ENTRY_SY - 0.0035f, SB_KILLS_SX, SB_ENTRY_SY, true);
	_labels->kills->setText("K");

	_labels->deaths = createStaticText("GlossySerpentFHD/StaticText", name + "Deaths",
		"CentreAligned", SB_DEATHS_X, SB_ENTRY_SY - 0.0035f, SB_DEATHS_SX, SB_ENTRY_SY, true);
	_labels->deaths->setText("D");

	_labels->score = createStaticText("GlossySerpentFHD/StaticText", name + "Score",
		"CentreAligned", SB_SCORE_X, SB_ENTRY_SY - 0.0035f, SB_SCORE_SX, SB_ENTRY_SY, true);
	_labels->score->setText("PTS");
}

/////////////////////////////////////////////////////////////////////
/////	Create game info header
/////////////////////////////////////////////////////////////////////

void Scoreboard::createGameInfosHeader(void)
{
	_gameInfos = new ScoreboardGameHeader;

	// Background
	CEGUI::Window *w = createImageButton("GlossySerpentFHD/ImageButton",
		"GameHeaderBackground", SB_GAMEINFO_PX, SB_GAMEINFO_PY, SB_GAMEINFO_SX,
		SB_GAMEINFO_SY, false);
	_gameInfos->background = w;
	setImageButtonImages(w, "ScoreboardGameInfos");

	// Title
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_Title", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_TITLE_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->title = w;
	w->setText("Infos serveur");

	// Server name
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_Server", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_SERVER_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->server = w;

	// Map name
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_Map", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_MAP_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->map = w;

	// Mode name
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_Mode", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_MODE_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->mode = w;

	// Player number
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_PlayerNumber", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_PLAYERNUMBER_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->playerNumber = w;

	// Spectator number
	w = createStaticText("GlossySerpentFHD/StaticText", "GameInfos_Spectator", "RightAligned",
		SB_GAMEINFO_FIELD_PX, SB_SPECTATOR_PY, SB_GAMEINFO_FIELD_SX, SB_GAMEINFO_FIELD_SY, false);
	_gameInfos->spectator = w;
}

/////////////////////////////////////////////////////////////////////
/////	Create team header
/////////////////////////////////////////////////////////////////////

void Scoreboard::createTeamHeaders(void)
{
	_teamHeaders = new ScoreboardTeamHeader;

	/////// Team 1 ///////

	CEGUI::Window *w = createImageButton("GlossySerpentFHD/ImageButton",
		"Team1Header_Background", SB_TEAM_PX, SB_TEAM_PY1, SB_TEAM_SX,
		SB_TEAM_SY, false);
	_teamHeaders->background1 = w;
	setImageButtonImages(w, "Team1HeaderBackground");

	// Score
	w = createStaticText("GlossySerpentFHD/StaticText", "Team1Header_Score", "LeftAligned",
		SB_HSCORE_PX, SB_SCORE_PY1, SB_HSCORE_SX, SB_TEAM_SY, false);
	_teamHeaders->score1 = w;
	w->setFont("Jura-25");

	/////// Team 2 ///////

	w = createImageButton("GlossySerpentFHD/ImageButton",
		"Team2Header_Background", SB_TEAM_PX, SB_TEAM_PY2, SB_TEAM_SX,
		SB_TEAM_SY, false);
	_teamHeaders->background2 = w;
	setImageButtonImages(w, "Team2HeaderBackground");

	// Score
	w = createStaticText("GlossySerpentFHD/StaticText", "Team2Header_Score", "LeftAligned",
		SB_HSCORE_PX, SB_SCORE_PY2, SB_HSCORE_SX, SB_TEAM_SY, false);
	_teamHeaders->score2 = w;
	w->setFont("Jura-25");
}

/////////////////////////////////////////////////////////////////////
/////	Setup a line
/////////////////////////////////////////////////////////////////////

ScoreboardEntry* Scoreboard::createEntry(int team, int index)
{
	// Name of the entry
	std::string name =
		std::to_string(index) +
		std::string("_") +
		std::to_string(team);

	// Create the entry
	ScoreboardEntry *entry = new ScoreboardEntry;
	std::memset(entry, 0, sizeof(ScoreboardEntry));

	float offY = (team == 1) ? (SB_ENTRY_Y1) : (SB_ENTRY_Y2);

	// Add fields
	entry->ping = createField(index, SB_PING_X, offY, SB_PING_SX, "Ping_" + name);
	entry->nick = createField(index, SB_NICK_X, offY, SB_NICK_SX, "Nick_" + name);
	entry->kills = createField(index, SB_KILLS_X, offY, SB_KILLS_SX, "Kills_" + name);
	entry->deaths = createField(index, SB_DEATHS_X, offY, SB_DEATHS_SX, "Deaths_" + name);
	entry->score = createField(index, SB_SCORE_X, offY, SB_SCORE_SX, "Score_" + name);

	// Background
	entry->background = createBackground(index, offY, ("Background_" + name));

	// Push the entry into the lists
	(team == 1) ? (_entries1.push_back(entry)) : (_entries2.push_back(entry));

	// Save entry into the FFA list
	_entriesFFA.push_back(entry);

	return entry;
}

/////////////////////////////////////////////////////////////////////
/////	Setup the background of the line
/////////////////////////////////////////////////////////////////////

CEGUI::Window* Scoreboard::createBackground(int index, float offY, const std::string &name)
{
	CEGUI::Window *w = CEGUI_WINDOW.createWindow("GlossySerpentFHD/ImageButton", name);
	if (w == NULL) failToCreateNewEntry();

	// Set position and size
	float offsetX = SB_ENTRY_X;
	float offsetY = offY + (SB_ENTRY_GAP + SB_ENTRY_SY) * index;
	float sizeX = SB_ENTRY_SX;
	float sizeY = SB_ENTRY_SY;

	w->setPosition(CEGUI::UVector2(CEGUI::UDim(offsetX, 0.0f), CEGUI::UDim(offsetY, 0.0f)));
	w->setSize(CEGUI::USize(CEGUI::UDim(sizeX, 0.0f), CEGUI::UDim(sizeY, 0.0f)));

	// Bind callbacks
	addButton(name, w, true);

	// Add to the view
	_mainWindow->addChild(w);

	return w;
}

/////////////////////////////////////////////////////////////////////
/////	Add a field to the line
/////////////////////////////////////////////////////////////////////

CEGUI::Window* Scoreboard::createField(int index, float offX, float offY, float sx,
	const std::string &name)
{
	// Check alignement to use
	std::string alignment = "RightAligned";
	if (name.find("Nick_") != std::string::npos)
		alignment = "WordWrapLeftAligned";

	// Set position and size
	float offsetX = offX;
	float offsetY = offY + (SB_ENTRY_GAP + SB_ENTRY_SY) * index;
	float sizeX = sx;
	float sizeY = SB_ENTRY_SY;

	CEGUI::Window *w = createStaticText("GlossySerpentFHD/StaticText", name, alignment,
		offsetX, offsetY, sizeX, sizeY, true);

	return w;
}

/////////////////////////////////////////////////////////////////////
/////	Sort entries
/////////////////////////////////////////////////////////////////////

bool scoreSort(const ScoreboardEntryBuffer &e1, const ScoreboardEntryBuffer &e2)
{
	return (e1.score > e2.score);
}

/////////////////////////////////////////////////////////////////////
/////	Update scoreboard
/////////////////////////////////////////////////////////////////////

void Scoreboard::updateScoreboard(void)
{
	// Buffers
	std::list<ScoreboardEntryBuffer> team1;
	std::list<ScoreboardEntryBuffer> team2;

	int playerTeam1 = refreshPlayersScore(team1, team2);

	// Sort by score (Solo and Team)
	team1.sort(scoreSort);
	refreshGUIEntries(1, team1, std::max(0, SB_ENTRY_NBR - playerTeam1));

	// Sort by score (Team)
	if (SB_TEAM_MODE == true)
	{
		team2.sort(scoreSort);
		refreshGUIEntries(2, team2);
	}

	// Refresh game infos
	refreshTeamHeaders(1);
	refreshTeamHeaders(2);
	refreshGameInfos();
}

/////////////////////////////////////////////////////////////////////
/////	Refrehs team infos
/////////////////////////////////////////////////////////////////////

void Scoreboard::refreshTeamHeaders(int team)
{
	bool teamMode = S_Map->getMode()->getProperty()->team;

	// Images
	CEGUI::Window *w = (team == 1) ? (_teamHeaders->background1) : (_teamHeaders->background2);

	if (teamMode == false)
		setImageButtonImages(w, "FFAHeaderBackground");
	else if (team == 1)
		setImageButtonImages(w, "Team1HeaderBackground");
	else if (team == 2)
		setImageButtonImages(w, "Team2HeaderBackground");

	// Score
	int score = (team == 1) ? (S_Map->getScore().first) : (S_Map->getScore().second);
	w = (team == 1) ? (_teamHeaders->score1) : (_teamHeaders->score2);

	std::string str("");
	Player *leader = S_Map->getLeadingPlayer().get();

	// Team - Display score
	if (teamMode == true)
		str = std::to_string(score);

	// FFA - Top header
	else if (team == 1 && leader != NULL)
	{
		str = "Meilleur joueur";
	}
	// FFA - Bottom header
	else if (team == 2 && leader != NULL)
	{
		str = S_Map->getLeadingPlayer()->getNick();
	}
	// Tie
	else if (team == 1)
	{
		str = "Aucun meilleur joueur";
	}
	// Tie
	else if (team == 2)
	{
		str = "Egalit� !";
	}

	w->setText(str);
}

/////////////////////////////////////////////////////////////////////
/////	Refrehs game infos
/////////////////////////////////////////////////////////////////////

void Scoreboard::refreshGameInfos(void)
{
	std::string format("[font='Jura-7']");

	// Server
	_gameInfos->server->setText(format + "Serveur: " + G_ip);

	// Map
	std::string mapName(S_Map->getMapDatabase()->getCurrentMapName());
	_gameInfos->map->setText(format + "Carte: " + mapName);

	// Mode
	_gameInfos->mode->setText(format + "Mode: " + S_Map->getMode()->getModeName());

	// Player number
	_gameInfos->playerNumber->setText(format + "Joueurs : " +
		std::to_string(S_Map->getRealPlayerInTeam(-1)));

	// Spectator
	_gameInfos->spectator->setText(format + "Spectateurs : " +
		std::to_string(S_Map->getRealPlayerInTeam(0)));
}

/////////////////////////////////////////////////////////////////////
/////	Prepare GUI refresh
/////////////////////////////////////////////////////////////////////

int Scoreboard::refreshPlayersScore(std::list<ScoreboardEntryBuffer> &t1,
	std::list<ScoreboardEntryBuffer> &t2)
{
	// Go through every players
	auto it = S_Map->getPlayers()->begin();
	auto end = S_Map->getPlayers()->end();

	int playerTeam1 = 0;
	while (it != end)
	{
		// Create buffer entry
		ScoreboardEntryBuffer e;

		Player *p = (*it).get();

		// Ignore spectator
		if (p != NULL && p->getTeam() != 0)
		{
			// Fill data
			e.nick = p->getNick();
			e.alive = !p->isRespawning();
			e.isCurrentPlayer = (S_Map->getCurrentPlayer().get() == p);
			e.team = p->getTeam();
			e.kills = p->getKills();
			e.deaths = p->getDeaths();
			e.score = p->getScore();
			e.ping = p->getLatency() * 0.001f;

			// Sort by team
			if (SB_TEAM_MODE == true)
			{
				if (e.team == 1)
				{
					t1.push_back(e);
					++playerTeam1;
				}
				else
				{
					t2.push_back(e);
				}
			}
			// Solo modes
			else
			{
				t1.push_back(e);
				++playerTeam1;
			}
		}
		++it;
	}
	return playerTeam1;
}

/////////////////////////////////////////////////////////////////////
/////	Update GUI
/////////////////////////////////////////////////////////////////////

void Scoreboard::refreshGUIEntries(int team, std::list<ScoreboardEntryBuffer> &t, int startIndex)
{
	// Get GUI button list to update
	std::list<ScoreboardEntry*> &entries = (team == 1) ? (_entries1) : (_entries2);
	if (SB_TEAM_MODE == false)
		entries = _entriesFFA;

	// Get entry slots
	auto eIt = entries.begin();
	auto eEnd = entries.end();

	// Go throught top players
	auto pIt = t.begin();
	auto pEnd = t.end();

	// Bottom align for team 1 if all the slots are not filled
	int indexAdvance = 0;
	while (team == 1 && pIt != pEnd && indexAdvance < startIndex)
	{
		disableEntry(*eIt);
		++eIt;
		++indexAdvance;
	}

	// Update GUI
	int index = indexAdvance;
	int indexMax = (SB_TEAM_MODE == true) ? (SB_ENTRY_NBR) : (SB_ENTRY_NBR * 2);
	while (eIt != eEnd && pIt != pEnd && index < indexMax)
	{
		ScoreboardEntry *e = (*eIt);
		ScoreboardEntryBuffer &p = (*pIt);

		// Set fields text
		setFieldText(e, p);

		// Enable the highlight
		e->background->activate();

		// Set images
		std::string normalTexture("Green");
		std::string hoverTexture("Green");

		// Red team
		if (p.team == 2)
		{
			normalTexture = "Red";
			hoverTexture = "Red";
		}

		// Solo modes
		if (SB_TEAM_MODE == false)
		{
			normalTexture = "FFA";
			hoverTexture = "FFA";
		}

		// Current player
		if (p.isCurrentPlayer == true)
		{
			normalTexture = "CurrentPlayer";
			hoverTexture = "CurrentPlayer";
		}

		// Player is dead
		if (p.alive == false)
			normalTexture = "Dead";

		// Update images
		setImageButtonImages((*eIt)->background,
			"ScoreboardEntryNormal" + normalTexture,
			"ScoreboardEntryHover" + hoverTexture,
			"ScoreboardEntryHover" + hoverTexture);

		++index;
		++pIt;
		++eIt;
	}

	// Hide all other
	while (eIt != eEnd)
	{
		disableEntry(*eIt);
		++eIt;
	}
}

/////////////////////////////////////////////////////////////////////
/////	Callbacks
/////////////////////////////////////////////////////////////////////

bool Scoreboard::onClick(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

bool Scoreboard::onEnterArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

bool Scoreboard::onLeaveArea(const CEGUI::EventArgs &eIN)
{
	(void)eIN;
	return false;
}

/////////////////////////////////////////////////////////////////////
/////	Create a button and bind callbacks
/////////////////////////////////////////////////////////////////////

void Scoreboard::addButton(const std::string &name, CEGUI::Window* w, bool hasHover)
{
	// Mouse hovered
	if (hasHover == true)
	{
		w->subscribeEvent(
			CEGUI::Window::EventMouseEntersArea,
			CEGUI::Event::Subscriber(&Scoreboard::onEnterArea, this));

		w->subscribeEvent(
			CEGUI::Window::EventMouseLeavesArea,
			CEGUI::Event::Subscriber(&Scoreboard::onLeaveArea, this));
	}

	// Mouse click
	w->subscribeEvent(
		CEGUI::PushButton::EventMouseClick,
		CEGUI::Event::Subscriber(&Scoreboard::onClick, this));

	// Manually add the window to the GUIEntity container
	_windows[name] = w;
}

/////////////////////////////////////////////////////////////////////
/////	Utils
/////////////////////////////////////////////////////////////////////

void Scoreboard::failToCreateNewEntry(void)
{
	VC_ERROR("Scoreboard::createEntry, unable to create new entry");
}

/////////////////////////////////////////////////////////////////////
/////	Create a static text
/////////////////////////////////////////////////////////////////////

CEGUI::Window *Scoreboard::createStaticText(const std::string &scheme, const std::string &name,
	const std::string &alignement, float posX, float posY, float sX, float sY, bool hasOver)
{
	CEGUI::Window *w = CEGUI_WINDOW.createWindow(scheme, name);
	if (w == NULL) failToCreateNewEntry();

	// Position and size
	w->setPosition(CEGUI::UVector2(CEGUI::UDim(posX, 0.0f), CEGUI::UDim(posY, 0.0f)));
	w->setSize(CEGUI::USize(CEGUI::UDim(sX, 0.0f), CEGUI::UDim(sY, 0.0f)));

	// Disable ugly border
	w->setProperty("FrameEnabled", "False");
	w->setProperty("BackgroundEnabled", "False");

	// Alignement
	w->setProperty("HorzFormatting", alignement);

	// Bind callbacks
	addButton(name, w, hasOver);

	// Add to the view
	_mainWindow->addChild(w);

	return w;
}

/////////////////////////////////////////////////////////////////////
/////	Create an ImageButton
/////////////////////////////////////////////////////////////////////

CEGUI::Window *Scoreboard::createImageButton(const std::string &scheme,
	const std::string &name, float posX, float posY, float sX, float sY, bool hasOver)
{
	CEGUI::Window *w = CEGUI_WINDOW.createWindow(scheme, name);
	if (w == NULL) failToCreateNewEntry();

	// Position and size
	w->setPosition(CEGUI::UVector2(CEGUI::UDim(posX, 0.0f), CEGUI::UDim(posY, 0.0f)));
	w->setSize(CEGUI::USize(CEGUI::UDim(sX, 0.0f), CEGUI::UDim(sY, 0.0f)));

	// Bind callbacks
	addButton(name, w, hasOver);

	// Add to the view
	_mainWindow->addChild(w);

	return w;
}

/////////////////////////////////////////////////////////////////////
/////	Empty fields
/////////////////////////////////////////////////////////////////////

void Scoreboard::resetFields(ScoreboardEntry *e)
{
	e->ping->setText("");
	e->nick->setText("");
	e->kills->setText("");
	e->deaths->setText("");
	e->score->setText("");
}

/////////////////////////////////////////////////////////////////////
/////	Set ImageButton images
/////////////////////////////////////////////////////////////////////

void Scoreboard::setImageButtonImages(CEGUI::Window *w, const std::string &normalImage,
	const std::string &hoverImage, const std::string &pushedImage)
{
	std::string path("ClientIcons/");
	std::string hover(hoverImage);
	std::string pushed(pushedImage);

	if (hover.empty() == true)
		hover = normalImage;

	if (pushed.empty() == true)
		pushed = normalImage;

	w->setProperty("NormalImage", path + normalImage);
	w->setProperty("HoverImage", path + hover);
	w->setProperty("PushedImage", path + pushed);
}

/////////////////////////////////////////////////////////////////////
/////	Disable an entry
/////////////////////////////////////////////////////////////////////

void Scoreboard::disableEntry(ScoreboardEntry *e)
{
	// Set fields text
	resetFields(e);

	// Set images
	setImageButtonImages(e->background, "ScoreboardEntryNormalBlack");

	// Disable inputs
	e->background->deactivate();
}

/////////////////////////////////////////////////////////////////////
/////	Fill a line
/////////////////////////////////////////////////////////////////////

void Scoreboard::setFieldText(ScoreboardEntry *e, const ScoreboardEntryBuffer &p)
{
	e->ping->setText(std::to_string(p.ping));
	e->nick->setText(p.nick);
	e->kills->setText(std::to_string(p.kills));
	e->deaths->setText(std::to_string(p.deaths));
	e->score->setText(std::to_string(p.score));
}

/////////////////////////////////////////////////////////////////////
/////	Stop
/////////////////////////////////////////////////////////////////////

void Scoreboard::stop(void)
{
	// Delete entries 1
	{
		auto it = _entries1.begin();
		auto end = _entries1.end();
		while (it != end)
		{
			// Remove the window from CEGUI::Root
			CEGUI_SYSTEM.getDefaultGUIContext().getRootWindow()->removeChild((*it)->background);
			// Delete the entry
			delete (*it);
			++it;
		}
		_entries1.clear();
	}

	// Delete entries 2
	{
		auto it = _entries2.begin();
		auto end = _entries2.end();
		while (it != end)
		{
			// Remove the window from CEGUI::Root
			CEGUI_SYSTEM.getDefaultGUIContext().getRootWindow()->removeChild((*it)->background);
			// Delete the entry
			delete (*it);
			++it;
		}
		_entries2.clear();
	}

	GUIEntity::stop();
}
