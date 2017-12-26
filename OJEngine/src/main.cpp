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

		if (wnd.getKey(KEY_W).held)
		{
			y -= 250 * dt;
		}
		if (wnd.getKey(KEY_A).held)
		{
			x -= 250 * dt;
		}
		if (wnd.getKey(KEY_S).held)
		{
			y += 250 * dt;
		}
		if (wnd.getKey(KEY_D).held)
		{
			x += 250 * dt;
		}
		gfx.putpixel(x, y, 255,255,255);

		gfx.display();
		wnd.pollEvent();

		dt = clock.restart<float>();
	}

	return 0;
}