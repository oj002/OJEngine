#pragma once
#include "Macros.h"
#include "Window.h"

class Graphics
{
public:
	Graphics(Window *wnd);
	~Graphics();

	inline void setPixel(glm::tvec2<unsigned int> pos, glm::tvec3<unsigned char> color) 
	{
		unsigned int pixelPos = (std::abs((int)pos.y - m_pWnd->getHeight() + 1)) * m_pWnd->getWidth() * 3 + (int)pos.x * 3;
		m_imgData[pixelPos    ] = color.r; 
		m_imgData[pixelPos + 1] = color.g;
		m_imgData[pixelPos + 2] = color.b;
	}

	inline void setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
	{
		unsigned int pixelPos = (std::abs((int)y - m_pWnd->getHeight() + 1)) * m_pWnd->getWidth() * 3 + (int)x * 3;
		m_imgData[pixelPos    ] = r;
		m_imgData[pixelPos + 1] = g;
		m_imgData[pixelPos + 2] = b;
	}

	inline glm::tvec3<unsigned char> getPixel(glm::tvec2<unsigned int> pos)
	{
		unsigned int pixelPos = (std::abs((int)pos.y - m_pWnd->getHeight() + 1)) * m_pWnd->getWidth() * 3 + (int)pos.x * 3;
		return {
			m_imgData[pixelPos],
			m_imgData[pixelPos + 1],
			m_imgData[pixelPos + 2]
		};
	}

	inline glm::tvec3<unsigned char> getPixel(unsigned int x, unsigned int y)
	{
		unsigned int pixelPos = (std::abs((int)y - m_pWnd->getHeight() + 1)) * m_pWnd->getWidth() * 3 + (int)x * 3;
		return {
			m_imgData[pixelPos],
			m_imgData[pixelPos + 1],
			m_imgData[pixelPos + 2]
		};
	}

	inline void clear(unsigned char grayLevel) { memset(m_imgData, grayLevel, m_imgDataSize); }
	void clear(glm::tvec3<unsigned char> color = { 0, 0, 0 });
	void clear(unsigned char r, unsigned char g, unsigned char b);
	void display();

private:
	Window *m_pWnd;
	unsigned int m_VAO;
	unsigned int m_texture;
	unsigned int m_shader;
	const unsigned int m_imgDataSize;
	unsigned char* m_imgData;
};