
#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 objectColor;

void main()
{
    color = vec4(objectColor, 1.0f);
	color.a = .3f;
} 

