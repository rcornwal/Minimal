#pragma once

#include <Windows.h>
#include "Collider.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

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

static class Physics {
public:

	static vector<Collider *> Physics::objects;
	static bool debug;
	const static float gravity;

	/* Functions */
	static void AddObject(Collider* c);
	static void RemoveObject(Collider c) {}
	static void Update(const glm::mat4 & projection, const glm::mat4 & modelview);

private:

	static vector<pair<int, pair<int, int>>> recentCollisions; // [<lifetime, <obj1, obj2>>]

protected:

};

