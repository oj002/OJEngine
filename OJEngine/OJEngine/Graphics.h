#pragma once
#include "Macros.h"
#include "Window.h"
#include <vector>

class Graphics
{
public:
	Graphics(Window *wnd);
	~Graphics();

	inline void putpixel(unsigned int x, unsigned int y, glm::tvec3<unsigned char> color)
	{
		if (x >= 0 && x < m_windowWidth && y >= 0 && y < m_windowHeight)
		{
			unsigned char* p = m_imgData;
			p += y * m_windowWidth * 3 + x * 3;
			*p = color.r;
			++p;
			*p = color.g;
			++p;
			*p = color.b;
		}
	}

	inline void putpixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
	{
		if (x >= 0 && x < m_windowWidth && y >= 0 && y < m_windowHeight)
		{
			unsigned char* p = m_imgData;
			p += y * m_windowWidth * 3 + x * 3;
			*p = r;
			++p;
			*p = g;
			++p;
			*p = b;
		}
	}

	inline glm::tvec3<unsigned char> getPixel(unsigned int x, unsigned int y)
	{
		if (x >= 0 && x < m_windowWidth && y >= 0 && y < m_windowHeight)
		{
			unsigned int pixelPos = y * m_windowWidth * 3 + x * 3;
			return {
				m_imgData[pixelPos],
				m_imgData[pixelPos + 1],
				m_imgData[pixelPos + 2]
			};
		}
		else
		{
			DEBUG_ERROR("pixel not in window");
		}
	}

	void drawLine(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b);
	void Graphics::drawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float radius, float scale, unsigned char r, unsigned char g, unsigned char b);

	inline void clear(unsigned char grayLevel = 0) { memset(m_imgData, grayLevel, m_imgDataSize); }
	void clear(glm::tvec3<unsigned char> color);
	void clear(unsigned char r, unsigned char g, unsigned char b);
	void display();

private:
	Window *m_pWnd;
	unsigned int m_VAO;
	unsigned int m_texture;
	unsigned int m_shader;
	const unsigned int m_imgDataSize;
	unsigned char* m_imgData;

	int m_windowWidth, m_windowHeight;
};