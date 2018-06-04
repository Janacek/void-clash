#ifndef __CONDITIONSOUND_HPP__
#define __CONDITIONSOUND_HPP__

#include "ASound.hpp"

enum eConditionSoundType {
	ISEMITTERFOLLOWEDPLAYER,
	ISEMITTERMYFLAG,
	ISEMITTERAGRAVITY
};

class ConditionSound : virtual public ASound
{
public:
	ConditionSound(sf::SoundBuffer *, int, bool, eConditionSoundType, AObject * = NULL);

	virtual void	init();
	virtual void	update(const sf::Time &);

private:
	bool			_setSoundSpatialisation();
	bool			_isEmitterFollowedPlayer();
	bool			_isEmitterMyFlag();
	bool			_isEmitterAGravityField();
	bool			_setSoundBufferDependOfTargetHitted();

	eConditionSoundType	_conditionSoundtype;
	bool		_spatialisation;
	std::map<eConditionSoundType, bool (ConditionSound::*)()> _conditionTypeToMethod;
};

#endif //__ISOLATEDSOUND_HPP__