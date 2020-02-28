#include "engine_pch.h"
#include "imgui/ImGuiLayer.h"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include "core/application.h"

namespace Engine
{
	void ImGuiLayer::onAttach()
	{
	}

	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onUpdate(float timestep)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// https://eliasdaler.github.io/using-imgui-with-sfml-pt2/
		ImGui::Begin("Editor window");
		if(ImGui::Button("Log something"))
		{
			LogInfo("Something");
		}
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		glm::vec2 res = glm::vec2(800, 600);
		io.DisplaySize = ImVec2((float)res.x, (float)res.y);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event & e)
	{
	}
}