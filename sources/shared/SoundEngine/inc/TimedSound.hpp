#ifndef __TIMEDSOUND_HPP__
#define __TIMEDSOUND_HPP__

#include "ASound.hpp"

class TimedSound : virtual public ASound
{
public:
	TimedSound(sf::SoundBuffer *, int, float, AObject * = NULL);

	virtual void	init();
	virtual void	update(const sf::Time &);

private:
	bool		_setSoundSpatialisation();

	float		_duration;
	float		_startedTime;
};

#endif //__BASICSOUND_HPP__