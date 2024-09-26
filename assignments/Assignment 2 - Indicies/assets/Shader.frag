#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform float _Time;
void main()
{
    FragColor = Color * abs(tan(_Time));
}