#version 330 core
// Classique fragment shader
// Entrée
in vec2 uv_map;

uniform sampler2D tex;

// Sortie
out vec4 out_color;

void main()
{
     // Couleur finale du pixel
     out_color = texture(tex, uv_map);
}

