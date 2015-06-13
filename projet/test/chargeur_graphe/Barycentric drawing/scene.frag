#version 330

in vec4 fColor;

out vec4 FragColor;

uniform vec4 Tint;

void main()
{
    FragColor = fColor + Tint;
}
