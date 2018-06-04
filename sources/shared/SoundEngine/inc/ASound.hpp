#ifndef __ASOUND_HPP__
#define __ASOUND_HPP__

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <string>
#include "AObject.hpp"

class ASound
{
public:
	ASound(sf::SoundBuffer *, int, AObject * = NULL);
	~ASound();

	virtual void	init();
	virtual void	update(const sf::Time &) = 0;
	virtual void	stop();

	bool			isReady() const;
	bool			isStarted() const;
	bool			isFinished() const;

protected:
	sf::SoundBuffer	*_soundData;
	sf::Sound		*_soundPlayer;
	int				_volFactor;

	AObject			*_emitter;

	bool		_ready;
	bool		_started;
	bool		_finished;
};

#endif //__ASOUND_HPP__