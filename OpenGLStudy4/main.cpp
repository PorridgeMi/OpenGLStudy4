import std;
import OpenGLStudy.Application;
import OpenGLStudy.Shader;
import OpenGLStudy.VertexArray;
import OpenGLStudy.Callback;
import OpenGLStudy.Transform;
import OpenGLStudy.Texture;

#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace OpenGLStudy;

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
const std::string windowName{ "MyWindow" };

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
	GLint texCoordLocation = requireAttribLocation(shader, "aTexCoord");

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
	std::array texCoords{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};
	std::array<GLuint, 6> indices{
		0, 1, 2,
		2, 3, 0
	};
	GLuint posVbo, colorVbo, texCoordVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &texCoordVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);

	vao.bind();
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
	glEnableVertexAttribArray(texCoordLocation);
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	VertexArray::unbind();

	glDeleteBuffers(1, &posVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &texCoordVbo);
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

void render(const VertexArray& vao, const Shader& shader, const Texture& texture, GLint modelLocation, GLint timeLocation)
{
	glClear(GL_COLOR_BUFFER_BIT);
	shader.begin();

	UploadModelRotationY(modelLocation, (float)glfwGetTime());
	glUniform1f(timeLocation, (float)glfwGetTime());

	texture.bind();
	vao.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	VertexArray::unbind();
	Texture::unbind();
	shader.end();
}

int main()
{
	Application* app = Application::getInstance();
	if (!app->init(windowWidth,windowHeight,windowName))
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
			Texture texture("Texture/wall.jpg");

			prepareSingleBuffer(vao, shader);
			GLint modelLocation = shader.getUniformLocation("model");
			GLint viewLocation = shader.getUniformLocation("view");
			GLint projectionLocation = shader.getUniformLocation("projection");
			GLint timeLocation = shader.getUniformLocation("time");
			GLint textureLocation = shader.getUniformLocation("ourTexture");

			shader.begin();
			UploadView(viewLocation);
			UploadPerspectiveProjection(projectionLocation, (float)app->getWidth() / (float)app->getHeight());
			glUniform1i(textureLocation, 0);
			shader.end();

			int projectionWidth = app->getWidth();
			int projectionHeight = app->getHeight();

			while (app->update())
			{
				if ((app->getWidth() != projectionWidth || app->getHeight() != projectionHeight) && app->getHeight() != 0)
				{
					projectionWidth = app->getWidth();
					projectionHeight = app->getHeight();
					shader.begin();
					UploadPerspectiveProjection(projectionLocation, (float)projectionWidth / (float)projectionHeight);
					shader.end();
				}

				render(vao, shader, texture, modelLocation, timeLocation);
			}
		}
		catch (const std::exception& e)
		{
			std::print("Failed to initialize rendering resources, rendering skipped:\n{}\n", e.what());
		}
	}

	app->destroy();
}
