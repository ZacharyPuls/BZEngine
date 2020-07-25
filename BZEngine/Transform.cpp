#include "Transform.h"

Transform::Transform(std::string name) : id_(0), name_(std::move(name))
{
}

Transform::Transform(const glm::vec3& scale, const glm::vec3& rotationOrigin, const glm::quat& rotation,
                     const glm::vec3& translation, std::string name)
	: id_(0), name_(std::move(name)), scale_(scale),
	  rotationOrigin_(rotationOrigin),
	  rotation_(rotation),
	  translation_(translation)
{
}

glm::mat4 Transform::ModelMatrix() const
{
	auto modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(-RotationOrigin()) * modelMatrix;
	modelMatrix = glm::mat4_cast(Rotation()) * modelMatrix;
	modelMatrix = glm::translate(RotationOrigin()) * modelMatrix;
	modelMatrix = glm::scale(Scale()) * modelMatrix;
	return glm::translate(Translation()) * modelMatrix;
}

uint32_t Transform::Id() const
{
	return id_;
}

std::string Transform::Name() const
{
	return name_;
}

void Transform::SetName(const std::string& name)
{
	name_ = name;
}

glm::vec3 Transform::Scale() const
{
	return scale_;
}

void Transform::SetScale(const glm::vec3& scale)
{
	scale_ = scale;
}

glm::vec3 Transform::RotationOrigin() const
{
	return rotationOrigin_;
}

void Transform::SetRotationOrigin(const glm::vec3& rotationOrigin)
{
	rotationOrigin_ = rotationOrigin;
}

glm::quat Transform::Rotation() const
{
	return rotation_;
}

void Transform::SetRotation(const glm::quat& rotation)
{
	rotation_ = rotation;
}

glm::vec3 Transform::Translation() const
{
	return translation_;
}

void Transform::SetTranslation(const glm::vec3& translation)
{
	translation_ = translation;
}
