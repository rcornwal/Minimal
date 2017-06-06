/////////////////////
// Physics.cpp
/////////////////////

#include "GameObject.h"

/////////////////////////////////////////////////////
vector<GameObject *> GameObject::objects;

void printv(glm::vec3 v) {
	OutputDebugString("(");
	OutputDebugString(to_string(v.x).c_str());
	OutputDebugString(", ");
	OutputDebugString(to_string(v.y).c_str());
	OutputDebugString(", ");
	OutputDebugString(to_string(v.z).c_str());
	OutputDebugString(")\n");
}

void GameObject::RenderAll(const glm::mat4 & modelview, const glm::mat4 & projection) {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render(modelview, projection);
	}
}

GameObject::GameObject() {
	objects.push_back(this);
}

glm::vec3 GameObject::GetPoint(glm::vec3 p) {
	// Match the collider to our model
	glm::vec3 mPos = GetPos();
	glm::vec4 mRot = GetRot();
	glm::vec3 mScale = GetScale();

	// Set up model matrix
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), mPos);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), mRot.w, glm::vec3(mRot.x, mRot.y, mRot.z));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), mScale);
	modelMat = T * R * S;

	glm::vec4 ip(p.x, p.y, p.z, 1.0f);
	ip = modelMat * ip;

	return (glm::vec3(ip.x, ip.y, ip.z));
}

glm::vec3 GameObject::GetDirection(glm::vec3 d) {
	// Match the collider to our model
	glm::vec3 mPos = GetPos();
	glm::vec4 mRot = GetRot();
	glm::vec3 mScale = GetScale();

	// Set up model matrix
	glm::mat4 modelMat;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), mPos);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), mRot.w, glm::vec3(mRot.x, mRot.y, mRot.z));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), mScale);
	modelMat = T * R * S;

	glm::vec4 ip(d.x, d.y, d.z, 0.0f);
	ip = modelMat * ip;

	return (glm::vec3(ip.x, ip.y, ip.z));
}

void GameObject::AddCollider(Collider::type cType) {

	// Match the collider to our model
	glm::vec3 mPos = GetPos();
	glm::vec4 mRot = GetRot();
	glm::vec3 mScale = GetScale();

	// Create our collider 
	collider = new Collider(cType, mPos, mRot, mScale);

	// Add the collider to our list of physical objects
	Physics::AddObject(collider);

	// Setup our plane collider
	if (cType == Collider::PlaneType) {
		Model * model = GetModel();
		if (model == nullptr)
			return;

		glm::vec3 max = model->GetMax();
		glm::vec3 min = model->GetMin();

		// normal
		glm::vec3 p1 = max;
		glm::vec3 p2 = min;
		glm::vec3 p3 = glm::vec3(min.x, max.y, max.z);
		glm::vec3 dir1 = p3 - p1;
		glm::vec3 dir2 = p3 - p2;
		glm::vec3 norm = glm::normalize(glm::cross(dir1, dir2));

		printv(GetDirection(norm));

		// bounds
		collider->SetXBounds(min.x, max.x);
		collider->SetZBounds(min.z, max.z);

		max = GetPoint(model->GetMax());
		min = GetPoint(model->GetMin());

		printv(GetPos());
		collider->SetPoint(GetPos());
		collider->SetNormal(GetDirection(norm));


	}
}

void GameObject::SetRadius(float radius) {
	collider->SetRadius(radius);
}


void GameObject::ApplyForce(glm::vec3 force) {
	collider->ApplyForce(force);
}

void GameObject::IsStatic(bool s) {
	collider->isStatic = s;
}

void GameObject::SetGravityScale(float s) {
	collider->gravityScale = s;
}

void GameObject::SetContinuousCollision(bool c) {
	collider->continuous = c;
}