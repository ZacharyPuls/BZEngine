#pragma once

#include "Headers.h"
#include <vector>
#include <atomic>
#include "AABB.h"
#include "Texture.h"

class Mesh
{
public:
	Mesh(std::string name = "");

	uint32_t Id() const;
	std::string Name() const;
	void SetName(const std::string& name);
	std::shared_ptr<GLuint> Vao() const;

	std::shared_ptr<GLuint> AttributeVbo() const;

	std::shared_ptr<GLuint> IndexVbo() const;

	std::shared_ptr<GLuint> BoundingBoxVao() const;
	
	std::shared_ptr<GLuint> BoundingBoxAttributeVbo() const;
	
	GLuint NumIndices() const;
	void SetNumIndices(GLuint numIndices);

	GLuint NumVertices() const;
	void SetNumVertices(GLuint numVertices);

	std::vector<DrawElementsIndirectCommand>& DrawCommands();
	const DrawElementsIndirectCommand& DrawCommand(uint32_t index) const;

	std::vector<uint32_t>& MaterialIDs();
	void SetMaterialIDs(std::vector<uint32_t> materialIDs);

	::AABB& AABB();
	void SetAABB(const ::AABB& aabb);

	uint32_t IconTextureId() const;
	void SetIconTextureId(uint32_t iconTextureId);
private:
	uint32_t id_;
	std::string name_;
	
	std::shared_ptr<GLuint> vao_;
	std::shared_ptr<GLuint> attributeVBO_;
	std::shared_ptr<GLuint> indexVBO_;

	std::shared_ptr<GLuint> boundingBoxVao_;
	std::shared_ptr<GLuint> boundingBoxAttributeVBO_;

	GLuint numIndices_;
	GLuint numVertices_;

	std::vector<DrawElementsIndirectCommand> drawCommands_;
	std::vector<uint32_t> materialIDs_;

	::AABB aabb_;

	uint32_t iconTextureId_;

	// static std::atomic<int> nextId_;
};

