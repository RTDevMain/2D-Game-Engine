#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "glm/ext/vector_float2.hpp"
#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;
};





#endif