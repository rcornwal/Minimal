/////////////////////
// Shield.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"
#include "GameObject.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

class Shield : public GameObject {
public:

	Shield();

	// Positioning
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);
	glm::vec3 GetColor();
	glm::vec3 GetPos();
	glm::vec4 GetRot();
	glm::vec3 GetScale();

private:
	/* Data */
	Model shieldModel;
	GLchar* pathToShield = "./Models/shield/shield.obj";
	GLchar* vertexShaderPath = "./Models/shield/shield.vs";
	GLchar* fragShaderPath   = "./Models/shield/shield.frag";
	Shader shieldShader;
	bool growing;
	float maxScale;
	float minScale;
	float scaleStep;
	float curScale;
};