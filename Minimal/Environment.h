/////////////////////
// Environment.h
/////////////////////

#pragma once
#include <Windows.h>
#include "Shader.h"

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
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

// All of our environment objects (floor / walls / etc)
#include "Skybox.h"
#include "Floor.h"
#include "Wall.h"
#include "Beachball.h"

class Environment {
public:
	Floor floor;
	Wall front;
	Wall back;
	Wall left;
	Wall right;
	Beachball beachball;

	// Place and scale all of our objects
	Environment() {
		floor.position = glm::vec3(-0.0f, -1.5f, 0.0f);
		floor.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		floor.AddCollider(Collider::PlaneType);
		floor.IsStatic(true);

		front.position = glm::vec3(0.0f, -1.5f, -4.0f);
		front.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		front.rotation = glm::vec4(1.0, 0, 0, 1.5708);
		front.AddCollider(Collider::PlaneType);
		front.IsStatic(true);
		
		back.position = glm::vec3(0.0f, -1.5f, 4.0f);
		back.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		back.rotation = glm::vec4(1.0, 0, 0, -1.5708);
		back.AddCollider(Collider::PlaneType);
		back.IsStatic(true);
		
		left.position = glm::vec3(-4.0f, -1.5f, 0.0f);
		left.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		left.rotation = glm::vec4(0, 0, 1.0, -1.5708);
		left.AddCollider(Collider::PlaneType);
		left.IsStatic(true);
		
		right.position = glm::vec3(4.0f, -1.5f, 0.0f);
		right.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		right.rotation = glm::vec4(0, 0, 1.0, 1.5708);
		right.AddCollider(Collider::PlaneType);
		right.IsStatic(true);
		
		beachball.position = glm::vec3(0, 0, -1.0);
		beachball.scale = glm::vec3(.25f, .25f, .25f);
		beachball.AddCollider(Collider::SphereType);
		beachball.SetRadius(.25f);
		beachball.SetGravityScale(.5f);
		beachball.SetContinuousCollision(true);

	}

	void Render(glm::mat4 view, glm::mat4 proj) {

	}

private:
	/* Data */

};