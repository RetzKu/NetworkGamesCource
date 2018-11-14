#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <random>

struct Vec2
{
	Vec2(int x, int y) { this->x = x; this->y = y; }
	Vec2() {};
	int x;
	int y;
};

struct Vec3
{
	int x;
	int y;
	int z;

	static Vec3 randomize() 
	{
		Vec3 tmp;
		tmp.x = std::rand() % 100;
		tmp.y = std::rand() % 100;
		tmp.z = std::rand() % 100;
		return tmp;
	}
};

struct Player
{
	Player(const char* name, int guid) { this->name = name; this->guid = guid; this->color = Vec3::randomize(); }
	std::string name;
	int guid;
	Vec2 position;
	Vec3 color;
};

class GameState
{
	/*Public Functions*/
public:
	GameState();
	~GameState();

	void AddPlayer(Player player);
	void RemovePlayer(int guid);

	void UpdatePlayerPos(int guid, Vec2 pos);

	/*Private Functions*/
private:
	Player* FindPlayer(int guid);
	
	/*Public Variables*/
public:

	/*Private Variables*/
private:	
	int playersLeft = 0;
	int playersRight = 0;
	std::vector<Player*> players;
};

