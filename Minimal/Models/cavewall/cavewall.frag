
#version 330 core
out vec4 color;

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
  
uniform sampler2D wallTexture;
uniform vec3 headPos;

void main()
{
    
	// Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(headPos - FragPos);
    float diff = max(dot(norm, -lightDir), 0.0);

	color = texture(wallTexture, TexCoords) * diff;
} 

