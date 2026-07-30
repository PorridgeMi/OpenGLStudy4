// 程序入口。负责：初始化 Application/窗口 -> 准备顶点数据和 shader -> 进入渲染循环。
import std;
import OpenGLStudy.Application;
import OpenGLStudy.Shader;
import OpenGLStudy.VertexArray;
import OpenGLStudy.Callback;

// glad/glfw是C库，不是C++ module，只能用#include引入，且必须放在文件最前面
// （因为main.cpp本身不是模块单元，没有module;全局模块片段这个概念，直接#include即可）
#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace OpenGLStudy;

// 窗口初始尺寸，constexpr表示编译期常量
constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

// 查询某个顶点属性在shader里的位置，如果没查到（名字写错，或者没被用到、被编译器优化掉了）就直接抛异常。
// getAttribLocation本身返回-1表示"没找到"，但如果不在这里主动判断，-1会被后面的glEnableVertexAttribArray/
// glVertexAttribPointer当成一个合法槽位号使用（隐式转成GLuint后会变成一个巨大的数），触发GL_INVALID_VALUE，
// 而且报错的地方跟真正的原因（属性名不存在）离得很远，不好排查。
GLint requireAttribLocation(const Shader& shader, std::string_view name)
{
	GLint location = shader.getAttribLocation(std::string(name));
	if (location == -1)
	{
		throw std::runtime_error(std::string("Vertex attribute not found in shader: ") + std::string(name));
	}
	return location;
}

// 用"分开存储"的策略准备一个矩形的顶点数据：
// 位置(position)和颜色(color)分别放在两个独立的VBO里，
// 通过同一个VAO把它们的读取方式关联起来；另外用一个EBO(索引缓冲区)
// 复用顶点，只存4个顶点、靠索引拼出2个三角形，而不是重复写6个顶点。
void prepareSingleBuffer(const VertexArray& vao, const Shader& shader)
{
	//0.动态获取位置：不写死0/1，而是问shader要"aPos"/"aColor"这两个属性实际被分配到了哪个槽位。
	// 用requireAttribLocation而不是直接调shader.getAttribLocation，是为了在"没查到"（返回-1）的情况下
	// 立刻抛异常，而不是让-1被当成一个合法槽位号继续往下传
	GLint posLocation = requireAttribLocation(shader, "aPos");
	GLint colorLocation = requireAttribLocation(shader, "aColor");

	//1.准备positions colors数据（矩形的4个角，每个顶点3个float：x,y,z 或 r,g,b）
	// 顺序：左上(0) 右上(1) 右下(2) 左下(3)
	std::array positions{
		-0.5f,  0.5f, 0.0f, // 0 左上
		 0.5f,  0.5f, 0.0f, // 1 右上
		 0.5f, -0.5f, 0.0f, // 2 右下
		-0.5f, -0.5f, 0.0f, // 3 左下
	};
	std::array colors{
		1.0f, 0.0f, 0.0f, // 0 红
		0.0f, 1.0f, 0.0f, // 1 绿
		0.0f, 0.0f, 1.0f, // 2 蓝
		1.0f, 1.0f, 0.0f, // 3 黄
	};
	// 索引数据：两个三角形共用0号(左上)和2号(右下)这两个顶点，不用重复存储
	std::array<GLuint, 6> indices{
		0, 1, 2,
		2, 3, 0
	};
		//2.使用数据生成两个vbo（顶点缓冲对象，存放在GPU显存里的原始字节数据）
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);

	// 把positions数据上传到posVbo：先绑定，再glBufferData把CPU内存里的数据拷贝进显存
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);

	// 同理把colors数据上传到colorVbo
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);


	//3.把位置、颜色属性的描述信息绑定到vao上
	vao.bind();
	vao.bind(); // 连续绑定两次：见下方prepareInterleavedBuffer里的说明，这是AMD驱动那个"首次绑定不生效"怪癖的workaround
	// 绑定posVbo后调用的glVertexAttribPointer，会把"posLocation号属性槽位应该从posVbo读数据"这件事记录进当前VAO
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(posLocation); // 开启shader里aPos实际所在的那个属性槽位
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // 3个float一组，无需归一化，紧密排列(stride=0)，从偏移0开始

	// 同理，绑定colorVbo后配置aColor所在的属性槽位
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//4.创建EBO：注意EBO的绑定关系(GL_ELEMENT_ARRAY_BUFFER)是被VAO记住的状态，
	// 所以必须在VAO已经绑定、还没解绑的这个区间内创建和绑定，顺序很重要
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	VertexArray::unbind(); // 解绑VAO，避免后续代码不小心改动它的状态

	//VAO已经记住了属性配置和EBO绑定，三个缓冲区都可以安全删除
	// （VAO内部会持有对它们的引用，删除"名字"不会影响VAO里已经记录的绑定关系）
	glDeleteBuffers(1, &posVbo);
	glDeleteBuffers(1, &colorVbo);
	glDeleteBuffers(1, &ebo);
}

