#pragma once

#include "Headers.h"

class AABB
{
public:
	AABB() = default;
	
	AABB(const glm::vec3& x0, const glm::vec3& x1)
		: x0_(x0),
		  x1_(x1)
	{
	}

	glm::vec3 X0() const
	{
		return x0_;
	}

	void SetX0(const glm::vec3& x0)
	{
		x0_ = x0;
	}

	glm::vec3 X1() const
	{
		return x1_;
	}

	void SetX1(const glm::vec3& x1)
	{
		x1_ = x1;
	}

	bool RayIntersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::mat4 model)
	{
		// rayOrigin = glm::vec3(model * glm::vec4(rayOrigin, 1.0f));
		// rayDirection = glm::normalize(glm::vec3(model * glm::vec4(rayDirection, 1.0f)));
		auto dirFrac = 1.0f / rayDirection;
		auto t135 = (x0_ - rayOrigin) * dirFrac;
		auto t246 = (x1_ - rayOrigin) * dirFrac;
		auto tMin = glm::max(glm::max(glm::min(t135.x, t246.x), glm::min(t135.y, t246.y)), glm::min(t135.z, t246.z));
		auto tMax = glm::min(glm::min(glm::max(t135.x, t246.x), glm::max(t135.y, t246.y)), glm::max(t135.z, t246.y));
		return tMax >= 0 && tMin <= tMax;
		/*
		auto worldspace = glm::vec3(model[3].x, model[3].y , model[3].z);
		auto delta = worldspace - rayOrigin;
		auto xAxis = glm::vec3(model[0].x, model[0].y, model[0].z);
		glm::intersectRayPlane()
		auto e = glm::dot(xAxis, delta);
		auto f = glm::dot(rayDirection, xAxis);
		auto t1 = e + x0_.x;
		auto t2 = e + x1_.x;
		if (glm::distance(f, 0.0f) > 0.01f)
		{
			t1 /= f;
			t2 /= f;
		}

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}
		*/
	}

private:
	glm::vec3 x0_;
	glm::vec3 x1_;
};