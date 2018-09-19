#include "Client.h"

#include <vector>
#define CONSOLE(x) std::cout << x;
#define CONSOLEL(x) std::cout << x << std::endl; 

bool Running = true;


struct WASD
{
	bool A = false; bool S = false; bool D = false; bool W =false;
};

void KeyStateThread(WASD* input);

//TODO: test #pragma pack(push,1). #pragma pack(pop) metodia structien lähettämisen kanssa.
int main()
{
	Client* Connection = new Client("127.0.0.1", 60000,"Loyalisti");
	Connection->OpenConnection();

	WASD* input = new WASD();
	std::thread(KeyStateThread,input).detach();

	int x = 1;
	int y = 2;
	std::vector<int*> cord;
	cord.push_back(&x);
	cord.push_back(&y);
	Connection->SetVar(PLAYER_COORD, cord);

	while (Running)
	{
		Connection->Update();
	}
	Connection->CloseConnection();
	delete Connection;
	delete input;
	Running = 0;
	return 0;
}

void KeyStateThread(WASD* input)
{
	while (Running)
	{
		if (GetAsyncKeyState(0x41) != 0) { input->A = true; }
		else { input->A = false; }

		if (GetAsyncKeyState(0x44) != 0) { input->D = true; }
		else { input->D = false; }

		if (GetAsyncKeyState(0x57) != 0) { input->W = true; }
		else { input->W = false; }

		if (GetAsyncKeyState(0x53) != 0) { input->S = true; }
		else { input->S = false; }
		this_thread::sleep_for(0.01s);
	}
}
