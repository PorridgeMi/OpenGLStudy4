#version 460 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 texCoord;
uniform float time;
uniform sampler2D ourTexture;
void main()
{
	float brightness = 0.5 + 0.5 * sin(time);
	vec3 colorShift = vec3(
		0.5 + 0.5 * sin(time),
		0.5 + 0.5 * sin(time + 2.094),
		0.5 + 0.5 * sin(time + 4.189)
	);
	vec3 shiftedColor = mix(ourColor, colorShift, 0.3);
	FragColor = texture(ourTexture, texCoord) * vec4(shiftedColor * brightness, 1.0f);
}
