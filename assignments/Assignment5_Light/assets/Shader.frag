#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
uniform float _Time;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
	// linearly interpolate between both textures (60% crystal, 40% gem)
	//FragColor = mix(texture(texture1, TexCoord)*vec4(Color*abs(tan(_Time))), texture(texture2, TexCoord), 0.4);
	FragColor = texture(texture1, TexCoord)*vec4(Color*abs(tan(_Time)));
}
