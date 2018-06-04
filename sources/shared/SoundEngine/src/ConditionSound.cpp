#include "ConditionSound.hpp"
#include "Map.hpp"
#include "Flag.hpp"

extern int volume;

ConditionSound::ConditionSound(sf::SoundBuffer *soundData, int volFactor, bool spatialisation, eConditionSoundType conditionSoundtype, AObject *emitter) :
ASound(soundData, volFactor, emitter), _conditionSoundtype(conditionSoundtype), _spatialisation(spatialisation)
{

}

void	ConditionSound::init()
{
	_ready = true;
	_conditionTypeToMethod[ISEMITTERFOLLOWEDPLAYER] = &ConditionSound::_isEmitterFollowedPlayer;
	_conditionTypeToMethod[ISEMITTERMYFLAG] = &ConditionSound::_isEmitterMyFlag;
	_conditionTypeToMethod[ISEMITTERAGRAVITY] = &ConditionSound::_isEmitterAGravityField;
	if (!(this->*_conditionTypeToMethod[_conditionSoundtype])() || !_setSoundSpatialisation())
		_finished = true;
	else
	{
		_soundPlayer = new sf::Sound();
		_soundPlayer->setBuffer(*_soundData);
		_soundPlayer->setVolume(_volFactor * volume / 100);
		if (_spatialisation) {
			float rangeX = (_emitter->getX() - S_Map->getPlayerFollowed()->getX()) / (S_Map->getZoom() * 200.0f);
			float rangeY = (_emitter->getY() - S_Map->getPlayerFollowed()->getY()) / (S_Map->getZoom() * 200.0f);
			_soundPlayer->setPosition(rangeX, rangeY, 0);
			_soundPlayer->setMinDistance(10.0f);
			_soundPlayer->setAttenuation(7.0f);
		}
	}
}

void	ConditionSound::update(const sf::Time &time)
{
	if (_finished)
		return;
	if (!_started) {
			_soundPlayer->play();
			_started = true;
	}
	if (_soundPlayer->getStatus() == sf::SoundSource::Stopped)
		_finished = true;
}

bool	ConditionSound::_setSoundSpatialisation()
{
	if (!_spatialisation)
		return true;
	if (!_emitter || !S_Map->getPlayerFollowed())
		return false;
	float rangeX = (_emitter->getX() - S_Map->getPlayerFollowed()->getX()) / (S_Map->getZoom() * 200.0f);
	float rangeY = (_emitter->getY() - S_Map->getPlayerFollowed()->getY()) / (S_Map->getZoom() * 200.0f);
	if (std::sqrt(std::pow(rangeX, 2) + std::pow(rangeY, 2)) > 30.0f)
		return false;
	return true;
}

bool	ConditionSound::_isEmitterFollowedPlayer()
{
	if (!_emitter)
		return false;
	if (_emitter == S_Map->getPlayerFollowed().get())
		return true;
	return false;
}

bool	ConditionSound::_isEmitterMyFlag()
{
	if (!_emitter || !S_Map->getCurrentPlayer())
		return false;
	Flag *tmp = dynamic_cast<Flag *>(_emitter);
	if (tmp && tmp != S_Map->getFlag(S_Map->getCurrentPlayer()->getTeam()))
		return true;
	return false;
}

bool	ConditionSound::_isEmitterAGravityField()
{
	if (!_emitter)
		return false;
	if (_emitter->getType() != GRAVITY_FIELD)
		return true;
	return false;
}

bool	ConditionSound::_setSoundBufferDependOfTargetHitted()
{
	//if (!_emitter)
	//	return false;
	//if (_emitter == S_Map->getPlayerFollowed.get() && 
	//	_emitter->getOwner().get() == )
	//	return true;
	return true;
}