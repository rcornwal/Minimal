#pragma once

#include <Windows.h>
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

class Beachball : public GameObject{
public:

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	Model model;
	Shader shader;

	Beachball();
	void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos);


protected:

	// Data
	GLchar* vertexShaderPath = "./Models/Beachball/beachball.vs";
	GLchar* fragShaderPath = "./Models/Beachball/beachball.frag";
	GLchar* pathToModel = "./Models/Beachball/beachball.obj";

	glm::vec3 GetPos();
	glm::vec3 GetScale();
	glm::vec4 GetRot();
	Model * GetModel();

};

