import std;
import OpenGLStudy.Application;
import OpenGLStudy.Shader;
import OpenGLStudy.VertexArray;
import OpenGLStudy.Callback;
import OpenGLStudy.Transform;
import OpenGLStudy.Texture;
import OpenGLStudy.Camera;

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

void prepareCubeBuffer(const VertexArray& vao, const Shader& shader)
{
	GLint posLocation = requireAttribLocation(shader, "aPos");
	GLint colorLocation = requireAttribLocation(shader, "aColor");
	GLint texCoordLocation = requireAttribLocation(shader, "aTexCoord");

	std::array positions{
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	std::array<float, 108> colors;
	colors.fill(1.0f);
	std::array texCoords{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
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

	VertexArray::unbind();

	glDeleteBuffers(1, &posVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &texCoordVbo);
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

void processInput(Application* app, Camera& camera, float deltaTime)
{
	if (app->isKeyPressed(GLFW_KEY_W))
	{
		camera.processKeyboard(CameraMovement::Forward, deltaTime);
	}
	if (app->isKeyPressed(GLFW_KEY_S))
	{
		camera.processKeyboard(CameraMovement::Backward, deltaTime);
	}
	if (app->isKeyPressed(GLFW_KEY_A))
	{
		camera.processKeyboard(CameraMovement::Left, deltaTime);
	}
	if (app->isKeyPressed(GLFW_KEY_D))
	{
		camera.processKeyboard(CameraMovement::Right, deltaTime);
	}
}

void render(const VertexArray& vao, const Shader& shader, const Texture& texture, const Texture& faceTexture, const Camera& camera, GLint modelLocation, GLint viewLocation, GLint timeLocation)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.begin();

	UploadModelRotation(modelLocation, (float)glfwGetTime());
	UploadView(viewLocation, camera.viewMatrix());
	glUniform1f(timeLocation, (float)glfwGetTime());

	texture.bind();
	faceTexture.bind();
	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	VertexArray::unbind();
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
	Camera camera;
	bool firstMouse = true;
	double lastX = 0.0;
	double lastY = 0.0;

	app->setResizeCallback(OnResize);
	app->setKeyCallback(OnKey);
	app->setMouseButtonCallback(OnMouseButton);
	app->setCursorPosCallback([&camera, &firstMouse, &lastX, &lastY](double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = (float)(xpos - lastX);
		float yoffset = (float)(lastY - ypos);
		lastX = xpos;
		lastY = ypos;
		camera.processMouseMovement(xoffset, yoffset);
	});
	app->setScrollCallback([&camera](double xoffset, double yoffset)
	{
		camera.processMouseScroll((float)yoffset);
	});
	app->setCursorCaptured(true);
	glViewport(0, 0, app->getWidth(), app->getHeight());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	{
		VertexArray vao;

		try
		{
			Shader shader = prepareShader();
			Texture texture("Textures/wall.jpg", 0);
			Texture faceTexture("Textures/awesomeface.png", 1);

			prepareCubeBuffer(vao, shader);
			GLint modelLocation = shader.getUniformLocation("model");
			GLint viewLocation = shader.getUniformLocation("view");
			GLint projectionLocation = shader.getUniformLocation("projection");
			GLint timeLocation = shader.getUniformLocation("time");
			GLint textureLocation = shader.getUniformLocation("ourTexture");
			GLint faceTextureLocation = shader.getUniformLocation("faceTexture");

			shader.begin();
			UploadPerspectiveProjection(projectionLocation, camera.fov(), (float)app->getWidth() / (float)app->getHeight());
			glUniform1i(textureLocation, texture.unit());
			glUniform1i(faceTextureLocation, faceTexture.unit());
			shader.end();

			int projectionWidth = app->getWidth();
			int projectionHeight = app->getHeight();
			float projectionFov = camera.fov();
			float lastFrameTime = (float)glfwGetTime();

			while (app->update())
			{
				float currentFrameTime = (float)glfwGetTime();
				float deltaTime = currentFrameTime - lastFrameTime;
				lastFrameTime = currentFrameTime;
				processInput(app, camera, deltaTime);

				if ((app->getWidth() != projectionWidth || app->getHeight() != projectionHeight || camera.fov() != projectionFov) && app->getHeight() != 0)
				{
					projectionWidth = app->getWidth();
					projectionHeight = app->getHeight();
					projectionFov = camera.fov();
					shader.begin();
					UploadPerspectiveProjection(projectionLocation, projectionFov, (float)projectionWidth / (float)projectionHeight);
					shader.end();
				}

				render(vao, shader, texture, faceTexture, camera, modelLocation, viewLocation, timeLocation);
			}
		}
		catch (const std::exception& e)
		{
			std::print("Failed to initialize rendering resources, rendering skipped:\n{}\n", e.what());
		}
	}

	app->destroy();
}
