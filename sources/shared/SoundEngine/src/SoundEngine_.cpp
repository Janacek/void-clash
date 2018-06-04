#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "SoundEngine_.hpp"
#include "Manager.hpp"
#include "ConfigParser.hpp"
#include "AssetPath.h"
#include "Files.hpp"

extern t_config *G_conf;
extern int	volume;

SoundEngine_::SoundEngine_() :
_mute(false)
{}

SoundEngine_::~SoundEngine_(void)
{
}

//////////////////////////////////////////////////////////////////////
void	SoundEngine_::start(void)
{
	_mute = 1;
	//Sounds loading
	_soundEventTypeToBuffer[ev_BULLET_LAUNCHED] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_BULLET_LAUNCHED]->loadFromFile(ASSETS_PATH + "assets/sound/weapon/bullet.wav"))
		throw std::runtime_error("Unable to load sound 'bullet.wav'");

	_soundEventTypeToBuffer[ev_WALL_COLLISION] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_WALL_COLLISION]->loadFromFile(ASSETS_PATH + "assets/sound/collision_wall.wav"))
		throw std::runtime_error("Unable to load sound 'collision_wall.wav'");

	_soundEventTypeToBuffer[ev_EXPLOSION] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_EXPLOSION]->loadFromFile(ASSETS_PATH + "assets/sound/weapon/explosion.wav"))
		throw std::runtime_error("Unable to load sound 'explosion.wav'");

	_soundEventTypeToBuffer[ev_TURRET_LAUNCHED] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_TURRET_LAUNCHED]->loadFromFile(ASSETS_PATH + "assets/sound/weapon/turret_deployed.wav"))
		throw std::runtime_error("Unable to load sound 'turret_deployed.wav'");

	_soundEventTypeToBuffer[ev_ZONE_CAPTURED] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_ZONE_CAPTURED]->loadFromFile(ASSETS_PATH + "assets/sound/zone_captured.wav"))
		throw std::runtime_error("Unable to load sound 'zone_captured.wav'");

	_soundEventTypeToBuffer[ev_TOUCH_FLAG] = new sf::SoundBuffer();
	if (!_soundEventTypeToBuffer[ev_TOUCH_FLAG]->loadFromFile(ASSETS_PATH + "assets/sound/flag_stolen.wav"))
		throw std::runtime_error("Unable to load sound 'flag_stolen.wav'");

	_soundTriggerTypeToBuffer[trig_HIT_ENNEMY] = new sf::SoundBuffer();
	if (!_soundTriggerTypeToBuffer[trig_HIT_ENNEMY]->loadFromFile(ASSETS_PATH + "assets/sound/hit/enemy_hit.wav"))
		throw std::runtime_error("Unable to load sound 'enemy_hit.wav'");

	_soundTriggerTypeToBuffer[trig_HIT_MYSELF] = new sf::SoundBuffer();
	if (!_soundTriggerTypeToBuffer[trig_HIT_MYSELF]->loadFromFile(ASSETS_PATH + "assets/sound/hit/myself_hit.wav"))
		throw std::runtime_error("Unable to load sound 'myself_hit.wav'");

	_soundTriggerTypeToBuffer[trig_STAGE_END] = new sf::SoundBuffer();
	if (!_soundTriggerTypeToBuffer[trig_STAGE_END]->loadFromFile(ASSETS_PATH + "assets/sound/end_round.wav"))
		throw std::runtime_error("Unable to load sound 'end_round.wav'");


	//_loadBackgroundMusics();
	//Basic Sound generator helpers init
	_basicSoundEvents.push_back(ev_BULLET_LAUNCHED);
	_basicSoundEvents.push_back(ev_EXPLOSION);
	_basicSoundEvents.push_back(ev_TURRET_LAUNCHED);
	_basicSoundEvents.push_back(ev_ZONE_CAPTURED);

	//Condition Sound generator helpers init
	_conditionSoundEvents.push_back(ev_WALL_COLLISION);
	_soundTypeToConditionSound[ev_WALL_COLLISION] = ISEMITTERFOLLOWEDPLAYER;

	_conditionSoundEvents.push_back(ev_TOUCH_FLAG);
	_soundTypeToConditionSound[ev_TOUCH_FLAG] = ISEMITTERMYFLAG;

	//Music Background helpers init
	//_backgroundMusicEvents.push_back(ev_GAME_START);

	//Sounds Balances
	_balanceSounds[ev_BULLET_LAUNCHED] = 15;
	_balanceSounds[ev_WALL_COLLISION] = 10;
	_balanceSounds[ev_EXPLOSION] = 70;
	_balanceSounds[ev_TURRET_LAUNCHED] = 50;
	_balanceSounds[ev_ZONE_CAPTURED] = 40;
	_balanceSounds[ev_TOUCH_FLAG] = 80;
}

