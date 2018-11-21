#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
	players.clear();
}

void GameState::AddPlayer(int guid,const char* name)
{
	players.push_back(new Player(name,guid));
}


void GameState::RemovePlayer(int guid)
{
	Player* result = FindPlayer(guid);
	if (result == nullptr)
	{
		std::cout << "Could not find player with guid " << guid << std::endl;
		return;
	}
	std::cout << "Removing player: " << result->name << std::endl;

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] == result)
		{
			players.erase(players.begin() + i);
		}
	}
}


void GameState::ProcessInput(int guid, WASD input) //Wasd is replaceable
{
	
}

void GameState::UpdatePlayerPos(int guid, Vec2 pos)
{
	Player* result = FindPlayer(guid);
	if (result == nullptr)
	{
		std::cout << "Could not find player with guid " << guid << std::endl;
		return;
	}
	result->position.y = pos.y;
}


Player* GameState::FindPlayer(int guid)
{
	for (Player* playerVar : players)
	{
		playerVar->guid == guid;
		return playerVar;
	}
	return nullptr;
}
