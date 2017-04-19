#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 matWVP;

void main(){
	gl_Position = matWVP * vec4(position, 1.0f);
	TexCoords = texCoords;
}