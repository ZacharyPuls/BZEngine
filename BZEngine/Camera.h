#pragma once

#include "Headers.h"
#include <atomic>

class Camera
{
public:
	Camera(std::string name = "") : Camera({}, {}, {}, {}, {}, {}, {}, std::move(name))
	{
	}

	Camera(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up, const float fovY, const float aspect,
	       const float near, const float far, std::string name = "");

	glm::mat4 View() const;

	glm::mat4 Projection() const;

	void Forward(float value = 0.05f);
	void Backward(float value = 0.05f);
	void Right(float value = 0.05f);
	void Left(float value = 0.05f);

	glm::vec3 RightVector() const;

	uint32_t Id() const;
	std::string Name() const;
	void SetName(const std::string& name);
	glm::vec3 Eye() const;
	void SetEye(const glm::vec3& eye);
	glm::vec3 Target() const;
	void SetTarget(const glm::vec3& target);
	glm::vec3 Up() const;
	void SetUp(const glm::vec3& up);
	float FovY() const;
	void SetFovY(float fovY);
	float Aspect() const;
	void SetAspect(float aspect);
	float ZNear() const;
	void SetZNear(float near);
	float ZFar() const;
	void SetZFar(float far);
private:
	uint32_t id_;
	std::string name_;

	glm::vec3 eye_;
	glm::vec3 target_;
	glm::vec3 up_;

	float fovY_;
	float aspect_;
	float zNear_;
	float zFar_;

	// static std::atomic<int> nextId_;
};
