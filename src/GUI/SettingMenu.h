#pragma once
#include "GameMenu.h"
#include <string>

namespace NCL {
	namespace CSC8503 {
		class SettingMenu : public GameMenu {
		public:
			SettingMenu() {};
			virtual ~SettingMenu() {};
			virtual void Draw() override
			{
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(360, 280), ImGuiCond_Always);

				if (!ImGui::Begin("SetingMenu", NULL, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return;
				}
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Button("Audio");

				ImGui::End();
			}
		protected:
		};

	}
}