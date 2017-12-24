#include "Game.h"
Window wnd(800, 600, "hello");
Graphics gfx(&wnd);
/*

int main()
{
	{
		
		
		std::cout << "hi";
	}

	return 0;
}
*/
#include <iostream>
#include <cmath>
#include <string>
int imageSize = 600, maxIterations = 255;
double minReal = -1.0, maxReal = 1.0, minImaginary = -1.0, maxImaginary = 1.0;
bool black_white;

inline int findMandelbrot(double cr, double ci);
inline double mapToReal(int x);
inline double mapToImaginary(int y);
void calc();

int main()
{

	while (!glfwWindowShouldClose(wnd.getWindow()))
	{
		gfx.clear(0, 0, 0);


		calc();
		static int i = 0;
		i++;
		if (i > 25)
		{
			std::cout << "hi\n";
		}

		gfx.display();
		glfwPollEvents();
	}

	return 0;
}

inline double mapToReal(int x)
{
	return x * ((maxReal - minReal) / imageSize) + minReal;
}

inline double mapToImaginary(int y)
{
	return y * ((maxImaginary - minImaginary) / imageSize) + minReal;
}

inline int findMandelbrot(double cr, double ci)
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < maxIterations && zr * zr + zi * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}
	return i;
}

void calc()
{
	for (int x = 0; x < imageSize; x++)
	{
		for (int y = 0; y < imageSize; y++)
		{
			double cReal = mapToReal(x);
			double cImaginary = mapToImaginary(y);
			int n = findMandelbrot(cReal, cImaginary);

			gfx.setPixel(x, y, (int)(sin(n)) % 256, (n * n) % 256, n % 256, 255);
		}
	}
}

