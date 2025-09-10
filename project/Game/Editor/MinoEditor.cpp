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

	if (ImGui::RadioButton("Man", data.gender == GenderType::Man)) data.gender = GenderType::Man;
	ImGui::SameLine();
	if (ImGui::RadioButton("Woman", data.gender == GenderType::Woman)) data.gender = GenderType::Woman;

	ImGui::Separator();
	ImGui::Text("Blocks");

	ImGui::BeginTable("BlocksTable", 5, ImGuiTableFlags_Borders, ImVec2{ 25 * 5,25 * 5 });
	for (auto& row : data.blocks) {
		for (auto& block : row) {
			bool isActive = block & 0b1;
			ImGui::TableNextColumn();
			if (ImGui::Selectable(std::format("##{}", (void*)&block).c_str(), &isActive, 0, ImVec2{ 25.0f, 25.0f })) {
				block ^= 0b1;
			}
		}
	}
	ImGui::EndTable();

	ImGui::End();

	return isActiveWindow;
}
