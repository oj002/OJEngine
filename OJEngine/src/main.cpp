#include "../OJEngine/core.h"
#include "timer.h"

Window wnd(800, 600, "hello");
Graphics gfx(&wnd);

int main()
{

	float x = 0, y = 0;
	
	timer clock;
	float dt = clock.restart<float>();
	while (!wnd.closed())
	{
		gfx.clear();

		if (wnd.isKeyPressed(KEY_W))
		{
			y -= 250 * dt;
		}
		if (wnd.isKeyPressed(KEY_A))
		{
			x -= 250 * dt;
		}
		if (wnd.isKeyPressed(KEY_S))
		{
			y += 250 * dt;
		}
		if (wnd.isKeyPressed(KEY_D))
		{
			x += 250 * dt;
		}
		gfx.setPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), 255, 0, 0);

		gfx.display();
		wnd.pollEvent();

		dt = clock.restart<float>();
	}

	return 0;
}