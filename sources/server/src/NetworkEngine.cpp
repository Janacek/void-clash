#include	<cstring>
#include	"Manager.hpp"
#include	"Command.hpp"
#include	"NetworkEngine.hpp"
#include	"ClientHandle.hpp"
#include	"Map.hpp"
#include	"main.hpp"
#include	"Defines.h"
#include	"Log.hpp"

///////////////////////////////////////////////
/////   NetworkEngine class

NetworkEngine::NetworkEngine()
{
	_sender = new Sender(this);
	_receiver = new Receiver(this);
	_webSender = new WebSender(this);
}

NetworkEngine::~NetworkEngine()
{
	delete _webSender;
	delete _receiver;
	delete _sender;
}

bool NetworkEngine::start()
{
	int port = BIND_PORT_START;
	while (port != BIND_PORT_END)
	{
		if (_serverSocket.bind(port) == sf::Socket::Done)
		{
			_serverSocket.setBlocking(false);
			if (!_webSender->sendCreate())
				return (false);
			VC_INFO_CRITICAL("Bind done on port " + std::to_string(port));
			return (true);
		}
		VC_WARNING_CRITICAL("Unable to bind port :" + std::to_string(port));
		++port;
	}

	return false;
}

void	NetworkEngine::stop()
{
	_webSender->sendClose();
}

void	NetworkEngine::update()
{
}

void	NetworkEngine::addNewClient(const sf::IpAddress & ip, unsigned short port)
{
	printLog(2, "New client with ip " + ip.toString(), VIOLET);

	ClientHandle	*client = new ClientHandle(this, ip, port);
	_clients.push_back(client);
	_sender->sendPacketEvent(PACKET_VERSION_CHECK, VOID_CLASH_VERSION, client);
	_sender->sendPacketEvent(PACKET_EVENT_IDENT, client->getPlayer()->getId(), client);
	printClientsInfo();
	std::string strToSend = client->getPlayer()->getNick() + " has joined the game";
	_sender->sendStringToDisplay(strToSend, 2);
}

void	NetworkEngine::pingClients()
{
	for (std::list<ClientHandle *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		(*it)->ping();
}

///////////////////////////////////////////////
/////   Tools

// log funcs
void		NetworkEngine::printLog(int level, const std::string &msg, const char *color)
{
	if (level > DEBUG_LEVEL)
		return;

	if (level >= 3)
		VC_INFO(msg);
	else if (level >= 2)
		VC_WARNING(msg);
	else if (level >= 1)
		VC_ERROR(msg);
}

void		NetworkEngine::printLogWithId(int level, const std::string &msg, const sf::Uint32 &id, const char *color)
{
	if (level > DEBUG_LEVEL)
		return;

	if (level >= 3)
		VC_INFO(msg + std::to_string(id));
	else if (level >= 2)
		VC_WARNING(msg + std::to_string(id));
	else if (level >= 1)
		VC_ERROR(msg + std::to_string(id));
}

// find funcs

std::shared_ptr<AObject> NetworkEngine::findObjectWithID(sf::Uint32 id)
{
	std::list<std::shared_ptr<AObject>>			*list = S_Map->getElems();
	std::list<std::shared_ptr<AObject>>::iterator        it = list->begin();
	std::list<std::shared_ptr<AObject>>::iterator        end = list->end();
	while (it != end)
	{
		if ((*it)->getId() == id)
			return (*it);
		++it;
	}
	return NULL;
}

std::shared_ptr<Player>	NetworkEngine::findPlayerWithIP(sf::IpAddress ip, unsigned short port)
{
	auto	it = _clients.begin();
	auto	end = _clients.end();

	while (it != end)
	{
		if ((*it)->getIp().toInteger() == ip.toInteger() && (*it)->getPort() == port)
			return (*it)->getPlayer();
		++it;
	}
	return NULL;
}

ClientHandle	*NetworkEngine::findClientHandleWithIP(sf::IpAddress ip, unsigned short port)
{
	std::list<ClientHandle *>::iterator	it = _clients.begin();
	std::list<ClientHandle *>::iterator	end = _clients.end();

	while (it != end)
	{
		if ((*it)->getIp().toInteger() == ip.toInteger() && (*it)->getPort() == port)
			return (*it);
		++it;
	}
	return NULL;
}


ClientHandle	*NetworkEngine::findClientHandleWithID(sf::Uint32 id)
{
	std::list<ClientHandle *>::iterator	it = _clients.begin();
	std::list<ClientHandle *>::iterator	end = _clients.end();

	while (it != end)
	{
		if ((*it)->getPlayer()->getId() == id)
			return (*it);
		++it;
	}
	return NULL;
}

///////////////////////////////////////////////
/////   Getterz

std::list<ClientHandle *>	&NetworkEngine::getClients()
{
	return (_clients);
}

sf::UdpSocket	*NetworkEngine::getSocket()
{
	return (&_serverSocket);
}

Sender		*NetworkEngine::getSender()
{
	return (_sender);
}

Receiver	*NetworkEngine::getReceiver()
{
	return (_receiver);
}

WebSender	*NetworkEngine::getWebSender()
{
	return (_webSender);
}

///////////////////////////////////////////////
/////   Debug functions

sf::Uint32	NetworkEngine::getNbClient() const
{
	return (_clients.size());
}

void	NetworkEngine::printClientsInfo()
{
	std::list<ClientHandle *>::iterator	it = _clients.begin();
	std::list<ClientHandle *>::iterator	end = _clients.end();

	if (LOG_LEVEL >= 2)
	{
		VC_INFO_CRITICAL(std::to_string(getNbClient()) + " clients are online");
		while (it != end)
		{
			VC_INFO_CRITICAL("ID: " + std::to_string((*it)->getPlayer()->getId()) + "    IP : " + (*it)->getIp().toString());
			++it;
		}
	}
}
