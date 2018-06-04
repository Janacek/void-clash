//
// PhysicEngine.hpp for pfa in /home/volent/dev/pfa/client
// 
// Made by florent espanet
// Login   <espane_f@epitech.net>
// 
// Started on  Thu Oct  3 14:28:34 2013 florent espanet
// Last update Tue Feb 25 17:16:41 2014 Gegon
//

#ifndef		__PHYSICENGINE_HPP__
# define	__PHYSICENGINE_HPP__

#include	<SFML/System.hpp>
#include	<list>
#include	"AEngine.hpp"
#include	"AObject.hpp"
#include	"Wall.hpp"
#include	"Defines.h"

class		Manager;

// When a collision is detected, send a t_impact struct with event ev_WALL_COLLISION
// Need to be deleted by graphic !
typedef	struct	s_impact
{
	std::shared_ptr<Wall>	wall;
	std::pair<float, float>pos;
}				t_impact;

#define OPTI_DISTANCE_MARGIN	1000;

class		PhysicEngine : public AEngine
{
public:
	PhysicEngine();
	virtual ~PhysicEngine(void);

	virtual void start(void);
	virtual void stop(void);
	virtual eGameState update(const sf::Time &deltaTime);

	void	updateObject(const std::shared_ptr<AObject> &obj);
	bool	initUpdatePhysObject(const std::shared_ptr<AObject> &obj);
	bool	updatePhysObject(const std::shared_ptr<AObject> &obj, int level);

	void	setDelta(const float &delta);
	const float &getDelta();

	bool	detectCollision(const std::shared_ptr<AObject> &obj);

	// Apply bounce effect
	void	bounce(const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall);
	Vecteur GetNormale(Point A, Point B, int len);
	Vecteur CalculerVecteurV2(Vecteur v, Vecteur N);

	// Segment circle
	bool	checkSegmentCircleCurrentPos(const std::shared_ptr<AObject> &obj);
	bool	checkSegmentCircle(const std::shared_ptr<AObject> &obj);
	bool	CollisionSegment(Point A, Point B, Cercle C, const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall);
	bool	CollisionDroite(Point A, Point B, Cercle C);
	bool	CollisionPointCercle(Point A, Cercle C);

	// Segment segment
	bool	checkSegmentSegment(const std::shared_ptr<AObject> &obj);
	bool	CollisionSegSeg(Point O, Point P, const std::shared_ptr<AObject> &obj, const std::shared_ptr<Wall> &wall);

	Point ProjectionI(Point A, Point B, Point C);

private:
	void	regenerateOptiWalls(const std::shared_ptr<AObject> &obj);
	void	checkPhysic(const std::shared_ptr<AObject> &obj);
	bool	initCheckBounce(const std::shared_ptr<AObject> &obj);

private:


	float _delta;
	std::list<std::shared_ptr<Wall>>	_optiWalls;
};

// This function is used for network simulation only
void	simulateUpdatePhysObject(const std::shared_ptr<AObject> &obj);

#endif		/* !__PHYSICENGINE_HPP__ */
