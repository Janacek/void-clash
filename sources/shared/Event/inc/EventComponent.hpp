/********************************************************************
**
**              EventComponent.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef		__EVENTCOMPONENT_H__
# define	__EVENTCOMPONENT_H__

#include	"AObject.hpp"

enum		eventType	// Arguments of s_event (Trigger - Data)
{
	ev_UNKNOWN,

	// Creation / Deletion events for Engines::list syncronisation
	ev_START,			// Object - NULL
	ev_DELETE,			// Object - NULL

	// Gameplay events
	ev_BOMB_PRIMED,		// Player (owner) - Bomb
	ev_BOMB_LAUNCHED,	// Player (owner) - NULL
	ev_BULLET_LAUNCHED,	// Player (owner) - NULL
	ev_ROCKET_LAUNCHED,	// Player (owner) - NULL
	ev_TURRET_LAUNCHED,	// Player (owner) - NULL
	ev_GRAVITY_LAUNCHED,	// Player (owner) - NULL
	ev_TURRET_FIRE,		// Player (owner) - AObject (bullet or rocket)
	ev_WALL_COLLISION,	// AObject of collided object - t_impact struct
	ev_EXPLOSION,		// Explosion - NULL
	ev_PLAYER_BOOST_BOMB,	// Player - NULL
	ev_PLAYER_HIT,		// Hitter (ex: bullet) - Hitted
	ev_PLAYER_HEAL,		// Healer (ex: bullet) - Healed
	ev_KILL,		// Serv: Hitter (ex: bullet) - Killed | Client: Killer - Killed
	ev_TURRET_HIT,		// Hitted - Hitter (ex: bullet)
	ev_TURRET_DESTROYED,	// Hitted - Hitter (ex: bullet)
	ev_BOT_HIT,		// Hitter (ex: bullet) - Hitted
	ev_BOT_DESTROYED,	// Hitter (ex: bullet) - Hitted
	ev_CAPTURE_FLAG,	// Flag - Player which capture
	ev_TOUCH_FLAG,		// Flag - Player which touch
	ev_DROP_FLAG,		// Flag - Player which had the flag
	ev_RESPAWN_FLAG,	// Flag - NULL
	ev_ZONE_CAPTURED,	// Capture - NULL

	// Input
	ev_PLAYER_ACTION,	// Player - s_actions
	ev_MUTE_SOUND,		// NULL - NULL

	// GraphicEngine event
	ev_CHAT_SWITCH,		// NULL - NULL
	ev_CHAT_TO_SEND,	// NULL - NULL
	ev_CHAT_MSG_CHANGE,	// NULL - msg (to delete)
	ev_CHAT_MSG_RECEIVED,	// NULL - t_msgHandler* (to delete)
	ev_SERVER_CONNECTIVITY,	// NULL - unsigned int *_msSinceLastActivity
	ev_INCORRECT_VERSION,	// NULL - int: version id on serv (check VOID_CLASH_VERSION for client version)

	// NETWORK
	ev_CONFIG,		// NULL
	ev_CONFIG_RECEIVED,	// NULL (triggered when client receive config packet from server)
	ev_WELCOME,		// NULL
	ev_SERVER_FULL,	// NULL
	ev_JOIN_TEAM,	// int: team

	// Map
	ev_GAME_START,		// NULL (warmup done - game starts)
	ev_SWITCH_MAP_MODE,	// NULL
	ev_END_ROUND,		// Player (winner)	(When received, client should display game result until ev_START_ROUND)
	ev_START_ROUND,		// NULL	
	ev_END_MAP,			// Player (MVP) - NULL			(When received client should display game result until ev_CHANGE_MAP)
	ev_CHANGE_MAP,
	ev_REQUEST_CHANGE_MAP,		// NULL
	ev_HORDE_ACTIVATE,	// NULL
	ev_HORDE_DESACTIVATE,	// NULL
	ev_MAP_LOADED, // NULL used for map editor

	//Menu
	ev_QUIT,			// NULL
	//ev_OPEN_MENU,		// NULL				When Esc key is pressed in mode RUN
	//ev_CLOSE_MENU,		// NULL				When Esc key is pressed in mode MENU
	//ev_MENU_ACTION,		// NULL - std::pair<posX, posY> of click
	ev_REBOOT_GRAPHIC,      // NULL - NULL
	ev_WEAPON_SELECTION,	// Player - std::pair<t_weapon*, t_weapon*>*

	ev_IN_SPEEDFIELD,	// Player - NULL

	//Admin Serv
	ev_PLAYER_KICK,

	//GUI
	ev_MOUSE_ENTERS_GUI,
	ev_MOUSE_LEAVES_GUI,
	ev_OBJECT_SELECTED,
	ev_OBJECT_DESELECTED,

	ev_OBJECT_CREATION,
	ev_OBJECT_CREATED,

	// Change NickiMinajName
	ev_NICKNAME_CHANGED,	// NULL - std::string * (to delete)
};

struct s_event
{
public:
	s_event(const std::shared_ptr<AObject>& triggerIN  = NULL, void *dataIN = NULL) :
		data(dataIN) {
		trigger = triggerIN;
	}

	std::string raiseInfo;
	std::shared_ptr<AObject> trigger;
	void* data;
};

#endif		/* !__EVENTCOMPONENT_H__ */
