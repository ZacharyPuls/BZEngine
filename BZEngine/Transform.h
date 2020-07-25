#pragma once

#include "Headers.h"
#include <atomic>

class Transform
{
public:
	Transform(std::string name = "");

	Transform(const glm::vec3& scale, const glm::vec3& rotationOrigin, const glm::quat& rotation,
		const glm::vec3& translation, std::string name = "");

	glm::mat4 ModelMatrix() const;
	
	uint32_t Id() const;
	std::string Name() const;
	void SetName(const std::string& name);
	glm::vec3 Scale() const;
	void SetScale(const glm::vec3& scale);
	glm::vec3 RotationOrigin() const;
	void SetRotationOrigin(const glm::vec3& rotationOrigin);
	glm::quat Rotation() const;
	void SetRotation(const glm::quat& rotation);
	glm::vec3 Translation() const;
	void SetTranslation(const glm::vec3& translation);
private:
	uint32_t id_;
	std::string name_;
	
	glm::vec3 scale_;
	glm::vec3 rotationOrigin_;
	glm::quat rotation_;
	glm::vec3 translation_;

	// static std::atomic<int> nextId_;
};
