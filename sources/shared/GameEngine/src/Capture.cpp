#include	"Capture.hpp"
#include	"Map.hpp"

Capture::Capture() :
AObject(CAPTURE, 0, 0, 0, 0)
{
	_width = 0;
	_height = 0;
	_teamControled = 0;
	_controlBar = 0.f;
}

Capture::Capture(float X, float Y,
	float dirX, float dirY,
	float width,
	float height) :
	AObject(CAPTURE, X, Y, dirX, dirY)
{
	_width = width;
	_height = height;
	_teamControled = 0;
	_controlBar = 0.f;
}

Capture::~Capture()
{}

bool	Capture::update()
{
	if (!S_Map->getMode()->getProperty()->capture)
		return true;

	handleCaptureControl();
	increaseTeamScore();
	return true;
}

void	Capture::reset()
{
	_teamControled = 0;
	_controlBar = 0.f;
}

void	Capture::increaseTeamScore()
{
	if (!_teamControled)
		return;

	if (_teamControled == 1)
		S_Map->incScore(_deltaTime.asMilliseconds() * 0.001f, 0);
	else
		S_Map->incScore(0, _deltaTime.asMilliseconds() * 0.001f);
}

// Check if player is in zone:
// If player of same team -> inc time - If time > value -> inc score
// If player of opposite team -> switch
// If no player -> 0
void	Capture::handleCaptureControl()
{
	// Determine which team is in zone
	std::shared_ptr<Player> one = NULL;
	std::shared_ptr<Player> two = NULL;
	auto	it = S_Map->getPlayers()->begin();
	auto	end = S_Map->getPlayers()->end();
	int nbOfPlayerInZone = 0;
	while (it != end)
	{
		if (isInsideCapture((*it)->getX(), (*it)->getY()))
		{
			if ((*it)->getTeam() == 1)
			{
				one = *it;
				nbOfPlayerInZone++;
			}
			if ((*it)->getTeam() == 2)
			{
				two = *it;
				nbOfPlayerInZone++;
			}
		}
		++it;
	}

	// Apply control
	if (_controlBar >= TIME_TO_CAPTURE)
	{
		// Send capture event
		if (_teamControled != 1)
			ADD_EVENT(ev_ZONE_CAPTURED, s_event(shared_from_this()));
		_teamControled = 1;
	}
	else if (_controlBar <= -TIME_TO_CAPTURE)
	{
		// Send capture event
		if (_teamControled != 2)
			ADD_EVENT(ev_ZONE_CAPTURED, s_event(shared_from_this()));
		_teamControled = 2;
	}
	else
	{
		// Send capture event
		if (_teamControled != 0)
		{
			int *curTeamCapturing = new int;
			// Green team is capturing
			if (one != NULL && two == NULL)
				*curTeamCapturing = 1;
			// Red team is capturing
			else if (one == NULL && two != NULL)
				*curTeamCapturing = 2;
			ADD_EVENT(ev_ZONE_CAPTURED, s_event(shared_from_this(), curTeamCapturing));
		}
		_teamControled = 0;
	}

	// Cap max
	if (_controlBar > CONTROL_MAX)
		_controlBar = CONTROL_MAX;
	if (_controlBar < -CONTROL_MAX)
		_controlBar = -CONTROL_MAX;

	// Apply clock actions
	if (one && two)
		return;
	if (!one && !two)
		return;

	if (one)
		_controlBar += _coefDeltaTime * nbOfPlayerInZone;
	if (two)
		_controlBar -= _coefDeltaTime * nbOfPlayerInZone;
}

bool	Capture::isInsideCapture(float X, float Y)
{
	if (X > _pos.first && X < _pos.first + _width &&
		Y > _pos.second && Y < _pos.second + _height)
		return (true);
	return (false);
}

// Return the control state of the capture zone
// Thresholds are: -CONTROL_MAX <= team 2 control <= -TIME_TO_CAPTURE <= neutral <= TIME_TO_CAPTURE <= team 1 control <= CONTROL_MAX
float	Capture::getControledStatus() const
{
	return _controlBar;
}

// Return 0 if no one is controling the zone
// else 1 or 2 for the team controling
sf::Uint16	Capture::getTeamControlling() const
{
	return _teamControled;
}