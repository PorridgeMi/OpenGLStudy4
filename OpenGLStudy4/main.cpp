import std;
import OpenGLStudy.Application;
import OpenGLStudy.Shader;
import OpenGLStudy.VertexArray;
import OpenGLStudy.Callback;

#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace OpenGLStudy;

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

GLint requireAttribLocation(const Shader& shader, std::string_view name)
{
	GLint location = shader.getAttribLocation(std::string(name));
	if (location == -1)
	{
		throw std::runtime_error(std::string("Vertex attribute not found in shader: ") + std::string(name));
	}
	return location;
}

void prepareSingleBuffer(const VertexArray& vao, const Shader& shader)
{
	GLint posLocation = requireAttribLocation(shader, "aPos");
	GLint colorLocation = requireAttribLocation(shader, "aColor");

	std::array positions{
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};
	std::array colors{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
	};
	std::array<GLuint, 6> indices{
		0, 1, 2,
		2, 3, 0
	};
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);

	vao.bind();
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	VertexArray::unbind();

	glDeleteBuffers(1, &posVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &ebo);
}

void prepareInterleavedBuffer(const VertexArray& vao, const Shader& shader)
{
	GLint posLocation = requireAttribLocation(shader, "aPos");
	GLint colorLocation = requireAttribLocation(shader, "aColor");

	std::array vertices{
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
	};

	std::array<GLuint, 6> indices{
		0, 1, 2,
		2, 3, 0
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	vao.bind();

	constexpr GLsizei stride = 6 * sizeof(float);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	VertexArray::unbind();

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

Shader prepareShader()
{
	return Shader("Shaders/rectangle.vert", "Shaders/rectangle.frag");
}

void render(const VertexArray& vao, const Shader& shader)
{
	glClear(GL_COLOR_BUFFER_BIT);
	shader.begin();
	vao.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	VertexArray::unbind();
	shader.end();
}

int main()
{
	Application* app = Application::getInstance();
	if (!app->init(windowWidth,windowHeight,"MyWindow"))
	{
		std::print("Failed to initialize Application");
		app->destroy();
		return -1;
	}
	app->setResizeCallback(OnResize);
	app->setKeyCallback(OnKey);
	app->setMouseButtonCallback(OnMouseButton);
	app->setCursorPosCallback(OnCursorPos);
	app->setScrollCallback(OnScroll);
	glViewport(0, 0, app->getWidth(), app->getHeight());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	{
		VertexArray vao;

		try
		{
			Shader shader = prepareShader();

			prepareSingleBuffer(vao, shader);

			while (app->update())
			{
				render(vao, shader);
			}
		}
		catch (const std::exception& e)
		{
			std::print("Failed to prepare shader, rendering skipped:\n{}\n", e.what());
		}
	}

	app->destroy();
}
