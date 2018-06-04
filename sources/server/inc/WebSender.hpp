#ifndef		WEB_SENDER_HPP_
# define	WEB_SENDER_HPP_

#include	"AObject.hpp"

// Send msg to central server every UPDATE_DELAY sec
#define		UPDATE_DELAY	30

// Central Server URL
#define		CENTRAL_SERVER_URL	"http://voidclashserver.azurewebsites.net"
// Central Server API
#define		CENTRAL_SERVER_API	"api/server"

class	NetworkEngine;

enum WebSenderStatus
{
	WEBSENDER_NOT_INIT,
	WEBSENDER_FAILED,
	WEBSENDER_OK
};
class WebSender
{
public:
	WebSender(NetworkEngine *networkEngine);
	~WebSender();

	bool	sendCreate();
	void	sendUpdate();
	bool	sendClose();

private:
	NetworkEngine	*_networkEngine;

	sf::Http *_http;

	// Server ID registered on central server
	int			_id;
	sf::Thread	_thread;

	std::string		_serverName;
	int				_maxPlayer;
	WebSenderStatus	_status;
};

#endif
