#pragma once

#include <Windows.h>
#include "Shader.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Plane.h"
#include "CollisionTests.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>

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

class Collider {
public:
	enum type { SphereType, BoxType, PlaneType };

	Collider();
	Collider(type, glm::vec3, glm::vec4, glm::vec3);

	/* Data */
	bool isStatic;
	void(*onCollide)();

	/* For Moving Objects*/
	glm::vec3 pos;
	glm::vec4 rot;
	glm::vec3 scale;
	glm::vec3 vel;
	bool offset;
	static glm::vec3 centerPos;
	float gravityScale;
	bool continuous;
	bool active;

	/* Functions */
	void Update();
	void ApplyForce(glm::vec3 vel);
	void ViewCollider(const glm::mat4 & projection, const glm::mat4 & modelview);
	bool CheckForCollision(Collider * otherObject);

	// Sets the attributes to match the model
	void SetRadius(float r);
	void SetNormal(glm::vec3 n);
	void SetPoint(glm::vec3 p);
	void SetXBounds(float minX, float maxX);
	void SetZBounds(float minZ, float maxZ);

private:
	// sphere
	float radius;

	// plane
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec2 xBounds;
	glm::vec2 zBounds;

	type colliderType;
	Primitive *colliderObject;
	float curTime;
	void SetBlueDelay(float t);

protected:

};

