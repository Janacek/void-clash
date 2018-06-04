//
// SoundEngine.hpp for Void Clash in /home/gegon/PFA/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Aug 14 10:32:31 2013 brunel guillaume
// Last update Wed Mar 19 12:42:26 2014 sadin_n
//

#ifndef         __SOUNDENGINE_HPP__
# define        __SOUNDENGINE_HPP__

#include	<SFML/Audio.hpp>
#include	"AEngine.hpp"
#include	"AObject.hpp"
#include	"Bomb.hpp"
#include	"GUIManager.hpp"

enum	eSound
  {
    SOUND_PRIME_BOMB,
	SOUND_SHOOT_BULLET,
	SOUND_SPEED_BOOST,
	SOUND_MY_HIT,
	SOUND_GOT_HIT,
	SOUND_EXPLO,
	SOUND_COLLISION_WALL,
	SOUND_SUICIDE,
	SOUND_DOUBLE_KILL,
	SOUND_TRIPLE_KILL,
	SOUND_FOUR_KILL,
	SOUND_FIVE_KILL
  };

class	Manager;

class   SoundEngine : public AEngine
{
public :
  SoundEngine();
  virtual ~SoundEngine(void);

  virtual void start(void);
  virtual void stop(void);
  virtual eGameState update(const sf::Time &deltaTime);

  void switchMute(void);
  void playInGameMusic(void);
  void	playSound(eSound sound, std::shared_ptr<AObject>emitter = NULL);
  void stopSound(eSound);
  void balanceSounds(void);

private :
  Manager			*_manager;

  bool				_mute;
  sf::Music			*_music;
  std::vector<sf::Sound>	_sound;
  std::vector<sf::SoundBuffer>	_buf;

  // tmp
  // _curBomb stock associated bomb with PRIME_BOMB, used to stop sound
  Bomb				*_curBomb;
};

#endif		// __SOUNDENGINE_HPP__
