#include "BasicSound.hpp"
#include "Map.hpp"

BasicSound::BasicSound(sf::SoundBuffer *soundData, int volFactor, bool spatialisation, AObject *emitter) :
ASound(soundData, volFactor, emitter), _spatialisation(spatialisation)
{

}


void	BasicSound::update(const sf::Time &time)
{
	if (!_started)
	{
		if (_setSoundSpatialisation())
		{
			_soundPlayer->play();
			_started = true;
		}
		else
			_finished = true;
	}
	if (_soundPlayer->getStatus() == sf::SoundSource::Stopped)
		_finished = true;
}

bool	BasicSound::_setSoundSpatialisation()
{
	if (!_spatialisation)
		return true;
	_soundPlayer->setMinDistance(10.0f);
	_soundPlayer->setAttenuation(7.0f);
	if (!_emitter || !S_Map->getPlayerFollowed())
		return true;
	float rangeX = (_emitter->getX() - S_Map->getPlayerFollowed()->getX()) / (S_Map->getZoom() * 200.0f);
	float rangeY = (_emitter->getY() - S_Map->getPlayerFollowed()->getY()) / (S_Map->getZoom() * 200.0f);
	if (std::sqrt(std::pow(rangeX, 2) + std::pow(rangeY, 2)) > 30.0f)
		return false;
	_soundPlayer->setPosition(rangeX, rangeY, 0);
	return true;
}