#pragma once

#include "rendering/layers/Layer.h"
#include "data/json/JsonLayer.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:
		JsonLayer* m_layer;

		bool m_gameObjectWindow;
		bool m_manageCompWindow;

		std::string m_name;
		const std::string& m_filepath;

		std::vector<std::function<void(JsonLayer*)>> m_functions;
		std::vector<std::string> m_shadersNames;
		std::vector<std::string> m_jsonModelNames;
		std::vector<std::string> m_assimpModelNames;
		std::vector<std::string> m_texturesNames;

	public:
		ImGuiLayer(const std::string& name, const std::string& path, std::shared_ptr<Layer> layer) : m_filepath(path), Layer(name)
		{
			m_layer = dynamic_cast<Engine::JsonLayer*>(layer.get());
		};

		// Inherited via Layer
		void onAttach() override;
		void onDetach() override;
		void onUpdate(float timestep) override;
		void onEvent(Event & e) override;

		void addGO();
		void manageComponents();

		void addImGuiFunction(std::function<void(JsonLayer*)> func);
		std::vector<std::function<void(JsonLayer*)>>& getFunctions();

		std::vector<std::string>& getShaderNames();
		std::vector<std::string>& getJsonModelsNames();
		std::vector<std::string>& getAssimpModelsNames();
		std::vector<std::string>& getTexturesNames();

		JsonLayer* getJsonLayer();
		std::string& getGOName();
	};
}