#pragma once

/*! \file Physics.h
\brief Implements physics system
*/

#include "systems/system.h"
#include <reactphysics3d.h>

namespace Engine 
{
	/*! \class Physics
	\brief Definition for physics system.
	*/
	class Physics : public System
	{
	private:
		//! Pointer to ReactPhysics3D world
		static std::shared_ptr<rp3d::DynamicsWorld> m_world;
		//! Gravity values
		rp3d::Vector3 gravity = rp3d::Vector3(0.0, -1.0, 0.0);

	public:
		//! Default constructor
		Physics();
		// Inherited via System
		virtual void start(SystemSignal init, ...) override;
		virtual void stop(SystemSignal close, ...) override;

		//! Gets physics world \return physics world
		std::shared_ptr<rp3d::DynamicsWorld>& getWorld()
		{
			return m_world;
		}

		//! Sets gravity \param newGrav new gravity
		void setGravity(rp3d::Vector3 newGrav)
		{
			gravity = newGrav;
		}
		
		//! Gets gravity \return current gravity
		rp3d::Vector3 getGravity()
		{
			return gravity;
		}

	};
}