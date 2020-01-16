#include "engine_pch.h"
#include "rendering/layers/Layer.h"

namespace Engine
{
	Layer::Layer(const std::string & name)
		: m_name(name)
	{
	}

	const std::string & Layer::getName()
	{
		return m_name;
	}
}