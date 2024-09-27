#version 330 core
out vec4 FragColor;
in vec4 Color;
in vec2 TexCoord;
uniform float _Time;
uniform sampler2D ourTexture;
void main()
{
    FragColor =texture(ourTexture, TexCoord);
    //* vec4(Color * abs(tan(_Time)), 1.0)
}