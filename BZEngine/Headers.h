#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include <fstream>

typedef  struct {
    uint32_t count;
    uint32_t primCount;
    uint32_t firstIndex;
    uint32_t baseVertex;
    uint32_t baseInstance;
} DrawElementsIndirectCommand;