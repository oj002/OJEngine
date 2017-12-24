#pragma once
#include "../OJEngine/Graphics.h"
#include "../OJEngine/Window.h"

class Game
{
public:
	Game();
	~Game();

	void run();

	bool update();
	bool render();

private:
	Window wnd;
	Graphics gfx;
};