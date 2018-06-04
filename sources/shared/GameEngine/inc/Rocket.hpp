#pragma once

#include	<utility>
#include	<SFML/System.hpp>
#include	"AWeapon.hpp"

class	GameEngine;
class	Player;

class	Rocket : public AWeapon
{
public:
	Rocket();
	Rocket(float, float, float, float, float, float, const std::shared_ptr<Player> &);
	virtual ~Rocket();

	virtual bool	update();

	bool	checkImpact(float &exploX, float &exploY);

	friend sf::Packet& operator >>(sf::Packet& packet, Rocket &m);

	const std::pair<float, float>	&getImpact(void) const;

protected:
	std::pair<float, float> _impact;
};

sf::Packet& operator >>(sf::Packet& packet, Rocket &m);

