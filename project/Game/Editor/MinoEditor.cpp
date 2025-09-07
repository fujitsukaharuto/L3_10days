#include "MinoEditor.h"

#include <format>

#include <imgui.h>
#include <imgui_stdlib.h>

void MinoEditor::Initialize() {
	data.blocks.resize(5, std::vector<int>(5));
	data.name = "NewMino";
}

bool MinoEditor::DrawGUI() {
	bool isActiveWindow = true;
	ImGui::Begin("MinoEditor", &isActiveWindow);

	ImGui::InputText("Name", &data.name);

	ImGui::ColorEdit3("Color", &data.color.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB);

	ImGui::InputInt("MaxUse", &data.numMaxUse, 1, 1);

	ImGui::Separator();
	ImGui::Text("Blocks");

	for (auto& row : data.blocks) {
		for (auto& block : row) {
			bool isActive = block & 0b1;
			if (ImGui::Selectable(std::format("##{}", (void*)&block).c_str(), &isActive, 0, ImVec2{ 25.0f, 25.0f })) {
				block ^= 0b1;
			}
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	ImGui::End();

	return isActiveWindow;
}
