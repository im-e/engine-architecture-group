#include "engine_pch.h"
#include "systems/Physics.h"

namespace Engine {

	std::shared_ptr<rp3d::DynamicsWorld> Physics::m_world;

	Physics::Physics(){}

	void Physics::start(SystemSignal init, ...){
		m_world.reset(new rp3d::DynamicsWorld (getGravity()));
	}
	void Physics::stop(SystemSignal close, ...){
		m_world.reset();
	}
}