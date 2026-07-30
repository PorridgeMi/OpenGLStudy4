#version 460 core
out vec4 FragColor; // 片段着色器的输出：这个像素最终的颜色
in vec3 ourColor; // 从顶点着色器传过来的插值颜色
void main()
{
	FragColor = vec4(ourColor, 1.0f); // 使用顶点着色器传过来的颜色
}
