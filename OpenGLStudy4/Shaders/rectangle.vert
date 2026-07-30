#version 460 core
layout (location = 0) in vec3 aPos;   // 对应VAO的0号属性槽位（顶点位置）
layout (location = 1) in vec3 aColor; // 对应VAO的1号属性槽位（顶点颜色）
out vec3 ourColor; // 传给片段着色器的输出变量，会被光栅化阶段插值
void main()
{
	gl_Position = vec4(aPos, 1.0); // 直接把位置透传成裁剪空间坐标，没有做任何变换矩阵运算
	ourColor = aColor;
}
