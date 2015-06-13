#version 330

in vec4 in_position;
in vec4 in_color;

out vec4 fColor;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * in_position;
    fColor = in_color;
}
