#pragma once

#include "systems/system.h"
#include <reactphysics3d.h>

namespace Engine {

	class Physics : public System
	{
	private:
		static std::shared_ptr<rp3d::DynamicsWorld> m_world;
		rp3d::Vector3 gravity = rp3d::Vector3(0.0, -9.81, 0.0);

	public:
		Physics();
		virtual void start(SystemSignal init, ...) override;
		virtual void stop(SystemSignal close, ...) override;

		void setGravity(rp3d::Vector3 newGrav)
		{
			gravity = newGrav;
		}
		rp3d::Vector3 getGravity()
		{
			return gravity;
		}

	};
}