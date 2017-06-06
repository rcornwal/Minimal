/////////////////////
// CollisionTests.h
/////////////////////

#pragma once
#include <Windows.h>

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

static class CollisionTests {
public:

	// Functions
	static bool SpherePlane(glm::vec3 sphere_pos, float sphere_rad, glm::vec3 plane_pos, glm::vec3 plane_norm) {
		
		glm::vec3 pNorm = glm::normalize(plane_norm);
		float D = glm::dot((sphere_pos - plane_pos), pNorm);
		if (D < sphere_rad) {
			return true;
		}
		return false;
	}

	static bool SphereSphere(glm::vec3 sphere1_pos, float sphere1_rad, glm::vec3 sphere2_pos, float sphere2_rad) {
		float dist = glm::distance(sphere1_pos, sphere2_pos);
		if (dist < (sphere1_rad + sphere2_rad)) {
			return true;
		}
		return false;
	}

protected:

};
