#include	<sstream>
#include	"WebSender.hpp"
#include	"NetworkEngine.hpp"
#include	"Map.hpp"
#include	"ConfigParser.hpp"
#include	"Log.hpp"

extern t_config	*G_conf;
WebSender::WebSender(NetworkEngine *networkEngine) :
	_networkEngine(networkEngine),
	_thread(&WebSender::sendUpdate, this)
{
	_http = new sf::Http;
	_http->setHost(CENTRAL_SERVER_URL);
	_thread.launch();
	_serverName = G_conf->server->name.c_str();
	_maxPlayer = G_conf->server->max_player;
	_status = WEBSENDER_NOT_INIT;
}

WebSender::~WebSender()
{
	_thread.terminate();
	delete _http;
}

// Post request
bool	WebSender::sendCreate()
{
	//http://voidclash.azurewebsites.net/Help
	//Id = 3 & Name = zaza&CurrentMode = CaptureTheFlag&PlayerNumber = 12 & CurrentMap = De_dust2

	// préparation de la requête
	sf::Http::Request request(CENTRAL_SERVER_API, sf::Http::Request::Post);

	// encodage des paramètres dans le corps de la requête
	std::ostringstream stream;
	stream
		<< "&Name=" << _serverName
		<< "&CurrentMap=" << S_Map->getMapDatabase()->getCurrentMapName().c_str()
		<< "&PlayerNumber=" << S_Map->getPlayers()->size()
		<< "&MaxPlayer=" << _maxPlayer
		<< "&Port=" << _networkEngine->getSocket()->getLocalPort()
		<< "&CurrentMode=" << S_Map->getMode()->getModeName().c_str();
	request.setBody(stream.str());

	// envoi de la requête au serveur
	sf::Http::Response response = _http->sendRequest(request, sf::seconds(2));

	// vérification du statut
	if (response.getStatus() == sf::Http::Response::Ok || response.getStatus() == sf::Http::Response::Created)
	{
		VC_INFO_CRITICAL("Server CREATION has been successfully sent to central server");

		// Get ID
		std::string str = response.getBody();
		std::size_t pos = str.find("Id\":") + 4;
		std::size_t end = str.find(",");
		std::size_t len = end - pos;
		std::string tmp = str.substr(pos, len);
		_id = atoi(tmp.c_str());
		_status = WEBSENDER_OK;
		return true;
	}
	else
	{
		VC_WARNING_CRITICAL("Unable to send server CREATION to central server");
		_status = WEBSENDER_FAILED;
	}
	return false;
}

// Put request
void	WebSender::sendUpdate()
{
	sf::sleep(sf::seconds(UPDATE_DELAY));
	//http://voidclash.azurewebsites.net/Help
	//Id = 3 & Name = zaza&CurrentMode = CaptureTheFlag&PlayerNumber = 12 & CurrentMap = De_dust2

	// préparation de la requête
	while (42)
	{
		if (_status == WEBSENDER_OK)
		{
			sf::Http::Request request(CENTRAL_SERVER_API, sf::Http::Request::Put);
			request.setField("Content-Type", "application/x-www-form-urlencoded");

			// encodage des paramètres dans le corps de la requête
			std::ostringstream stream;
			stream << "Id=" << _id
				<< "&Name=" << _serverName
				<< "&CurrentMap=" << S_Map->getMapDatabase()->getCurrentMapName().c_str()
				<< "&PlayerNumber=" << S_Map->getPlayers()->size()
				<< "&MaxPlayer=" << _maxPlayer
				<< "&Port=" << _networkEngine->getSocket()->getLocalPort()
				<< "&CurrentMode=" << S_Map->getMode()->getModeName().c_str();
			request.setBody(stream.str());

			// envoi de la requête au serveur
			sf::Http::Response response = _http->sendRequest(request, sf::seconds(2));

			// vérification du statut
			sf::Http::Response::Status status = response.getStatus();
			if (status == sf::Http::Response::Ok || status == sf::Http::Response::Created || status == sf::Http::Response::Accepted)
			{
				VC_INFO_CRITICAL("Server UPDATE has been successfully sent to central server");
			}
			else
			{
				VC_WARNING_CRITICAL("Unable to send server UPDATE to central server");
				sendCreate();
			}
		}
		else
			sendCreate();
		sf::sleep(sf::seconds(UPDATE_DELAY));
	}
}

// Delete request
bool	WebSender::sendClose()
{
	//http://voidclash.azurewebsites.net/Help
	//Id = 3 & Name = zaza&CurrentMode = CaptureTheFlag&PlayerNumber = 12 & CurrentMap = De_dust2

	// préparation de la requête
	sf::Http::Request request(CENTRAL_SERVER_API, sf::Http::Request::Delete);

	// encodage des paramètres dans le corps de la requête
	std::ostringstream stream;
	stream << "Id=" << _id;
	request.setBody(stream.str());

	// envoi de la requête au serveur
	sf::Http::Response response = _http->sendRequest(request, sf::seconds(2));

	// vérification du statut
	if (response.getStatus() == sf::Http::Response::Ok || response.getStatus() == sf::Http::Response::Created)
	{
		VC_INFO_CRITICAL("Server CLOSE has been successfully sent to central server");
		return true;
	}
	else
		VC_WARNING_CRITICAL("Unable to send server CLOSE to central server");
	
	return false;
}

