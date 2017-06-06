/////////////////////
// CaveWall.cpp
/////////////////////

#include "CaveWall.h"
#pragma warning(disable: 4996)

/////////////////////////////////////////////////////

CaveWall::CaveWall() {

	// Create the shader to use for the controller
	Shader cubeS(vertexShaderPath, fragShaderPath);
	CaveWallShader = cubeS;

	// Sets the position / rotation / scale
	color = glm::vec3(0, 0, 0);
	position = glm::vec3(0.0f, 0.0f, -1.0f);
	rotation = glm::mat4(1.0f);
	scale = glm::vec3(3.0f, 3.0f, 3.0f);

	// Set up the arrays
	GLfloat vertices[] = {
		// Position				// Normals				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,		// left bottom 
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,		// right bottom 
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,		// right top 
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,		// right top 
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,		// left top 
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,		// left bottom 
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

	GenerateFramebuffer(0);
}

void CaveWall::GenerateFramebuffer(GLuint defaultFB) {

	screensize.x = 1000;
	screensize.y = 1000;

	/* =============== Generate framebuffer for the wall ================== */
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create a color attachment texture
	textureColorbuffer = GenerateAttachmentTexture();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screensize.x, screensize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check to make sure the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		OutputDebugString("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
	}
	else {
		OutputDebugString("FRAMEBUFFER SETUP CORRECTLY!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, defaultFB);
}

glm::mat4 CaveWall::GenerateProjection(glm::mat4 view) {

	// Info we have
	glm::vec3 pa = bottomRightPos;		
	glm::vec3 pb = bottomLeftPos;		
	glm::vec3 pc = topRightPos;  
	glm::vec3 pe = (view[3]);

	OutputDebugString(to_string(pe.x).c_str());
	OutputDebugString(", ");
	OutputDebugString(to_string(pe.y).c_str());
	OutputDebugString(", ");
	OutputDebugString(to_string(pe.z).c_str());
	OutputDebugString("\n");



	float n = .001f;
	float f = 1000.0f;

	// Vectors we will calculate
	glm::vec3 va = pa - pe; 
	glm::vec3 vb = pb - pe; 
	glm::vec3 vc = pc - pe;
	glm::vec3 vr = glm::normalize(pb - pa); 
	glm::vec3 vu = glm::normalize(pc - pa); 
	glm::vec3 vn = glm::normalize(glm::cross(vr, vu));

	float l, r, b, t, d;

	// Find the distance from the eye to screen plane.
	d = -glm::dot(va, vn);

	// Find the extent of the perpendicular projection.
	l = glm::dot(vr, va) * n / d;
	r = glm::dot(vr, vb) * n / d;
	b = glm::dot(vu, va) * n / d;
	t = glm::dot(vu, vc) * n / d;

	// Load the perpendicular projection.
	glm::mat4 frustum = glm::frustum(l, r, b, t, n, f);

	// rotate the frustum
	glm::mat4 M = glm::mat4(glm::mat3(vr, vu, vn));
	M[3][3] = 1.0f;

	glm::mat4 T = glm::mat4(1.0f);
	T[3][0] = -pe.x;
	T[3][1] = -pe.y;
	T[3][2] = -pe.z;
	T[3][3] = 1.0f;

	glm::mat4 Mt = glm::transpose(M);

	frustum = frustum * Mt * T;

	glm::mat4 flipX = glm::rotate(glm::mat4(), 3.14159f, glm::vec3(0, 0, 1));
	glm::mat4 flipY = glm::rotate(glm::mat4(), 3.14159f, glm::vec3(0, 1, 0));

	frustum = frustum * flipX * flipY;

	return frustum;
}

void CaveWall::Draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void CaveWall::Render(glm::mat4 view, glm::mat4 proj, int eyeNum) {

	CaveWallShader.Use();

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	glm::mat4 transM = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), scale);
	model = transM * rotation * scaleM;

	// Set the model view projection matrix in our shader
	glUniformMatrix4fv(glGetUniformLocation(CaveWallShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(CaveWallShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(CaveWallShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	
	// Update our corners with the transorm of the wall
	topRightPos		= model * glm::vec4(.5f, .5f, -.5f, 1);
	topLeftPos		= model * glm::vec4(-.5f, .5f, -.5f, 1);
	bottomRightPos  = model * glm::vec4(.5f, -.5f, -.5f, 1);
	bottomLeftPos   = model * glm::vec4(-.5f, -.5f, -.5f, 1);;

	Draw();

	// Get the camera's (eye's) position
	glm::mat4 view_inverse = inverse(view);
	glm::vec3 cameraPos (view[3]);
	glUniform3f (glGetUniformLocation(CaveWallShader.Program, "headPos"), 1, GL_FALSE, (cameraPos.x, cameraPos.y, cameraPos.z));


	// The debugging lines
	debug_topRight.SetStartPos(cameraPos);
	debug_topRight.SetEndPos(glm::vec3(topRightPos.x, topRightPos.y, topRightPos.z));
	debug_topRight.color = (eyeNum == 1) ? glm::vec3(1.0f, 0, 0) : glm::vec3(0, 1.0f, 0);

	debug_topLeft.SetStartPos(cameraPos);
	debug_topLeft.SetEndPos(glm::vec3(topLeftPos.x, topLeftPos.y, topLeftPos.z));
	debug_topLeft.color = (eyeNum == 1) ? glm::vec3(1.0f, 0, 0) : glm::vec3(0, 1.0f, 0);

	debug_bottomRight.SetStartPos(cameraPos);
	debug_bottomRight.SetEndPos(glm::vec3(bottomRightPos.x, bottomRightPos.y, bottomRightPos.z));
	debug_bottomRight.color = (eyeNum == 1) ? glm::vec3(1.0f, 0, 0) : glm::vec3(0, 1.0f, 0);

	debug_bottomLeft.SetStartPos(cameraPos);
	debug_bottomLeft.SetEndPos(glm::vec3(bottomLeftPos.x, bottomLeftPos.y, bottomLeftPos.z));
	debug_bottomLeft.color = (eyeNum == 1) ? glm::vec3(1.0f, 0, 0) : glm::vec3(0, 1.0f, 0);

}

void CaveWall::RenderDebug(glm::mat4 view, glm::mat4 proj) {
	debug_topRight.Render(view, proj);
	debug_topLeft.Render(view, proj);
	debug_bottomRight.Render(view, proj);
	debug_bottomLeft.Render(view, proj);
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint CaveWall::GenerateAttachmentTexture(){

	// What enum to use
	GLenum attachment_type;
	attachment_type = GL_RGB;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screensize.x, screensize.y, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
