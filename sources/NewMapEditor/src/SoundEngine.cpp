//
// SoundEngine.cpp for Void Clash in /home/gegon/PFA/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Aug 14 10:32:37 2013 brunel guillaume
// Last update Wed Mar 19 12:41:55 2014 sadin_n
//

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <SFML/Audio.hpp>
#include "SoundEngine.hpp"
#include "Manager.hpp"
#include "ConfigParser.hpp"
#include "AssetPath.h"

extern t_config *G_conf;
extern int	volume;
extern bool G_isMapEditor;

SoundEngine::SoundEngine() :
	 _music(NULL)
{}

SoundEngine::~SoundEngine(void)
{
}

//////////////////////////////////////////////////////////////////////

void	SoundEngine::start(void)
{
	_music = new sf::Music;
	_mute = false;

	_buf.resize(30);
	_sound.resize(30);

	//if (!_buf[SOUND_PRIME_BOMB].loadFromFile(ASSETS_PATH + "assets/sound/timer.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_PRIME_BOMB].setBuffer(_buf[SOUND_PRIME_BOMB]);

	//if (!_buf[SOUND_SHOOT_BULLET].loadFromFile(ASSETS_PATH + "assets/sound/bullet.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_SHOOT_BULLET].setBuffer(_buf[SOUND_SHOOT_BULLET]);

	//// TODO: CHANGE ASSET
	//if (!_buf[SOUND_SPEED_BOOST].loadFromFile(ASSETS_PATH + "assets/sound/shotgun.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_SPEED_BOOST].setBuffer(_buf[SOUND_SPEED_BOOST]);

	//if (!_buf[SOUND_COLLISION_WALL].loadFromFile(ASSETS_PATH + "assets/sound/collision_wall.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_COLLISION_WALL].setBuffer(_buf[SOUND_COLLISION_WALL]);

	//if (!_buf[SOUND_EXPLO].loadFromFile(ASSETS_PATH + "assets/sound/explosion.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_EXPLO].setBuffer(_buf[SOUND_EXPLO]);

	////Hit
	//if (!_buf[SOUND_MY_HIT].loadFromFile(ASSETS_PATH + "assets/sound/hit/my_hit.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_MY_HIT].setBuffer(_buf[SOUND_MY_HIT]);

	//if (!_buf[SOUND_GOT_HIT].loadFromFile(ASSETS_PATH + "assets/sound/hit/got_hit.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_GOT_HIT].setBuffer(_buf[SOUND_GOT_HIT]);

	//// Kills
	//if (!_buf[SOUND_SUICIDE].loadFromFile(ASSETS_PATH + "assets/sound/kills/suicide.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_SUICIDE].setBuffer(_buf[SOUND_SUICIDE]);

	//if (!_buf[SOUND_DOUBLE_KILL].loadFromFile(ASSETS_PATH + "assets/sound/kills/dominating.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_DOUBLE_KILL].setBuffer(_buf[SOUND_DOUBLE_KILL]);

	//if (!_buf[SOUND_TRIPLE_KILL].loadFromFile(ASSETS_PATH + "assets/sound/kills/unstopable.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_TRIPLE_KILL].setBuffer(_buf[SOUND_TRIPLE_KILL]);

	//if (!_buf[SOUND_FOUR_KILL].loadFromFile(ASSETS_PATH + "assets/sound/kills/rampage.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_FOUR_KILL].setBuffer(_buf[SOUND_FOUR_KILL]);

	//if (!_buf[SOUND_FIVE_KILL].loadFromFile(ASSETS_PATH + "assets/sound/kills/holyshit.wav"))
	//	throw std::runtime_error("Unable to load sound");
	//_sound[SOUND_FIVE_KILL].setBuffer(_buf[SOUND_FIVE_KILL]);

	balanceSounds();
}

//---------------------------------//

