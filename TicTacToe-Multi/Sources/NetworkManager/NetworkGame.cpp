#include "pch.h"
#include "NetworkConstants.h"
#include "NetworkGame.h"

NetworkGame::NetworkGame(const std::string& localPlayerName)
	: _isServer(false)
	, _isConnectionEstablish(false)
	, _hasReceivedOpponentName(false)
	, _localPlayerName(localPlayerName)
{

}

NetworkGame::~NetworkGame()
{
	StopPlaying();
}

bool NetworkGame::HostGame()
{
	// À compléter :
	// 1. Configurer le listener TCP sur NetworkPort
	// 2. Ajouter le listener au sélecteur
	// 3. Définir _isServer à true
	_listener.listen(NetworkPort);
	_selector.add(_listener);
	_isServer = true;
	return true;
	return true;
}

bool NetworkGame::WaitingAnOpponent()
{
	// À compléter :
	// 1. Vérifier si une connexion est en attente avec le sélecteur
	// 2. Accepter la connexion
	// 3. Envoyer le nom du joueur local
	// 4. Configurer la connexion
	if(_IsConnectionEstablish == true)
	{
		_selector.Accepter;
		socket.send(localPlayerName);
		
	}
	return true;
}

sf::Socket::Status NetworkGame::WaitingAnOpponentName()
{
	sf::Socket::Status status = TryToReceivePacket(PacketType::PlayerName);

	if(status != sf::Socket::Done)
	{
		return status;
	}

	if(!(_receivedPacket >> _opponentName))
	{
		std::cerr << "Mal formed packet, invalid extract opponentName" << std::endl;
		return sf::Socket::Error;
	}

	if(_opponentName.size() == 0)
	{
		std::cerr << "Mal formed packet, empty opponentName" << std::endl;
		return sf::Socket::Error;
	}

	_hasReceivedOpponentName = true;

	std::cout << "[Packet] Received PlayerName packet with " << _opponentName << " payload" << std::endl;
	return sf::Socket::Done;
}

bool NetworkGame::JoinGame(const sf::IpAddress& serverIP)
{
	std::cout << "Connecting to " << serverIP.toString() << ":" << NetworkPort << std::endl;

	if(_socket.connect(serverIP, NetworkPort) != sf::Socket::Done)
	{
		std::cerr << "Fail to connect to " << serverIP.toString() << ":" << NetworkPort << std::endl;
		return false;
	}

	std::cout << "Connected to " << serverIP.toString() << ":" << NetworkPort << std::endl;

	if(!SendLocalPlayerName())
	{
		return false;
	}

	_selector.add(_socket);
	_isConnectionEstablish = true;
	return true;
}

void NetworkGame::StopPlaying()
{
	std::cout << "NetworkGame reset" << std::endl;

	_selector.remove(_listener);
	_selector.remove(_socket);
	
	_listener.close();
	_socket.disconnect();

	_isServer = false;
	_isConnectionEstablish = false;
	_hasReceivedOpponentName = false;
	_opponentName.clear();
}

bool NetworkGame::SendMove(int row, int col)
{
	// À compléter :
	// 1. Créer un paquet avec MagicPacket et PacketType::Move
	// 2. Ajouter les coordonnées row et col
	// 3. Envoyer le paquet
	return true;
}

sf::Socket::Status NetworkGame::WaitingReceiveMove(int& row, int& col)
{
	sf::Socket::Status status = TryToReceivePacket(PacketType::Move);

	if(status != sf::Socket::Done)
	{
		return status;
	}

	if(!(_receivedPacket >> row >> col))
	{
		std::cerr << "Mal formed packet, invalid extract row and col" << std::endl;
		return sf::Socket::Error;
	}

	if(row < 0 || row >= BoardSize || col < 0 || col >= BoardSize)
	{
		std::cerr << "Mal formed packet, invalid row or col" << std::endl;
		return sf::Socket::Error;
	}

	std::cout << "[Packet] Received a move, row " << row << ", col " << col << std::endl;
	return sf::Socket::Done;
}

bool NetworkGame::IsServer() const
{
	return _isServer;
}

bool NetworkGame::IsConnectionEstablish() const
{
	return _isConnectionEstablish;
}

bool NetworkGame::HasReceivedOpponentName() const
{
	return _hasReceivedOpponentName;
}

const std::string& NetworkGame::GetLocalPlayerName() const
{
	return _localPlayerName;
}

const std::string& NetworkGame::GetOpponentName() const
{
	return _opponentName;
}

bool NetworkGame::SendLocalPlayerName()
{
	sf::Packet packet;
	packet << MagicPacket << std::underlying_type_t<PacketType>(PacketType::PlayerName) << _localPlayerName;
	
	if(_socket.send(packet) != sf::Socket::Done)
	{
		std::cerr << "Fail to send local player name" << std::endl;
		return false;
	}

	std::cout << "[Packet] Send PlayerName packet with " << _localPlayerName << " payload" << std::endl;

	return true;
}

sf::Socket::Status NetworkGame::TryToReceivePacket(PacketType packetTypeExpect)
{
	// À compléter :
	// 1. Vérifier si des données sont disponibles
	// 2. Recevoir le paquet
	// 3. Vérifier le MagicPacket
	// 4. Vérifier le type de paquet
	return sf::Socket::Done;
}

std::string_view PacketTypeToString(PacketType packetType)
{
	switch(packetType)
	{
	case PacketType::None:					return "None"sv;
	case PacketType::PlayerName:			return "PlayerName"sv;
	case PacketType::Move:					return "Move"sv;
	default:								return "Unknown"sv;
	};
}
