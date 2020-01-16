#pragma once

/*! \file MaterialComponent.h
\brief Implements a component responsible for providing materials for a GO
*/

#include "Component.h"
#include "rendering/Material.h"


namespace Engine
{
	/*! \class MaterialComponent
	\brief Defines component providing materials for GOs.
	
		Inherits from Component
	*/
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_material; //!< Material associated with a GO
	public:
		//! Custom constructor \param material material of a GO
		MaterialComponent(const std::shared_ptr<Material>& material) : m_material(material) {};
		//! Gets material associated with a GO \return material of a GO
		inline std::shared_ptr<Material> getMaterial() { return m_material; }

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;

			m_possibleMessages = { ComponentMessageType::UniformSet };

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg)
				{
				case ComponentMessageType::UniformSet:				
					m_msgsMap[msg] = [this](std::any data)
					{
						std::pair<std::string, void*> d = std::any_cast<std::pair<std::string, void*>>(data);
						m_material->setShaderDataElement(d.first, d.second);
						return true;
					};
					break;
				}
			}
		}
	};
}