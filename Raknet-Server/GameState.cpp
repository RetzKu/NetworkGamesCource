#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
	//for (Player* playerVar : players)
	//{

	//}
	players.clear();
}

void GameState::AddPlayer(Player player)
{
	if (playersLeft <= playersRight)
	{
		playersLeft += 1;
		player.position = Vec2(-10, 0);
	}
	else
	{
		playersRight += 1;
		player.position = Vec2(10, 0);
	}

	players.push_back(new Player(player));
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

void GameState::UpdatePlayerPos(int guid, Vec2 pos)
{
	Player* result = FindPlayer(guid);
	if (result == nullptr)
	{
		std::cout << "Could not find player with guid " << guid << std::endl;
		return;
	}
	result->position = pos;
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
