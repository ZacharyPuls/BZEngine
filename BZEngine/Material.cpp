#include "Material.h"

#include <utility>

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse,
                   const glm::vec3& specular, const float shininess, std::string name)
	: id_(0), name_(std::move(name)),
	  ambient_(ambient),
	  diffuse_(diffuse),
	  specular_(specular),
	  shininess_(shininess),
	  diffuseTextureId_(-1),
	  normalTextureId_(-1)
{
}

uint32_t Material::Id() const
{
	return id_;
}

std::string Material::Name() const
{
	return name_;
}

void Material::SetName(const std::string& name)
{
	name_ = name;
}

glm::vec3 Material::Ambient() const
{
	return ambient_;
}

void Material::SetAmbient(const glm::vec3& ambient)
{
	ambient_ = ambient;
}

glm::vec3 Material::Diffuse() const
{
	return diffuse_;
}

void Material::SetDiffuse(const glm::vec3& diffuse)
{
	diffuse_ = diffuse;
}

glm::vec3 Material::Specular() const
{
	return specular_;
}

void Material::SetSpecular(const glm::vec3& specular)
{
	specular_ = specular;
}

float Material::Shininess() const
{
	return shininess_;
}

void Material::SetShininess(float shininess)
{
	shininess_ = shininess;
}

uint32_t Material::DiffuseTextureId() const
{
	return diffuseTextureId_;
}

void Material::SetDiffuseTextureId(uint32_t diffuseTextureId)
{
	diffuseTextureId_ = diffuseTextureId;
}

uint32_t Material::NormalTextureId() const
{
	return normalTextureId_;
}

void Material::SetNormalTextureId(uint32_t normalTextureId)
{
	normalTextureId_ = normalTextureId;
}
