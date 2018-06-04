#ifndef		AI_HPP_
# define	AI_HPP_

#include	<memory>
#include	<SFML/System.hpp>
#include	"Defines.h"

class	Player;
class	AObject;
	
// Movement
#define	AI_RAYCAST_NB		40					// When no direct detection, do AI_RAYCAST_NB raycasts with the closest target in aim the center
#define AI_RAYCAST_ANGLE	6						// Angles between each raycast
#define AI_INDIRECT_COLLISION_DIST	300				// Distance of the proximity check wall
#define	AI_RANDOM_INDIRECT_COLLISION_DIST	300		// Random factor with the proximity check wall
#define AI_RANDOM_DISTANCE	500						// When raycasting, 
#define	AI_LINEAR_MOVE_FACTOR	2					// If the goal in X is AI_LINEAR_MOVE_FACTOR * in Y, just move in X
#define AI_TARGET_DIRECT_DIST_BONUS 500				// 
#define	AI_UNSTUCK_ANGLE	2.4						// max angle between goal and player dir

// Close range actions
#define AI_TARGET_LOCK_DIST_BONUS 500				// When the AI has a target locked, this define how close another target must be to switch target
#define AI_DISTANCE_TO_SHOOT 3000					// How close a target must be to be attacked by the AI

// Attack
#define	AI_SHOOT_INACCURACY	200						// The range of inaccuracy when AI shoot

class	AI
{
public:
	AI(std::shared_ptr<Player> player);
	~AI(void);

	bool	update(void);

private:
	void	selectWeapons();

	// Get target
	std::shared_ptr<AObject>findTarget(); // Return current target or closest target not behind a wall
	//std::shared_ptr<AObject>findTargetIndirect(); // Return closest target

	// Shoot
	void	setShootInputs();

	// Move
	void	setMoveInputs(float goalX, float goalY);
	void	moveIndirect();
	bool	checkSegmentSegment(const std::shared_ptr<AObject> &nmy);
	bool	checkSegmentSegment(float startX, float startY, float goalX, float goalY);
	bool	CollisionSegSeg(Point A, Point B, Point O, Point P);
	bool	CollisionDroiteSeg(Point A, Point B, Point O, Point P);
	float	checkDistance(const std::shared_ptr<AObject>& target);
	float	checkDistance(float X, float Y);
	bool	checkDistanceAndCollision(const std::shared_ptr<AObject>& obj, float &distance);

private:
	std::shared_ptr<Player>_player;
	std::shared_ptr<AObject>_target;

	// Indirect movement
	std::pair<float, float>	_raycastedPoint;
	std::pair<float, float>	_goal;
	bool	_goToGoal;		// Lock the goal until AI reach a defined proximity with a wall
	bool	_isTooClose;
	float	_indirectDist;	// Distance used by the wall proximity checkl

	// Close move distance variable
	float _dirX;
	float _dirY;
	sf::Time _tooCloseTime;

	float averageX;
	float averageY;
};

#endif
