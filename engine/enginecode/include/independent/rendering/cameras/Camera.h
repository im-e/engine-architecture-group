#pragma once

/*! \file Camera.h
\brief Implementation of a base class for a camera
*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Engine
{
	/*! \class Camera
	\brief Base definition of a camera in a scene
	*/
	class Camera
	{
	protected:
		glm::mat4 m_projection; //!< Projection matrix
		glm::mat4 m_view; //!< View matrix
		glm::mat4 m_ViewProjection; //!< View-Projection matrix
		float m_cutOff = glm::cos(glm::radians(12.5f));

	public:
		//! Updates view of a camera
		virtual void updateViewMatrix() = 0;
		//! Gets projection of a camera \return projection
		const glm::mat4& getProjection() { return m_projection; }
		//! Gets view of a camera \return view
		const glm::mat4& getView() { return m_view; }
		//! Gets view-projection of a camera \return VP
		const glm::mat4& getViewProjection() { return m_ViewProjection; }

		const float& getCutOff() { return m_cutOff; }

		//! Sets position of a camera \param pos new position
		virtual void setPosition(const glm::vec3 pos) = 0;
		//! Gets current position of a camera \return current position
		virtual glm::vec3 getPosition() = 0;

		/*! Creates 2D camera 
		\param left left boundary of a view 
		\param right right boundary of a view 
		\param bottom bottom boundary of a view 
		\param top top boundary of a view
		*/
		static Camera* create2D(float left, float right, float bottom, float top);
		/*! Creates 3D camera
		\param fov field of view
		\param aspectRatio resolution
		\param nearClip closest point to a camera drawing will occur
		\param farclip furtherst point from a camera drawing will occur
		*/
		static Camera* create3D(float fov, float aspectRatio, float nearClip, float farClip);
	};
}