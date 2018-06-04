/********************************************************************
**
**		Scoreboard.hpp
**		Created by : Joris VIal
**
********************************************************************/

#ifndef SCORE_BOARD
#define SCORE_BOARD

#include "GUIEntity.hpp"

// Describe a score board entry (line)
struct ScoreboardEntry
{
	// Background
	CEGUI::Window *background;

	// Datas
	CEGUI::Window *ping;
	CEGUI::Window *nick;
	CEGUI::Window *kills;
	CEGUI::Window *deaths;
	CEGUI::Window *score;
};

// Stucture used for scoreboard refresh
struct ScoreboardEntryBuffer
{
	bool alive;
	bool isCurrentPlayer;
	int team;
	int kills;
	int deaths;
	int score;
	int ping;
	std::string nick;
};

// Describe the team score part
// Only displayed in team modes
struct ScoreboardTeamHeader
{
	// Background
	CEGUI::Window *background1; // Team1
	CEGUI::Window *background2; // Team2

	// Score
	CEGUI::Window *score1; // Team1
	CEGUI::Window *score2; // Team2

	// Only ffa
	CEGUI::Window* leader;
	CEGUI::Window* currentPlayerRank;
};

// Describe the game info part
struct ScoreboardGameHeader
{
	// Background
	CEGUI::Window *background;

	// Title
	CEGUI::Window *title;

	// Server name
	CEGUI::Window *server;

	// Current map
	CEGUI::Window *map;

	// Current mode
	CEGUI::Window *mode;

	// Player number
	CEGUI::Window *playerNumber;

	// Specator number
	CEGUI::Window *spectator;
};

class Scoreboard : public GUIEntity
{
public:
	Scoreboard(void);
	virtual ~Scoreboard(void);

	virtual void start(CEGUI::Window *root);
	virtual void update(float deltatime);
	virtual void stop(void);

	// Status management
	virtual void setActive(bool active, bool autofocus = true); // overrided

private:
	// Wrapper of setActive
	void setActive(bool active, std::list<ScoreboardEntry*> &e);

	// Create all GUI
	void setupInitialState(void);

	// Create entries
	ScoreboardEntry* createEntry(int team, int index);
	CEGUI::Window* createBackground(int index, float offY, const std::string &name);
	CEGUI::Window* createField(int index, float offX, float offY, float sx,
		const std::string &name);

	// Create headers
	void createGameInfosHeader(void);
	void createTeamHeaders(void);
	void createLabels(void);

	// Refresh the scoreboard
	void updateScoreboard(void);
	int refreshPlayersScore(std::list<ScoreboardEntryBuffer> &t1,
		std::list<ScoreboardEntryBuffer> &t2);
	void refreshGUIEntries(int team, std::list<ScoreboardEntryBuffer> &t, int startIndex = 0);
	void refreshGameInfos(void);
	void refreshTeamHeaders(int team);

	// Callbacks
	bool onClick(const CEGUI::EventArgs &eIN);
	bool onEnterArea(const CEGUI::EventArgs &eIN);
	bool onLeaveArea(const CEGUI::EventArgs &eIN);

	// Utils (for code clarity)
	void setFieldText(ScoreboardEntry *e, const ScoreboardEntryBuffer &p);
	void disableEntry(ScoreboardEntry *e);
	void resetFields(ScoreboardEntry *e);
	void failToCreateNewEntry(void);
	void setImageButtonImages(CEGUI::Window *w, const std::string &normalImage,
		const std::string &hoverImage = "", const std::string &pushedImage = "");
	CEGUI::Window* createStaticText(const std::string &scheme, const std::string &name,
		const std::string &alignement, float posX, float posY, float sX, float sY,
		bool hasOver);
	CEGUI::Window* createImageButton(const std::string &scheme, const std::string &name,
		float posX, float posY, float sX, float sY, bool hasOver);

	// Add button and bind callbacks
	void addButton(const std::string &name, CEGUI::Window* w, bool hasOver = false);

private:
	// Refresh every seconds
	float _refreshTime;

	// Main window
	CEGUI::Window *_mainWindow;

	// Entries
	std::list<ScoreboardEntry*> _entries1;
	std::list<ScoreboardEntry*> _entries2;

	// Contains only pointer to _entries1 and _entries2
	std::list<ScoreboardEntry*> _entriesFFA;

	// Headers
	ScoreboardEntry *_labels;
	ScoreboardGameHeader *_gameInfos;
	ScoreboardTeamHeader *_teamHeaders;
};

#endif
