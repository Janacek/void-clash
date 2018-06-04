#ifndef __BASICSOUND_HPP__
#define __BASICSOUND_HPP__

#include "ASound.hpp"

class BasicSound: virtual public ASound
{
public:
	BasicSound(sf::SoundBuffer *, int, bool, AObject * = NULL);

	virtual void	update(const sf::Time &);

private:
	bool		_setSoundSpatialisation();
	bool		_spatialisation;
};

#endif //__BASICSOUND_HPP__