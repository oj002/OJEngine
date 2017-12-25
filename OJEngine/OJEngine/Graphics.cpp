#include "Graphics.h"
#include <string>

GLuint CompileShader(unsigned int type, const std::string & sourc);
GLuint CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

Graphics::Graphics(Window *wnd)
	: m_pWnd(wnd)
	, m_imgDataSize(wnd->getWidth() * wnd->getHeight() * 3)
	, m_imgData(new unsigned char[wnd->getWidth() * wnd->getHeight() * 3])
	, m_windowWidth(wnd->getWidth())
	, m_windowHeight(wnd->getHeight())
{

	std::string vertexShader(
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec2 aTexCoord;\n"
		"\n"
		"out vec2 TexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"	TexCoord = aTexCoord;\n"
		"}\n");

	std::string fragmentShader(
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"\n"
		"in vec2 TexCoord;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	FragColor = texture(ourTexture, TexCoord);\n"
		"}\n");

	m_shader = CreateShader(vertexShader, fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions			// texture coords
		1.0f,  1.0f, 0.0f,		1.0f, 0.0f,	// top right
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f,	// bottom right
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// bottom left
		-1.0f,  1.0f, 0.0f,		0.0f, 0.0f	// top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(m_VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// position attribute
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	// texture coord attribute
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));


	// load and create a texture 
	// -------------------------
	GLCall(glGenTextures(1, &m_texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture)); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
													 // set the texture wrapping parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));	// set texture wrapping to GL_REPEAT (default wrapping method)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	// set texture filtering parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// load image, create texture and generate mipmaps

	memset(m_imgData, 0, m_imgDataSize);
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wnd->getWidth(), wnd->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_imgData));


}

Graphics::~Graphics()
{
	delete[] m_imgData;
}

void Graphics::drawLine(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = abs(dx);
	dy1 = abs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		putpixel(x, y, r, g, b);
		for (i = 0; x<xe; i++)
		{
			x = x + 1;
			if (px<0)
				px = px + 2 * dy1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
					y = y + 1;
				else
					y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			putpixel(x, y, r, g, b);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		putpixel(x, y, r, g, b);
		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
					x = x + 1;
				else
					x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			putpixel(x, y, r, g, b);
		}
	}
}
void Graphics::drawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float radius, float scale, unsigned char r, unsigned char g, unsigned char b)
{
	// pair.first = x coordinate
	// pair.second = y coordinate

	// Create translated model vector of coordinate pairs
	std::vector<std::pair<float, float>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

	// Rotate
	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(radius) - vecModelCoordinates[i].second * sinf(radius);
		vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(radius) + vecModelCoordinates[i].second * cosf(radius);
	}

	// Scale
	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * scale;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * scale;
	}

	// Translate
	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
	}

	// Draw Closed Polygon
	for (int i = 0; i < verts + 1; i++)
	{
		int j = (i + 1);
		drawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
			(int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, r, g, b);
	}
}


void Graphics::clear(glm::tvec3<unsigned char> color)
{
	for (unsigned int i = 0; i < m_imgDataSize; i += 3)
	{
		m_imgData[i] = color.r;
		m_imgData[i + 1] = color.g;
		m_imgData[i + 2] = color.b;
	}
}

void Graphics::clear(unsigned char r, unsigned char g, unsigned char b)
{
	for (unsigned int i = 0; i < m_imgDataSize; i += 4)
	{
		m_imgData[i] = r;
		m_imgData[i + 1] = g;
		m_imgData[i + 2] = b;
	}
}

void Graphics::display()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pWnd->getWidth(), m_pWnd->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_imgData));

	GLCall(glUseProgram(m_shader));
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

	glfwSwapBuffers(m_pWnd->getWindow());

}

GLuint CompileShader(unsigned int type, const std::string & sourc)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = sourc.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int lenght;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		char* message = (char*)alloca(lenght * sizeof(char));
		GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
		DEBUG_ERROR("Failed to compile Shader : " << message)
			GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

GLuint CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	GLCall(GLuint program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}
