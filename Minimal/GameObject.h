#pragma once

#include <Windows.h>
#include "Model.h"
#include "Physics.h"
#include "Collider.h"

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

class GameObject {
public:

	GameObject();
	static glm::vec3 centerPos;
	static vector<GameObject *> GameObject::objects;
	static void RenderAll(const glm::mat4 & projection, const glm::mat4 & modelview);

	/* Functions */
	virtual void Render(glm::mat4 view, glm::mat4 proj, glm::vec3 centerPos) {}
	glm::vec3 GetPoint(glm::vec3 p);
	glm::vec3 GetDirection(glm::vec3 d);
	void AddCollider(Collider::type cType);
	void ApplyForce(glm::vec3 force);
	void SetRadius(float radius);
	void IsStatic(bool isStatic);
	void SetGravityScale(float gravityScale);
	void SetContinuousCollision(bool c);
	Collider * collider;

private:

protected:
	virtual glm::vec3 GetPos() { return glm::vec3(0, 0, 0); }
	virtual glm::vec3 GetScale() { return glm::vec3(0, 0, 0); }
	virtual glm::vec4 GetRot() { return glm::vec4(0, 0, 0, 1.0f); }
	virtual Model* GetModel() { return nullptr; }


};

