#version 330 core
// Classique vertex shader
// Entrée
in vec3 in_position;
in vec2 in_uv_map;

// Uniform
uniform mat4 mvp;

// Sortie envoyé au fragment shader
out vec2 uv_map;

void main()
{
     // Position 3D finale du vertex
     gl_Position = mvp * ( vec4(in_position, 1.0) );

     // Envoi de la coordonnees de position texture au Fragment Shader
     uv_map = in_uv_map;
	
}