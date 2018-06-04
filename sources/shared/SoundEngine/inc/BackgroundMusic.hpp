#ifndef __BACKGROUNDMUSIC_HPP__
#define __BACKGROUNDMUSIC_HPP__

#include <SFML/Audio/Music.hpp>

class BackgroundMusic : public sf::Music
{
public:
	BackgroundMusic(const std::string &fileName);

	void	init();
	void	update(const sf::Time &);
	void	stopMusic();

	bool			isReady() const;
	bool			isStarted() const;
	bool			isFinished() const;

private:
	std::string		_filename;

	bool			_started;
	bool			_ready;
	bool			_finished;
};

#endif //__BACKGROUNDMUSIC_HPP__