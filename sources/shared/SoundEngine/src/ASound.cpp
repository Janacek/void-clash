#include "ASound.hpp"

extern int volume;

ASound::ASound(sf::SoundBuffer *soundData, int volFactor, AObject *emitter) :
_soundPlayer(NULL), _started(false), _finished(false), _ready(false), _emitter(emitter), _volFactor(volFactor)
{
	_soundData = soundData;
}

ASound::~ASound()
{
}

void	ASound::init()
{
	_soundPlayer = new sf::Sound();
	_soundPlayer->setBuffer(*_soundData);
	_soundPlayer->setVolume(_volFactor * volume / 100);
	_ready = true;
}

void	ASound::stop()
{
	if (!_soundPlayer)
		return;
	_soundPlayer->stop();
	delete _soundPlayer;
}

bool	ASound::isReady() const
{
	return _ready;
}

bool	ASound::isStarted() const
{
	return _started;
}

bool	ASound::isFinished() const
{
	return _finished;
}