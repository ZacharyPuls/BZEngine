#include "VAO.h"

VAO::VAO(): arrayId_(0)
{
}

VAO::~VAO() = default;

void VAO::Create()
{
	glGenVertexArrays(1, &arrayId_);
}

void VAO::Bind() const
{
	glBindVertexArray(arrayId_);
}
