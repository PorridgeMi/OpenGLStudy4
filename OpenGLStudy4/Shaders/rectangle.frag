#version 460 core
out vec4 FragColor;
in vec3 ourColor;
uniform float time;
void main()
{
	float brightness = 0.5 + 0.5 * sin(time);
	FragColor = vec4(ourColor * brightness, 1.0f);
}
