#pragma once

#include "Headers.h"
#include <atomic>

class Instance
{
public:
	Instance() = default;
	Instance(uint32_t meshId, uint32_t transformId, std::string name = "");

	uint32_t Id() const;
	std::string Name() const;
	void SetName(const std::string& name);
	uint32_t MeshId() const;
	void SetMeshId(uint32_t meshId);
	uint32_t TransformId() const;
	void SetTransformId(uint32_t transformId);
	bool RenderBoundingBox() const;
	void SetRenderBoundingBox(bool renderBoundingBox);
private:
	uint32_t id_;
	std::string name_;
	
	uint32_t meshId_;
	uint32_t transformId_;

	bool renderBoundingBox_;
	// static std::atomic<int> nextId_;
};

