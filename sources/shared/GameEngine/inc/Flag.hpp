//
// Flag.hpp for Void Clash in /home/gegon/pfa/sources/client/Common/src
// 
// Made by Gegon
// Login   <gegon@epitech.net>
// 
// Started on  Fri Dec 13 21:34:32 2013 Gegon
// Last update Fri Dec 20 02:30:53 2013 Gegon
//

#ifndef		FLAG_HPP_
# define	FLAG_HPP_

#include	"AObject.hpp"
#include	"Player.hpp"

#define	DELAY_RESPAWN_FLAG 15.0f

class	Flag : public AObject
{
public:
	Flag();
	Flag(float, float, float, float, sf::Uint16);
	virtual ~Flag();

	virtual bool	update();

	void		resetOwner();
	void		setOwner(std::shared_ptr<Player>p);
	std::shared_ptr<Player>	getOwner();

	bool		checkTimer();
	bool		checkCapture();
	void		checkIfPlayerTook();
	bool		checkIfOwnerDisconnected();
	bool flagAtSpawn(void);

	void		reset(bool afterCapture = false);

	const float&	getInitX() const;
	const float&	getInitY() const;

	friend sf::Packet& operator >>(sf::Packet& packet, Flag &m);

private:
	bool _flagAtSpawn;
	float		_time;
	sf::Clock	_clock;

	std::shared_ptr<Player>	_owner;

	std::pair<float, float> _initPos;
};

#endif
