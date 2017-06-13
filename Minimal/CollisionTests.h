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

static struct plane {
	glm::vec3 pos;
	glm::vec3 norm;
};

static struct sphere {
	glm::vec3 center;
	float radius;
};

static class CollisionTests {
public:

	// Functions
	static bool SpherePlane(glm::vec3 sphere_pos, float sphere_rad, glm::vec3 plane_pos, glm::vec3 plane_norm) {
		
		sphere s;
		s.center = sphere_pos;
		s.radius = sphere_rad;

		plane p;
		p.pos = plane_pos;
		p.norm = glm::normalize(plane_norm);

		return sphere_inside_plane(s, p);

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

private:
	static float plane_distance(plane p, glm::vec3 point) {
		return glm::dot((point - p.pos), p.norm);
	}

	static bool sphere_inside_plane(sphere s, plane p) {
		return plane_distance(p, s.center) < s.radius;
	}

	static bool sphere_outside_plane(sphere s, plane p) {
		return plane_distance(p, s.center) > s.radius;
	}

	static bool sphere_intersects_plane(sphere s, plane p) {
		return fabs(plane_distance(p, s.center)) <= s.radius;
	}
};
