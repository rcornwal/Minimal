
#version 330 core

in vec2 TexCoord;
in vec3 Normal;  

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 objectColor;

void main()
{
    color = texture(ourTexture, TexCoord);
	//color = vec4 (1.0f, 0.0f, 0.0f, 1.0f);
}

