#include "engine_pch.h"
#include "systems/ImGuiSystem.h"

namespace Engine
{
	void ImGuiSystem::start(SystemSignal init, ...)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::StyleColorsDark();
	}

	void ImGuiSystem::stop(SystemSignal close, ...)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}
}