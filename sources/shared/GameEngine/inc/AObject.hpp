//
// AObject.hpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Wed Jul 31 22:51:27 2013 brunel guillaume
// Last update Mon Jan 20 23:16:56 2014 Gegon
//

#ifndef		AOBJECT_HPP_
# define	AOBJECT_HPP_

#include	<SFML/Network.hpp>
#include	<memory>
#include	"ConfigParser.hpp"

class Player;
class GameEngine;

enum	eObjectType
{
	NONE = 0,
	WALL,
	PLAYER,
	BOT,

	BULLET,
	BOMB,
	ROCKET,
	TURRET,
	SHIELD,

	EXPLOSION,
	GRAVITY_FIELD,
	SPEED_FIELD,
	RESPAWN,
	FLAG,
	CAPTURE
};

///////////////////////////////////////////////
/////	Parent class of all objects including entities

class AObject : public std::enable_shared_from_this<AObject>
{
public:
	explicit AObject(void);
	explicit AObject(eObjectType type, float X, float Y, float dirX, float dirY);
	AObject(std::shared_ptr<AObject>) { };
	virtual ~AObject(void);
	void	damage(float posX, float posY, float endX, float endY, float width, float height, float team);

	// Update object attribute
	virtual void  setPosition(float, float);
	virtual void	setPrevFramePosition(float X, float Y);
	virtual void  setDirX(float);
	virtual void  setDirY(float);

	// Update and init
	virtual bool	update(void);
	virtual void	updateAObject(const sf::Time &deltaTime);

	// Add force in X and Y
	void addForce(float dirX, float dirY);
	void divideForceBy(float nb);
	void invertForce(void);

	// Getters
	int getRadius() const;
	const float& getX() const;
	const float& getY() const;
	const float& getDirX() const;
	const float& getDirY() const;
	const sf::Uint32& getId() const;
	const eObjectType& getType() const;
	const std::pair<float, float>& getPosition() const;
	const std::pair<float, float>& getPrevFramePosition() const;
	const std::pair<float, float>& getDirection(void) const;
	const std::pair<float, float>& getEnd() const;
	const float&		getEndX() const;
	const float&		getEndY() const;
	int	getSlow() const;
	sf::Int32	getWidth() const;
	sf::Int32	getHeight() const;
	
	// Map editor
	bool isSelected(void) { return _selected; }
	void setSelected(bool selected) { _selected = selected; }

	// Team
	void		setTeam(sf::Uint16 team);
	sf::Uint16	getTeam() const;

	// Owner
	std::shared_ptr<Player> getOwner(void) const;
	void	setOwner(std::shared_ptr<Player>p);

	// Setters
	void setId(sf::Uint32 id);
	const float&	getTimeSinceCreation() const;
	void	applySlow(t_weapon *weapon);

	// MANDATORY to call this function to push AObject in MAP
	void pushInMap(void);

protected:
	std::shared_ptr<Player>	_owner;
	int _radius;
	eObjectType _type;
	std::pair<float, float> _pos;
	std::pair<float, float> _prevFramePos;
	std::pair<float, float> _end;
	std::pair<float, float> _dir;
	sf::Int32				_width;
	sf::Int32				_height;
	sf::Uint16				_team;

	sf::Time				_deltaTime;
	float					_coefDeltaTime;
	float					_timeSinceCreation;

	int						_slow;
	float					_slowDuration;
	float					_slowStartTime;

	sf::Uint32 _id;

	// Map editor
	bool _selected;
};

#endif		/* !AOBJECT_HPP_ */
