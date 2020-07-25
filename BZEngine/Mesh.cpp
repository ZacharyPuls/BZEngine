#include "Mesh.h"

Mesh::Mesh(std::string name) : id_(0), name_(std::move(name)),
                               vao_(new GLuint(), [](auto id) { glDeleteVertexArrays(1, id); }),
                               attributeVBO_(new GLuint(), [](auto id) { glDeleteBuffers(1, id); }),
                               indexVBO_(new GLuint(), [](auto id) { glDeleteBuffers(1, id); }),
                               boundingBoxVao_(new GLuint(), [](auto id) { glDeleteVertexArrays(1, id); }),
                               boundingBoxAttributeVBO_(new GLuint(), [](auto id) { glDeleteBuffers(1, id); }),
                               numIndices_(0),
                               numVertices_(0)
{
	glGenVertexArrays(1, vao_.get());
	glGenBuffers(1, attributeVBO_.get());
	glGenBuffers(1, indexVBO_.get());

	glGenVertexArrays(1, boundingBoxVao_.get());
	glGenBuffers(1, boundingBoxAttributeVBO_.get());
}

uint32_t Mesh::Id() const
{
	return id_;
}

std::string Mesh::Name() const
{
	return name_;
}

void Mesh::SetName(const std::string& name)
{
	name_ = name;
}

std::shared_ptr<GLuint> Mesh::Vao() const
{
	return vao_;
}

std::shared_ptr<GLuint> Mesh::AttributeVbo() const
{
	return attributeVBO_;
}

std::shared_ptr<GLuint> Mesh::IndexVbo() const
{
	return indexVBO_;
}

std::shared_ptr<GLuint> Mesh::BoundingBoxVao() const
{
	return boundingBoxVao_;
}

std::shared_ptr<GLuint> Mesh::BoundingBoxAttributeVbo() const
{
	return boundingBoxAttributeVBO_;
}

GLuint Mesh::NumIndices() const
{
	return numIndices_;
}

void Mesh::SetNumIndices(GLuint numIndices)
{
	numIndices_ = numIndices;
}

GLuint Mesh::NumVertices() const
{
	return numVertices_;
}

void Mesh::SetNumVertices(GLuint numVertices)
{
	numVertices_ = numVertices;
}

std::vector<DrawElementsIndirectCommand>& Mesh::DrawCommands()
{
	return drawCommands_;
}

const DrawElementsIndirectCommand& Mesh::DrawCommand(const uint32_t index) const
{
	return drawCommands_[index];
}

std::vector<uint32_t>& Mesh::MaterialIDs()
{
	return materialIDs_;
}

void Mesh::SetMaterialIDs(std::vector<uint32_t> materialIDs)
{
	materialIDs_ = materialIDs;
}

::AABB& Mesh::AABB()
{
	return aabb_;
}

void Mesh::SetAABB(const ::AABB& aabb)
{
	aabb_ = aabb;
}

uint32_t Mesh::IconTextureId() const
{
	return iconTextureId_;
}

void Mesh::SetIconTextureId(uint32_t iconTextureId)
{
	iconTextureId_ = iconTextureId;
}
