#version 330

in vec3 in_position;
in vec3 in_normals;

out vec3 vertex_normal;

uniform mat4 mvp;

void main(void)
{
     gl_Position = mvp * vec4(in_position, 1.0);
     vertex_normal = in_normals;
}