// 用"交错存储"的策略准备同一个矩形：位置和颜色紧挨着存在同一个VBO里，
// 通过stride（每个顶点占用的字节数）和offset（某个属性在顶点内的起始偏移）区分；
// 同样用EBO复用顶点。目前main()里没有调用这个函数，留着跟prepareSingleBuffer做对比/参考。
void prepareInterleavedBuffer(const VertexArray& vao, const Shader& shader)
{
	//0.动态获取位置：同prepareSingleBuffer里的做法
	GLint posLocation = requireAttribLocation(shader, "aPos");
	GLint colorLocation = requireAttribLocation(shader, "aColor");

	//1.准备interleaved数据：每个顶点后面紧跟着它的颜色，position和color交替存放
	// 布局：[x,y,z, r,g,b] x4，顺序：左上(0) 右上(1) 右下(2) 左下(3)
	std::array vertices{
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 0 左上 红
		 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 1 右上 绿
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 2 右下 蓝
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // 3 左下 黄
	};
	// 索引数据，跟prepareSingleBuffer里的一样：两个三角形共用0号和2号顶点
	std::array<GLuint, 6> indices{
		0, 1, 2,
		2, 3, 0
	};

	//2.用同一份数据生成一个vbo
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	//3.把位置、颜色属性的描述信息绑定到vao上
	// 这里连续绑定两次是为了绕开某些AMD显卡驱动的一个怪癖——
	// 刚生成的VAO第一次绑定不会真正生效，必须紧接着再绑定同一个id一次才会生效，
	// 否则接下来的glVertexAttribPointer会报GL_INVALID_OPERATION（因为"没有真正绑定VAO"）
	vao.bind();
	vao.bind();

	//4.从同一个vbo里分别取出位置、颜色属性，用stride和offset区分
	constexpr GLsizei stride = 6 * sizeof(float); // 每个顶点占6个float（3位置+3颜色）
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); // 位置：从第0个字节开始

	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // 颜色：跳过前3个float(位置)之后开始

	//5.创建EBO，必须在VAO还绑定着的时候创建/绑定，原理和prepareSingleBuffer里一样
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	VertexArray::unbind();

	//VAO已经记住了属性配置和EBO绑定，VBO、EBO都可以安全删除
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

// 从Shaders/目录下的文件读取vs与fs源码，编译、链接成shader program。
// 返回一个Shader对象（RAII管理，析构时自动glDeleteProgram）
Shader prepareShader()
{
	// 路径是相对于程序运行时的工作目录的——.vcxproj里配了PostBuildEvent，
	// 每次编译会把Shaders文件夹整个拷贝到exe旁边，所以这个相对路径能找到文件
	return Shader("Shaders/rectangle.vert", "Shaders/rectangle.frag");
}

// 每一帧调用一次：清屏 -> 启用shader -> 绑定vao -> 发出绘制指令
void render(const VertexArray& vao, const Shader& shader)
{
	glClear(GL_COLOR_BUFFER_BIT); // 用glClearColor设置好的颜色清空颜色缓冲区
	shader.begin();              // 告诉GPU接下来用这个shader program
	vao.bind();                  // 告诉GPU接下来用这个VAO记录的顶点属性配置(包括EBO绑定)
	// 按索引绘制：6个索引，类型是无符号int，从EBO偏移0开始读
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 画完解绑：现在只画一个物体，这两行不解绑也不会出问题，
	// 但提前养成习惯，以后场景里有多个VAO/shader时能避免"忘记切换、用错绑定状态"这类bug
	VertexArray::unbind();
	shader.end();
}

int main()
{
	// Application是单例，getInstance()第一次调用时会在堆上new出唯一实例
	Application* app = Application::getInstance();
	// init()内部完成：glfwInit、创建窗口、创建OpenGL上下文、加载GLAD、注册窗口回调
	if (!app->init(windowWidth,windowHeight,"MyWindow"))
	{
		std::print("Failed to initialize Application");
		app->destroy(); // 即使init失败也调用一下destroy，确保glfwTerminate等收尾工作被执行
		return -1;
	}
	// 把main.cpp里定义的这几个回调函数注册给Application，
	// 之后窗口大小变化/按键/鼠标事件发生时，Application内部的静态回调会转发调用到这几个函数
	app->setResizeCallback(OnResize);
	app->setKeyCallback(OnKey);
	app->setMouseButtonCallback(OnMouseButton);
	app->setCursorPosCallback(OnCursorPos);
	app->setScrollCallback(OnScroll);
	//设置视口大小（告诉GL渲染结果要映射到窗口的哪块像素区域）
	glViewport(0, 0, app->getWidth(), app->getHeight());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 背景色：深青灰色，只需要设置一次，不用每帧重设

	{
		// 用花括号包一层作用域：保证vao/shader在离开这个作用域时（也就是app->destroy()销毁GL上下文之前）
		// 就已经被析构、对应的glDelete*调用已经执行完，避免"上下文已经销毁、还在删GL对象"的问题
		VertexArray vao;

		// prepareShader()内部读文件、编译、链接任意一步失败，都会抛出std::runtime_error，
		// 这里catch住它，打印出错误原因、直接跳过渲染循环，而不是让程序崩溃或者带着无效shader继续跑。
		// prepareSingleBuffer需要查询shader里aPos/aColor的实际位置，所以也放进try里、在shader创建成功之后再调用
		try
		{
			Shader shader = prepareShader();

			prepareSingleBuffer(vao, shader); // 只调用了这一个策略；prepareInterleavedBuffer留着做对比

			//渲染循环：只要窗口没有被关闭（ESC键/点击关闭按钮），就一直重复渲染+处理事件
			while (app->update())
			{
				render(vao, shader);
			}
		}
		catch (const std::exception& e)
		{
			std::print("Failed to prepare shader, rendering skipped:\n{}\n", e.what());
		}
	} // vao、shader 在这里析构，此时GL上下文还有效

	app->destroy(); // 销毁窗口、调用glfwTerminate()
}
