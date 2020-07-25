#pragma once

#include "Headers.h"
#include <atomic>
#include <utility>

class Material
{
public:
	Material(std::string name = "") : Material({}, {}, {}, {}, std::move(name))
	{
	}

	Material(const glm::vec3& ambient, const glm::vec3& diffuse,
	         const glm::vec3& specular, const float shininess, std::string name = "");

	uint32_t Id() const;
	std::string Name() const;
	void SetName(const std::string& name);
	glm::vec3 Ambient() const;
	void SetAmbient(const glm::vec3& ambient);
	glm::vec3 Diffuse() const;
	void SetDiffuse(const glm::vec3& diffuse);
	glm::vec3 Specular() const;
	void SetSpecular(const glm::vec3& specular);
	float Shininess() const;
	void SetShininess(float shininess);
	uint32_t DiffuseTextureId() const;
	void SetDiffuseTextureId(uint32_t diffuseTextureId);
	uint32_t NormalTextureId() const;
	void SetNormalTextureId(uint32_t normalTextureId);
private:
	uint32_t id_;
	std::string name_;
	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;
	float shininess_;

	uint32_t diffuseTextureId_;
	uint32_t normalTextureId_;

	// static std::atomic<int> nextId_;
};
