#ifndef		BOT_HPP_
# define	BOT_HPP_

#include	<SFML/System.hpp>
#include	"AWeapon.hpp"

class Bot : public AObject
{
public:
	Bot(float X, float Y,
		float dirX, float dirY);
	Bot();
	~Bot();

	bool	update();

	AObject		*lockTarget();
	float		checkDistance(AObject *target);
	void		move();

	bool		checkHitPlayers();
	bool		isInsideHitbox(int X, int Y, float radius);
	void		isHitBy(std::shared_ptr<AWeapon> hitter, float dmg);

	sf::Int16	getLife(void) const;

	friend sf::Packet& operator >>(sf::Packet& packet, Bot &m);

private:
	sf::Clock			_clock;
	AObject				*_target;

	sf::Int16			_life;
};

sf::Packet& operator >>(sf::Packet& packet, Bot &m);

#endif // !BOT_HPP_
