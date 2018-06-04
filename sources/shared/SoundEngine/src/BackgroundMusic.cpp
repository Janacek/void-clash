#include <stdexcept>
#include "BackgroundMusic.hpp"

extern int volume;

BackgroundMusic::BackgroundMusic(const std::string &fileName) :
_filename(fileName), _started(false), _ready(false), _finished(false)
{

}

void	BackgroundMusic::init()
{
	if (!openFromFile(_filename))
		throw std::runtime_error("Unable to load music : " + _filename);
	_ready = true;
}

void	BackgroundMusic::update(const sf::Time &time)
{
	if (!_started)
	{
		setVolume(volume);
		play();
		_started = true;
	}
	if (getStatus() == sf::SoundSource::Stopped)
		_finished = true;
}

void	BackgroundMusic::stopMusic()
{
	stop();
}

bool	BackgroundMusic::isReady() const
{
	return _ready;
}

bool	BackgroundMusic::isStarted() const
{
	return _started;
}

bool	BackgroundMusic::isFinished() const
{
	return _finished;
}
