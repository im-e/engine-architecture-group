#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Engine {

	struct AudioGeometryDefinition {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
		float directOcculsion = 1.0f;
		float reverbOcculsion = 0.3f;
	};

}