//---------------------------------//

eGameState	SoundEngine_::update(const sf::Time &time)
{
	std::list<std::pair<eventType, s_event> >::const_iterator it;
	std::list<eventType> currentBuffer;

	if (Event::getEventByType(ev_MUTE_SOUND))
		switchMute();
	if (_mute || volume == 0)
		return RUN;
	_updateDatabaseWithEvents();
	_updateDatabaseWithTriggers();
	_updateSounds(time);
	//_updateMusics(time);
	return RUN;
}

//---------------------------------//

void	SoundEngine_::stop(void)
{
}

//********************************************************************//
//--------------------------------------------------------------------//
//------------------------ PRIVATE FUNCTION --------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//********************************************************************//

//--------------------------------//
//------- UPDATE FUNCTION --------//
//--------------------------------//

void	SoundEngine_::_updateDatabaseWithEvents()
{
	std::list<std::pair<eventType, s_event> >::const_iterator it;
	std::list<eventType> currentBuffer;

	for (it = Event::getMainEventList()->begin(); it != Event::getMainEventList()->end(); ++it)
	{
		if (_isEventInThisCategory(_basicSoundEvents, it->first) && !_isEventInThisCategory(currentBuffer, it->first))
		{
			AObject * emitter = it->second.trigger ? it->second.trigger.get() : NULL;
			_soundInProgress.push_back(new BasicSound(_soundEventTypeToBuffer[it->first], _balanceSounds[it->first], true, emitter));
		}
		else if (_isEventInThisCategory(_conditionSoundEvents, it->first) && !_isEventInThisCategory(currentBuffer, it->first))
		{
			AObject * emitter = it->second.trigger ? it->second.trigger.get() : NULL;
			_soundInProgress.push_back(new ConditionSound(_soundEventTypeToBuffer[it->first], _balanceSounds[it->first], true, _soundTypeToConditionSound[it->first], emitter));
		}
		else if (it->first == ev_PLAYER_HIT && it->second.trigger != NULL && !_isEventInThisCategory(currentBuffer, it->first))
		{
			AObject * emitter = it->second.trigger ? it->second.trigger.get() : NULL;
			if (it->second.trigger->getOwner() == S_Map->getPlayerFollowed() &&
				it->second.trigger->getOwner().get() != it->second.data)
				_soundInProgress.push_back(new ConditionSound(_soundTriggerTypeToBuffer[trig_HIT_ENNEMY], 80, false, ISEMITTERAGRAVITY, emitter));
			if (it->second.data == S_Map->getPlayerFollowed().get())
				_soundInProgress.push_back(new ConditionSound(_soundTriggerTypeToBuffer[trig_HIT_MYSELF], 35, false, ISEMITTERAGRAVITY, emitter));
		}
		currentBuffer.push_back(it->first);
	}
	currentBuffer.clear();
}

