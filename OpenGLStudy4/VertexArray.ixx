module;
#include <glad/glad.h>
#include <glfw/glfw3.h>
export module OpenGLStudy.VertexArray;
export namespace OpenGLStudy
{
	class VertexArray // VAO(顶点数组对象)的RAII封装：本身不存顶点数据，存的是"顶点属性该怎么读"这份配置
	{
	public:
		VertexArray();  // 构造时glGenVertexArrays申请一个VAO对象
		~VertexArray(); // 析构时glDeleteVertexArrays自动释放，不需要调用方手动管理
		// 禁止拷贝：避免两个对象持有同一个mId，其中一个析构把GL对象删了，另一个就变悬空指针
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void bind() const;        // 把这个VAO设为"当前"，之后的顶点属性配置/绘制调用都针对它生效
		static void unbind();     // 解绑（绑定0），避免后续代码不小心改动到这个VAO
		GLuint id() const { return mId; } // 暴露底层句柄，供调试打印或直接调GL API使用
	private:
		GLuint mId{}; // 底层OpenGL VAO对象的句柄
	};
}
