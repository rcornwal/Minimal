/////////////////////
// TexCube.cpp
/////////////////////

#include "Skybox.h"
#pragma warning(disable: 4996)

/////////////////////////////////////////////////////

Skybox::Skybox() {

	// Create the shader to use for the controller
	Shader cubeS(vertexShaderPath, fragShaderPath);
	skyboxShader = cubeS;

	// Sets the position / rotation / scale
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec4(0, 0, 0, 0);
	scale = glm::vec3(20, 20, 20);

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
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,			// left back
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,			// right back
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,			// right front
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,			// right front
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,			// left front
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f			// left back
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

	GLchar* texturePathNX;
	GLchar* texturePathNY;
	GLchar* texturePathNZ;
	GLchar* texturePathPX;
	GLchar* texturePathPY;
	GLchar* texturePathPZ;

	texturePathNX = "./Models/skybox/sea_skybox/nx.ppm";
	texturePathNY = "./Models/skybox/sea_skybox/ny.ppm";
	texturePathNZ = "./Models/skybox/sea_skybox/nz.ppm";
	texturePathPX = "./Models/skybox/sea_skybox/px.ppm";
	texturePathPY = "./Models/skybox/sea_skybox/py.ppm";
	texturePathPZ = "./Models/skybox/sea_skybox/pz.ppm";

	vector<const GLchar*> faces;
	faces.push_back(texturePathPX);
	faces.push_back(texturePathNX);
	faces.push_back(texturePathPY);
	faces.push_back(texturePathNY);
	faces.push_back(texturePathPZ);
	faces.push_back(texturePathNZ);

	skyTexture = loadCubemap(faces);
	// skyTexture = TextureFromFile(texturePathNY);
}

void Skybox::Draw() {
	// Draw the box
	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "ourTexture"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Skybox::Render(glm::mat4 view, glm::mat4 proj) {

	skyboxShader.Use();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "ourTexture"), 0);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(95.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	// Set the model view projection matrix in our shader
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

void Skybox::Render(glm::mat4 view, glm::mat4 proj, GLuint framebuffer) {

	/////////////////////////////////////////////////////
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skyboxShader.Use();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "ourTexture"), 0);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(95.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	// Set the model view projection matrix in our shader
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	Draw();
}

GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = loadPPM(faces[i], width, height);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;

}

unsigned char* Skybox::loadPPM(const char* filename, int& width, int& height)
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


GLint Skybox::TextureFromFile(const char* filename) {
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

