#include <iostream>
#include "Server.h"

int main()
{
	Server* Connection = new Server("127.0.0.1", 60000);
	bool Running = true;

	bool coolDown = false;
	while (Running)
	{
		Connection->ServerUpdate();
		if (GetConsoleWindow() == GetForegroundWindow())
		{
			if (GetAsyncKeyState(VK_ESCAPE) == -32767)
			{
				Running = false;
			}
			if (GetAsyncKeyState(0x41) == -32767 && coolDown == false)
			{
				//Connection->AskForVariable(PLAYER_COORD, "retzku");
				Connection->RequestFromAll(PLAYER_COORD);
				coolDown = true;
			}
			else 
			{
				coolDown = false;
			}
		}
	}
	delete Connection;
	return 0;
}