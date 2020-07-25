#include "Instance.h"

#include <utility>

Instance::Instance(const uint32_t meshId, const uint32_t transformId, std::string name)
	: id_(0),
	  name_(std::move(name)),
	  meshId_(meshId),
	  transformId_(
		  transformId),
	  renderBoundingBox_(false)
{
}

uint32_t Instance::Id() const
{
	return id_;
}

void Instance::SetName(const std::string& name)
{
	name_ = name;
}

void Instance::SetMeshId(uint32_t meshId)
{
	meshId_ = meshId;
}

void Instance::SetTransformId(uint32_t transformId)
{
	transformId_ = transformId;
}

bool Instance::RenderBoundingBox() const
{
	return renderBoundingBox_;
}

void Instance::SetRenderBoundingBox(bool renderBoundingBox)
{
	renderBoundingBox_ = renderBoundingBox;
}

std::string Instance::Name() const
{
	return name_;
}

uint32_t Instance::MeshId() const
{
	return meshId_;
}

uint32_t Instance::TransformId() const
{
	return transformId_;
}
