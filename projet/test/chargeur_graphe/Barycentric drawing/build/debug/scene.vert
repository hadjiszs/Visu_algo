#version 330

in vec4 Position;
in vec4 Color;

out vec4 fColor;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * Position;
    fColor = Color;
}
