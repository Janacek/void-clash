//
// SoundEngine.hpp for Void Clash in /home/gegon/PFA/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Aug 14 10:32:31 2013 brunel guillaume
// Last update Wed Mar 19 12:42:26 2014 sadin_n
//

#ifndef         __SOUNDENGINE__HPP__
# define        __SOUNDENGINE__HPP__

#include	<SFML/Audio/SoundBuffer.hpp>
#include	<map>
#include	<list>
#include	"AEngine.hpp"
#include	"BasicSound.hpp"
#include	"ConditionSound.hpp"
#include	"TimedSound.hpp"
#include	"BackgroundMusic.hpp"
#include	"Event.hpp"

class	Manager;

enum eSoundTriggerType {
	trig_STAGE_END,
	trig_HIT_ENNEMY,
	trig_HIT_MYSELF
};

class   SoundEngine_ : public AEngine
{
public :
  SoundEngine_();
  virtual ~SoundEngine_(void);

  virtual void start(void);
  virtual void stop(void);
  virtual eGameState update(const sf::Time &);

private :

	//******  Private FUNCTION *******//

	// Private Update functions
	void	_updateDatabaseWithEvents();
	void	_updateDatabaseWithTriggers();

	// Sounds function
	void	_updateSounds(const sf::Time &);

	// Musics Function
	void	_updateMusics(const sf::Time &);
	void _loadBackgroundMusics();
	const std::string &_randomizeNextMusics();
	const std::string &_nextMusic();

	// Own event management functions
	bool	_isStageFinishing(float, bool &);

	// General functions
	bool	_isEventInThisCategory(std::list < eventType >, eventType type) const;

	//******  Private ATTRIBUTE *******//

	// Sounds and musics helpers
	std::list<eventType> _basicSoundEvents;
	std::list<eventType> _conditionSoundEvents;
	//std::list<eventType> _backgroundMusicEvents;

	// Musics randomizers
	int						 _currentMusicPos;
	std::vector<std::string> _randomizedMusicsList;
	// Database
	std::vector<ASound *>			_soundInProgress;
	std::vector<BackgroundMusic *>	_musicInProgress;

	// Sounds type helpers
	std::map<eventType, sf::SoundBuffer *> _soundEventTypeToBuffer;
	std::map<eSoundTriggerType, sf::SoundBuffer *> _soundTriggerTypeToBuffer;
	std::map<eventType, eConditionSoundType> _soundTypeToConditionSound;
	std::map<eventType, int> _balanceSounds;

	void switchMute();
	bool _mute;
};

#endif		// __SOUNDENGINE_HPP__