void	SoundEngine_::_updateDatabaseWithTriggers()
{
	static bool	_warmUpTimeChecked = false;
	static bool	_mapTimeChecked = false;

	if (_isStageFinishing(S_Map->getWarmupTime().asMilliseconds(), _warmUpTimeChecked))
		_soundInProgress.push_back(new TimedSound(_soundTriggerTypeToBuffer[trig_STAGE_END], 30, S_Map->getWarmupTime().asMilliseconds(), NULL));
	if (_isStageFinishing(S_Map->getTimeRemaningOnMap().asMilliseconds(), _mapTimeChecked))
		_soundInProgress.push_back(new TimedSound(_soundTriggerTypeToBuffer[trig_STAGE_END], 30, S_Map->getTimeRemaningOnMap().asMilliseconds(), NULL));
}

//--------------------------------//
//------  SOUNDS FUNCTION --------//
//--------------------------------//

void	SoundEngine_::_updateSounds(const sf::Time &time)
{
	for (unsigned int i = 0; i < _soundInProgress.size(); ++i)
	{
		if (!_soundInProgress[i]->isReady())
			_soundInProgress[i]->init();
		else
			_soundInProgress[i]->update(time);



		if (_soundInProgress[i]->isFinished()) {
			_soundInProgress[i]->stop();
			_soundInProgress.erase(_soundInProgress.begin() + i);
		}
	}
}

//--------------------------------//
//------  MUSICS FUNCTION --------//
//--------------------------------//

void	SoundEngine_::_updateMusics(const sf::Time &time)
{
	if (!_musicInProgress.size())
		_musicInProgress.push_back(new BackgroundMusic(_nextMusic()));
	for (unsigned int i = 0; i < _musicInProgress.size(); ++i)
	{
		if (!_musicInProgress[i]->isReady())
			_musicInProgress[i]->init();
		else
			_musicInProgress[i]->update(time);



		if (_musicInProgress[i]->isFinished()) {
			_musicInProgress[i]->stop();
			_musicInProgress.erase(_musicInProgress.begin() + i);
			_musicInProgress.push_back(new BackgroundMusic(_nextMusic()));
		}
	}
}

//---------------------------------//

void	SoundEngine_::_loadBackgroundMusics()
{
	void		*handle = NULL;
	std::vector<std::string>	tmpMusicsList;
	std::string musicDir = std::string(ASSETS_PATH + "assets/musics/");
	char *musicName;
	if (!(handle = Files::first(musicDir.c_str(), &musicName)))
		throw std::runtime_error("Unable to load music : cannot open folder");
	if (Files::isFile(Files::getPath(musicDir.c_str(), musicName))) {
		tmpMusicsList.push_back(musicDir + musicName);
	}
	while ((musicName = Files::next(handle)))
		if (Files::isFile(Files::getPath(musicDir.c_str(), musicName))) {
			tmpMusicsList.push_back(musicDir + musicName);
		}
	while (tmpMusicsList.size())
	{
		int pos = std::rand() % tmpMusicsList.size();
		_randomizedMusicsList.push_back(tmpMusicsList[pos]);
		tmpMusicsList.erase(tmpMusicsList.begin() + pos);
	}
}

//---------------------------------//

const std::string &SoundEngine_::_randomizeNextMusics()
{
	return _randomizedMusicsList[std::rand() % _randomizedMusicsList.size()];
}

//---------------------------------//

const std::string &SoundEngine_::_nextMusic()
{
	static int currentMusic = -1;

	currentMusic = (currentMusic + 1) % _randomizedMusicsList.size();
	return _randomizedMusicsList[currentMusic];
}

//--------------------------------//
//----- OWN EVENT FUNCTION -------//
//--------------------------------//

bool	SoundEngine_::_isStageFinishing(float time, bool &alreadyCheck)
{
	if (!alreadyCheck && time < 10000.f && time > 0.f) {
		alreadyCheck = true;
		return true;
	}
	else if (time <= 0.0f) {
		alreadyCheck = false;
	}
	return false;
}


//--------------------------------//
//------ GENERAL FUNCTION --------//
//--------------------------------//

bool	SoundEngine_::_isEventInThisCategory(std::list < eventType > category, eventType type) const
{
	if (std::find(category.begin(), category.end(), type) != category.end())
		return true;
	return false;
}

void	SoundEngine_::switchMute()
{
	if (_mute)
		_mute = false;
	else
		_mute = true;
}
