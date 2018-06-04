#ifndef		CAPTURE_HPP_
# define	CAPTURE_HPP_

#include	"AObject.hpp"

#define	TIME_TO_CAPTURE		200
#define	CONTROL_MAX		300
#define	TIME_TO_INC_POINT	5000	

class Capture : public AObject
{
public:

	Capture(float X, float Y, float dirX, float dirY, float width, float height);
	Capture();
	~Capture();

	virtual bool	update();

	void	reset();

	// Return 0 if no one is controling the zone
	// else 1 or 2 for the team controling
	sf::Uint16	getTeamControlling() const;
	// Return the control state of the capture zone
	// Thresholds are: -CONTROL_MAX <= team 2 control <= -TIME_TO_CAPTURE <= neutral <= TIME_TO_CAPTURE <= team 1 control <= CONTROL_MAX
	float			getControledStatus() const;

	friend sf::Packet& operator >>(sf::Packet& packet, Capture &m);


private:

	sf::Uint16	_teamControled;
	float	_controlBar;

	// Funcs
	bool	isInsideCapture(float X, float Y);
	void	handleCaptureControl();
	void	increaseTeamScore();
};

sf::Packet& operator >>(sf::Packet& packet, Capture &m);

#endif		// !CAPTURE_HPP_
