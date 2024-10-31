#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
uniform float _Time;
// texture samplers
uniform sampler2D texture1;
void main()
{
	FragColor = texture(texture1, TexCoord);
}