eGameState	SoundEngine::update(const sf::Time &)
{
	std::list<std::pair<eventType, s_event> >::const_iterator it;

	//CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("KeysListbox"));
	if (G_isMapEditor == false)
	{
		CEGUI::Window* myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
		CEGUI::ToggleButton *checkbox = static_cast<CEGUI::ToggleButton *>(myRoot->getChildRecursive("MuteSound"));
		//CEGUI::Scrollbar *soundVolumeScrollbar = static_cast<CEGUI::Scrollbar *>(myRoot->getChildRecursive("SoundValueSlider"));

		_mute = checkbox->isSelected();
	}
	for (it = Event::getMainEventList()->begin(); it != Event::getMainEventList()->end(); ++it)
	{
		if (it->first == ev_BOMB_PRIMED)
		{
			// Check if own bomb
			if (it->second.trigger == S_Map->getPlayerFollowed())
			{
				playSound(SOUND_PRIME_BOMB);
				_curBomb = (Bomb *)it->second.data;
			}
		}
		else if (it->first == ev_BULLET_LAUNCHED)
		{
			if (it->second.trigger == S_Map->getPlayerFollowed())
				playSound(SOUND_SHOOT_BULLET);
		}
		// trigger == hitter | data == player
		else if (it->first == ev_PLAYER_HIT && it->second.trigger != NULL)
		{
			// If I hit someone
			if (it->second.trigger->getOwner() == S_Map->getPlayerFollowed() &&
				it->second.trigger->getOwner().get() != it->second.data)
				playSound(SOUND_MY_HIT);
			if (it->second.data == S_Map->getPlayerFollowed().get())
				playSound(SOUND_GOT_HIT);
			// Sound choice according to hitter type
			// if (it->second.trigger->getType() == BULLET)
			//   playSound(BULLET_HIT);
		}
		else if (it->first == ev_EXPLOSION)
			playSound(SOUND_EXPLO, it->second.trigger);
		//else if (it->first == ev_WALL_COLLISION)
		//{
		//	if (it->second.trigger == S_Map->getPlayerFollowed())
		//		playSound(SOUND_COLLISION_WALL);
		//}
		else if (it->first == ev_KILL)
		{
			Player *killer = (Player *)it->second.trigger.get();
			Player *killed = (Player *)it->second.data;
			if (killer && killed)
			{
				//if (killer == killed)
				//	playSound(SOUND_SUICIDE);
				//if (killer->getKillsStreak() == 2)
				//	playSound(SOUND_DOUBLE_KILL);
				//if (killer->getKillsStreak() == 3)
				//	playSound(SOUND_TRIPLE_KILL);
				//if (killer->getKillsStreak() == 4)
				//	playSound(SOUND_FOUR_KILL);
				//if (killer->getKillsStreak() >= 5)
				//	playSound(SOUND_FIVE_KILL);
			}
		}
		else if (it->first == ev_DELETE && it->second.trigger)
		{
			// Used to delete prime bomb when player going inside resupply
			if (it->second.trigger->getType() == BOMB &&
				it->second.trigger->getOwner() == S_Map->getPlayerFollowed() &&
				S_Map->getPlayerFollowed()->getBombOnHold() == NULL &&
				_curBomb == it->second.trigger.get())
				stopSound(SOUND_PRIME_BOMB);
		}
		else if (it->first == ev_MUTE_SOUND)
			switchMute();
	}
	return RUN;
}

//---------------------------------//

void	SoundEngine::stop(void)
{
	delete _music;
	_sound.clear();
	_buf.clear();
}

///////////////////////////////////////////////
/////   Utilities funcs

void	SoundEngine::balanceSounds()
{
	_sound[SOUND_COLLISION_WALL].setVolume(0 * volume / 100);
	_sound[SOUND_SHOOT_BULLET].setVolume(50 * volume / 100);
	_sound[SOUND_SPEED_BOOST].setVolume(50 * volume / 100);
	_sound[SOUND_PRIME_BOMB].setVolume(70 * volume / 100);
	_sound[SOUND_GOT_HIT].setVolume(50 * volume / 100);
	_sound[SOUND_DOUBLE_KILL].setVolume(60 * volume / 100);
	_sound[SOUND_TRIPLE_KILL].setVolume(60 * volume / 100);
	_sound[SOUND_FOUR_KILL].setVolume(60 * volume / 100);
	_sound[SOUND_FIVE_KILL].setVolume(60 * volume / 100);
}

void	SoundEngine::switchMute()
{
	_mute ^= 1;

	// Sounds
	std::vector<sf::Sound>::iterator	it = _sound.begin();
	std::vector<sf::Sound>::iterator	end = _sound.end();
	if (_mute)
	{
		while (it != end)
		{
			(*it).setVolume(0);
			++it;
		}
	}
	else
	{
		while (it != end)
		{
			(*it).setVolume(100);
			++it;
		}
		balanceSounds();
	}
}

///////////////////////////////////////////////
/////   Play sounds

// Emitter is used to spatialize sound accord to current player

void	SoundEngine::playSound(eSound sound, std::shared_ptr<AObject>emitter)
{
	if (emitter)
	{
		_sound[sound].setMinDistance(10.0f);
		_sound[sound].setAttenuation(7.0f);
		float rangeX = (emitter->getX() - S_Map->getPlayerFollowed()->getX()) / (S_Map->getZoom() * 200.0f);
		float rangeY = (emitter->getY() - S_Map->getPlayerFollowed()->getY()) / (S_Map->getZoom() * 200.0f);
		if (std::sqrt(std::pow(rangeX, 2) + std::pow(rangeY, 2)) > 30.0f)
			return;
		_sound[sound].setPosition(rangeX, rangeY, 0);
	}
	if (!_mute)
	{
		CEGUI::Window* myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
		CEGUI::Slider *soundVolumeScrollbar = static_cast<CEGUI::Slider *>(myRoot->getChildRecursive("SoundValueSlider"));
		
		if (soundVolumeScrollbar)
			_sound[sound].setVolume(soundVolumeScrollbar->getCurrentValue());
		
		_sound[sound].play();
	}
}


void	SoundEngine::stopSound(eSound sound)
{
	_sound[sound].stop();
}

///////////////////////////////////////////////
/////   Play musics

void	SoundEngine::playInGameMusic()
{
	// if (!_music->openFromFile("assets/music/in_game.ogg"))
	//   {
	//     // error...
	//   }

	// _music->play();
}
