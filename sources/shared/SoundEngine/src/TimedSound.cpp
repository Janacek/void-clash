#include "TimedSound.hpp"
#include "Map.hpp"

extern int volume;

TimedSound::TimedSound(sf::SoundBuffer *soundData, int volFactor, float duration, AObject *emitter) :
ASound(soundData, volFactor, emitter), _duration(duration)
{

}

void	TimedSound::init()
{
	_soundPlayer = new sf::Sound();
	_soundPlayer->setBuffer(*_soundData);
	_soundPlayer->setVolume(_volFactor * volume / 100);
	_ready = true;
	_startedTime = S_Map->getTime().asMilliseconds();
}

void	TimedSound::update(const sf::Time &time)
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
	else
	{
		if (S_Map->getTime().asMilliseconds() - _startedTime > _duration)
			_finished = true;
	}
	if (_soundPlayer->getStatus() == sf::SoundSource::Stopped)
		_finished = true;
}

bool	TimedSound::_setSoundSpatialisation()
{
	if (!_emitter)
		return true;
	float rangeX = (_emitter->getX() - S_Map->getPlayerFollowed()->getX()) / (S_Map->getZoom() * 200.0f);
	float rangeY = (_emitter->getY() - S_Map->getPlayerFollowed()->getY()) / (S_Map->getZoom() * 200.0f);
	if (std::sqrt(std::pow(rangeX, 2) + std::pow(rangeY, 2)) > 30.0f)
		return false;
	_soundPlayer->setMinDistance(10.0f);
	_soundPlayer->setAttenuation(7.0f);
	_soundPlayer->setPosition(rangeX, rangeY, 0);
	return true;
}