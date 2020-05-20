#pragma once

/*! \file audioGeometryDefinition.h
\brief Defines and audio geometry
*/

#include <vector>
#include <glm/glm.hpp>

namespace Engine 
{
	/*! \struct AudioGeometryDefinition
	\brief Defines a geometry
	*/
	struct AudioGeometryDefinition 
	{
		std::vector<float> vertices; //!< Collection of vertices
		std::vector<unsigned int> indices; //!< Collection of indices
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f); //!< Position
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f); //!< Scale
		float directOcculsion = 1.0f; //!< Direct sound occlusion
		float reverbOcculsion = 0.3f; //!< Direct reverb occlusion
	};

}