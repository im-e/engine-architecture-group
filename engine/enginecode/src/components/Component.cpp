#include "engine_pch.h"
#include "components/Component.h"

#include "components/GameObject.h"

namespace Engine
{
	void Component::sendMessage(const ComponentMessage & msg)
	{
		auto & recipients = m_owner->getMap().equal_range(msg.m_msgType);
		for (auto iter = recipients.first; iter != recipients.second; iter++) 
		{
			(*iter).second->receiveMessage(msg);
		}
	}

	void Component::onDetach()
	{
		m_msgsMap.clear();
		m_owner = nullptr;
	}
}