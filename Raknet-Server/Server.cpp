#include "Server.h"

using namespace RakNet;
using namespace std;

#define PORT 60000

Server::Server(string IP, int Port)
{
	string title = "Raknet-Server";
	SetConsoleTitle(title.c_str());
	this->IP = IP;
	this->Port = Port;
	Connections = new UserDatabase();
	ServerStart();
}

Server::~Server()
{
}

void Server::ServerStart()
{
	Peer = RakPeerInterface::GetInstance();
	serverLogic = GameState();
	SD = new SocketDescriptor(PORT, 0);
	/*Creating Instance for server and starting up basic setup*/
	Peer->Startup(MaxConnections, SD, 1);
	Peer->SetMaximumIncomingConnections(MaxConnections);
	CONSOLE("Starting server at port " << Port);

	Delta120 = chrono::system_clock::now();
	TimeInterval = (int)((1.0 / 60) * 1000);

	running = true;
}

void Server::ServerStop()
{
	running = false;
	RakPeerInterface::DestroyInstance(Peer);
	delete SD;
	delete Connections;
}

void Server::ServerUpdate()
{
	auto Delta = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - Delta120);
	/*Loads packet from peer*/
	if ((float)Delta.count() > TimeInterval)
	{
		Delta120 += chrono::milliseconds((int)TimeInterval);
		RequestFromAll(PLAYER_INPUT);
		serverLogic.Tick();
		BroadcastBallPosition();
		for (Packet = Peer->Receive(); Packet; Packet = Peer->Receive())
		{
			CheckPacket(*Packet);
			Peer->DeallocatePacket(Packet);
		}
	}
}

void Server::CheckPacket(const RakNet::Packet& P)
{
	switch (P.data[0])
	{
	case ID_NEW_INCOMING_CONNECTION:
		Connections->ConnectUser(Packet);
		CONSOLE(Packet->guid.ToString() << " Connected to server");
		break;
	case USERNAME_FOR_GUID:
		Result = Connections->RegisterGuid(Packet);
		if (Result == "RECONNECT") { SendResponse(Packet->systemAddress, LOGIN_ACCEPTED); CONSOLE("ID: " << Packet->guid.ToString() << " reconnected"); break; }
		if (Result == "NONE") { SendResponse(Packet->systemAddress, LOGIN_FAILED); CONSOLE("ID: " << Packet->guid.ToString() << " failed to give username"); break; }
		else 
		{
			SendResponse(Packet->systemAddress, LOGIN_ACCEPTED);
			CONSOLE(Packet->guid.ToString() << " gave an username " << Result);
			serverLogic.AddPlayer(Packet->guid.ToUint32(Packet->guid),Result.c_str());
		}
		break;
	case ID_CONNECTION_LOST:
		Connections->RemoveUser(Packet);
		CONSOLE(Packet->guid.ToString() << " Connection lost");
		break;
	case PLAYER_COORD:
		ReadPlayerCoord(Packet);
		break;
	case PLAYER_INPUT:
		ReadPlayerInput(Packet);
		break;
	}
}

void Server::BroadcastBallPosition()
{
	string guid = Connections->FindGuid("Loyalisti");
	RakNet::RakNetGUID rakguid;
	rakguid.FromString(guid.c_str());

	RakNet::BitStream bs;
	Vec2 ballPos = serverLogic.GetBallPos();
	bs.Write((RakNet::MessageID)BALL_UPDATE);
	bs.Write(ballPos.x);
	bs.Write(ballPos.y);

	bool result = Peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS ,true);
}

void Server::BroadcastVar(CustomMessages Var, RakNet::Packet Packet)
{
	RakNet::BitStream bs(Packet.data,Packet.length,false);
	Peer->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, Packet.systemAddress, true, 0);
}

void Server::ReadPlayerCoord(RakNet::Packet* packet)
{
	RakNet::BitStream bs(packet->data,packet->length,false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int x;
	int y;

	bs.Read(x);
	bs.Read(y);
	std::string user = Connections->FindUsername(packet->guid.ToString());
	if (user != "NONE")
	{
		CONSOLE("Received " << x << ", " << y << " From: " << user);
	}
}

void Server::ReadPlayerInput(RakNet::Packet* packet)
{
	RakNet::BitStream bs(packet->data, packet->length, false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	int x = 0;
	int y = 0;
	bs.Read(x);
	bs.Read(y);

	//serverLogic.ProcessInput(packet->guid.ToUint32(packet->guid), {x,y});
}

void Server::SendResponse(RakNet::SystemAddress sys, CustomMessages responseID)
{
	RakNet::BitStream bs;
	bs.Write((MessageID)responseID);
	Peer->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, sys, false, 0);
}

bool Server::AskForVariable(CustomMessages var, INT64 guid)
{
	RakNet::BitStream bs;
	RakNet::RakNetGUID rakguid;
	rakguid.FromString(to_string(guid).c_str());

	bs.Write((RakNet::MessageID)var);
	Peer->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, rakguid, false, 0);
	return true;
}

bool Server::AskForVariable(CustomMessages var, string username)
{
	string guid = Connections->FindGuid(username);
	if (guid == "NONE")
	{
		return false;
	}
	INT64 guidint = stoll(guid);
	return AskForVariable(var, guidint);
}

void Server::RequestFromAll(CustomMessages Requested)
{
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)Requested);
	Peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0);
}

