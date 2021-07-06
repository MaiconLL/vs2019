#include "olcConsoleGameEngine.h"



class OneLoneCoder_Example : public olcConsoleGameEngine
{
public:
	OneLoneCoder_Example()
	{

	}
protected:
	virtual bool OnUserCreate()
	{
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		
		Draw(10, 10, 50, 50);

		return true;
	}
};


int main()
{
	OneLoneCoder_Example game;


	game.ConstructConsole(160, 80, 8, 8);
	game.Start();
	return 0;
}