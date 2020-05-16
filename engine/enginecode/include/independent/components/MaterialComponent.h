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

		std::string m_typeName; //! Material's type name (json/assimp)
		std::string m_shaderName; //! Shader's name associated with material
		std::string m_modelName; //! Model's name associated with material
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

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		void onDetach() override
		{
			auto iter = m_owner->getMap().begin();
			while (iter != m_owner->getMap().end())
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
				else
				{
					++iter;
				}
			}
		}

		//! Sets shader name \param name new shader name
		inline void setShaderName(std::string name) { m_shaderName = name; }

		//! Sets model name \param name new model name
		inline void setModelName(std::string name) { m_modelName = name; }

		//! Sets type name \param name new type name
		inline void setTypeName(std::string name) { m_typeName = name; }

		//! Gets shader name \return current shader name
		inline std::string& getShaderName() { return m_shaderName; }

		//! Gets model name \return current model name
		inline std::string& getModelName() { return m_modelName; }

		//! Gets type name \return current type name
		inline std::string& getTypeName() { return m_typeName; }
	};
}