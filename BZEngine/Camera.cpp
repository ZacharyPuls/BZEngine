#include "Camera.h"

Camera::Camera(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up, const float fovY,
               const float aspect, const float near, const float far, std::string name)
	: id_(0),
	  name_(std::move(name)),
	  eye_(eye),
	  target_(target),
	  up_(up),
	  fovY_(fovY),
	  aspect_(aspect),
	  zNear_(near),
	  zFar_(far)
{
}

glm::mat4 Camera::View() const
{
	return glm::lookAt(eye_, target_, up_);
}

glm::mat4 Camera::Projection() const
{
	return glm::perspective(fovY_, aspect_, zNear_, zFar_);
}

void Camera::Forward(const float value)
{
	eye_.z -= value;
	target_.z -= value;
}

void Camera::Backward(const float value)
{
	eye_.z += value;
	target_.z += value;
}

void Camera::Right(const float value)
{
	eye_.x += value;
	target_.x += value;
}

void Camera::Left(const float value)
{
	eye_.x -= value;
	target_.x -= value;
}

glm::vec3 Camera::RightVector() const
{
	const auto forwardVector = glm::normalize(Eye() - Target());
	return glm::cross(Up(), forwardVector);
}

uint32_t Camera::Id() const
{
	return id_;
}

std::string Camera::Name() const
{
	return name_;
}

void Camera::SetName(const std::string& name)
{
	name_ = name;
}

glm::vec3 Camera::Eye() const
{
	return eye_;
}

void Camera::SetEye(const glm::vec3& eye)
{
	eye_ = eye;
}

glm::vec3 Camera::Target() const
{
	return target_;
}

void Camera::SetTarget(const glm::vec3& target)
{
	target_ = target;
}

glm::vec3 Camera::Up() const
{
	return up_;
}

void Camera::SetUp(const glm::vec3& up)
{
	up_ = up;
}

float Camera::FovY() const
{
	return fovY_;
}

void Camera::SetFovY(float fovY)
{
	fovY_ = fovY;
}

float Camera::Aspect() const
{
	return aspect_;
}

void Camera::SetAspect(float aspect)
{
	aspect_ = aspect;
}

float Camera::ZNear() const
{
	return zNear_;
}

void Camera::SetZNear(float near)
{
	zNear_ = near;
}

float Camera::ZFar() const
{
	return zFar_;
}

void Camera::SetZFar(float far)
{
	zFar_ = far;
}
