/////////////////////
// TexCube.cpp
/////////////////////

#include "TexCube.h"
#pragma warning(disable: 4996)

/////////////////////////////////////////////////////

TexCube::TexCube() {

	// Create the shader to use for the controller
	Shader cubeS(vertexShaderPath, fragShaderPath);
	texCubeShader = cubeS;

	// Sets the position / rotation / scale
	position = glm::vec3(0.0f, -.5f, -.1f);
	rotation = glm::vec4(0, 0, 0, 0);
	scale = glm::vec3(.5f, .5f, .5f);
	origScale = glm::vec3(.5f, .5f, .5f);

	// Set up the arrays
	GLfloat vertices[] = {
		// Position				// Normals				// Texture Coords

		// back face
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,			// left bottom 
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,			// right bottom 
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,			// right top 
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,			// right top 
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,			// left top 
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,			// left bottom 

		// front face
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,			// left bottom
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 0.0f,			// right bottom
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,			// right top
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,			// right top 
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,			// left top 
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,			// left bottom

		// left face
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,			// top front	
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,			// top back
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,			// bottom back
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,			// bottom back
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,			// bottom front
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,			// top front

		// right face
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,			// top front
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,			// top back
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,			// bottom back
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,			// bottom back
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,			// bottom front
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,			// top front

		// bottom face
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,			// left back
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,			// right back
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,			// right front
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,			// right front
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,			// left front
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,			// left back

		// top face
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,			// left back
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,			// right back
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,			// right front
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,			// right front
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,			// left front
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f			// left back
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	cubeTexture = TextureFromFile(texturePath);
}

void TexCube::Draw() {
	// Draw the box
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void TexCube::Render(glm::mat4 view, glm::mat4 proj) {

	texCubeShader.Use();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glUniform1i(glGetUniformLocation(texCubeShader.Program, "ourTexture"), 0);

	//GLint objectColorLoc = glGetUniformLocation(texCubeShader.Program, "objectColor");
	//glUniform3f(objectColorLoc, 0.0f, 1.0f, 0.0f);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

	// Set the model view projection matrix in our shader
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	
	Draw();
}

void TexCube::Render(glm::mat4 view, glm::mat4 proj, GLuint framebuffer) {

	/////////////////////////////////////////////////////
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texCubeShader.Use();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	glUniform1i(glGetUniformLocation(texCubeShader.Program, "ourTexture"), 0);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

	// Set the model view projection matrix in our shader
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(texCubeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

void TexCube::ScaleUp() {
	float scaleStep = .005f;
	if (scale.x + scaleStep > 1) scaleStep = 0;
	scale = glm::vec3(scale.x + scaleStep, scale.y + scaleStep, scale.z + scaleStep);
}

void TexCube::ScaleDown() {
	float scaleStep = .005f;
	if (scale.x - scaleStep < 0) scaleStep = 0;
	scale = glm::vec3(scale.x - scaleStep, scale.y - scaleStep, scale.z - scaleStep);
}

void TexCube::MoveRight() {
	float moveStep = .005f;
	position = glm::vec3(position.x + moveStep, position.y, position.z);
}

void TexCube::MoveLeft() {
	float moveStep = .005f;
	position = glm::vec3(position.x - moveStep, position.y, position.z);
}

void TexCube::MoveUp() {
	float moveStep = .005f;
	position = glm::vec3(position.x, position.y + moveStep, position.z);
}

void TexCube::MoveDown() {
	float moveStep = .005f;
	position = glm::vec3(position.x, position.y - moveStep, position.z);
}

void TexCube::ResetScale() {
	scale = origScale;
}

unsigned char* TexCube::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return 0;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;

		return 0;
	}

	return rawData;
}


GLint TexCube::TextureFromFile(const char* filename) {
	// generate texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = loadPPM(filename, width, height);

	// assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureID;
}

