#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class AnaglyphCam {

public:

	AnaglyphCam(float iod, float ratio, float fov, float convergence, float near, float far);
	glm::mat4 RightFrustum();
	glm::mat4 LeftFrustum();
	float getIOD();

private:

	void CalculateABC(float * a, float * b, float * c);

	float EyeSeparation;
	float AspectRatio;
	float FOV;
	float Convergence;
	float NearClip;
	float FarClip;
};