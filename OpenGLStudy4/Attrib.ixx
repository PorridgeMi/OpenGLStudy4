module;
#include<glad/glad.h>
#include<glfw/glfw3.h>
export module OpenGLStudy.Attrib;
import std;
export namespace OpenGLStudy
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;


	template<Arithmetic T,int vertexNum,int vertexSize,int ...lengths>
	requires(vertexNum>0 && vertexSize>0 && (lengths + ...)==vertexSize)
	class Attrib
	{
	public:
		Attrib(const std::array<T, vertexNum * vertexSize>& newData) : data(newData) {}

		std::array<T, vertexNum * vertexSize> getData() const { return data; }

		template<int num2>
		static constexpr int getStride()
		{
			constexpr std::array<int, sizeof...(lengths)> arr{ lengths... };
			int offset = 0;
			for (int i = 0; i < num2; ++i)
			{
				offset += arr[i];
			}
			return offset;
		}

		int getAttribNum() const
		{
			return sizeof...(lengths);
		}
	private:
		std::array<T, vertexNum * vertexSize> data;
	};
}
