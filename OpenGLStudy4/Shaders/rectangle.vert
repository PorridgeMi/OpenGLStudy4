#version 460 core
in vec3 aPos;
in vec3 aColor;
out vec3 ourColor;
uniform mat4 transform;
void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	ourColor = aColor;
}
