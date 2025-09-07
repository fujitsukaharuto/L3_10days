#include "UsableMinoEditor.h"

#include <filesystem>
#include <format>
#include <unordered_set>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <json.hpp>

#include "Engine/Editor/JsonSerializer.h"

void UsableMinoEditor::Initialize() {
	patternData.clear();
	selectedIndex.reset();

	Deserialize();
}

void UsableMinoEditor::DrawGUI() {
	ImGui::Begin("UsableMinoEditor");

	ImGui::Columns(2);

	// パターンの追加
	if (ImGui::Button("Add Pattern")) {
		auto& mino = patternData.emplace_back();
		mino.name = "NewPattern";
		selectedIndex = static_cast<i32>(patternData.size() - 1);
	}

	// パターンの選択
	for (i32 i = 0; auto& pattern : patternData) {
		if (ImGui::Selectable(std::format("{}##{}", pattern.name, i).c_str(), selectedIndex == i)) {
			selectedIndex = i;
			selectedMinoIndex = std::nullopt;
		}
		++i;
	}

	// 保存
	if (ImGui::Button("Save")) {
		SerializeCheck();
		if (errorMessages.empty()) {
			Serialize();
		}
		else {
			ImGui::OpenPopup("SaveError", ImGuiPopupFlags_NoOpenOverItems);
		}
	}
	SerializeErrorPopup();

	// 削除ボタン
	if (ImGui::Button("Delete##Pattern")) {
		if (selectedIndex.has_value()) {
			patternData.erase(patternData.begin() + selectedIndex.value());
			selectedIndex = std::nullopt;
			selectedMinoIndex = std::nullopt;
		}
	}

	ImGui::NextColumn();

	// パターン内のミノ種選択
	if (selectedIndex.has_value()) {
		UsePatternData& pattern = patternData[selectedIndex.value()];

		if (ImGui::Button("Add Mino")) {
			auto& mino = pattern.editor.emplace_back();
			mino.Initialize();
			selectedMinoIndex = static_cast<i32>(pattern.editor.size() - 1);
		}

		ImGui::InputText("Name", &pattern.name);

		for (i32 i = 0; auto& mino : pattern.editor) {
			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::Selectable(std::format("{}##{}", mino.GetData().name, i).c_str(), selectedMinoIndex == i)) {
				selectedMinoIndex = i;
			}
			++i;
		}
	}

	// 削除ボタン
	if (ImGui::Button("Delete##Mino")) {
		if (selectedIndex.has_value() && selectedMinoIndex.has_value()) {
			patternData[selectedIndex.value()].editor.erase(patternData[selectedIndex.value()].editor.begin() + selectedMinoIndex.value());
			selectedMinoIndex = std::nullopt;
		}
	}

	ImGui::End();

	// 選択中のミノパターンの選択
	if (selectedIndex.has_value() && selectedMinoIndex.has_value()) {
		if (!patternData[selectedIndex.value()].editor[selectedMinoIndex.value()].DrawGUI()) {
			// ウィンドウが閉じられた
			selectedMinoIndex = std::nullopt;
		}
	}
}

void UsableMinoEditor::SerializeCheck() {
	// 名前チェック
	std::unordered_set<std::string> nameSet;
	for (i32 i = 0; const UsePatternData& patternElem : patternData) {
		if (patternElem.name.empty()) {
			errorMessages.emplace_back(std::format("Pattern Index-\'{}\' name is empty.", i));
		}
		else if (nameSet.contains(patternElem.name)) {
			errorMessages.emplace_back(std::format("Pattern name '{}' is duplicated.", patternElem.name));
		}
		else {
			nameSet.emplace(patternElem.name);
		}

		++i;
	}
}

void UsableMinoEditor::SerializeErrorPopup() {
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2{ 400.0f,0.0f }, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("SaveError")) {
		ImGui::TextWrapped("Cannot save due to the following errors:");

		for (const auto& message : errorMessages) {
			ImGui::TextColored(ImVec4{ 1.0f,0.0f,0.0f,1.0f }, message.c_str());
		}

		if (ImGui::Button("OK", ImVec2{ 100.0f,30.0f })) {
			errorMessages.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void UsableMinoEditor::Serialize() const {
	// 名前収集
	std::unordered_set<std::string> nameSet;
	for (const UsePatternData& patternElem : patternData) {
		nameSet.emplace(patternElem.name);
	}

	// 残っているファイルの削除
	for (const auto& directory : std::filesystem::directory_iterator("resource/json/minos/")) {
		std::string filename = directory.path().filename().string();
		std::string name = filename.substr(0, filename.find_last_of('.'));
		if (!nameSet.contains(name)) {
			std::filesystem::remove(directory.path());
		}
	}

	for (const UsePatternData& patternElem : patternData) {
		// 各パターンの保存
		nlohmann::json root;
		root["Name"] = patternElem.name;
		root["Type"] = patternElem.type;
		nlohmann::json minosJson = nlohmann::json::array();
		for (const auto& minos : patternElem.editor) {
			nlohmann::json& minoJson = minosJson.emplace_back();
			const MinoEditor::Data& data = minos.GetData();
			minoJson["Name"] = data.name;
			minoJson["Color"] = { data.color.x,data.color.y,data.color.z };
			minoJson["NumMaxUse"] = data.numMaxUse;
			minoJson["Blocks"] = data.blocks;
			minoJson["GenderType"] = data.gender;
		}

		root["Minos"] = minosJson;

		// ファイル出力
		JsonSerializer::SerializeJsonData(root, "resource/json/minos/" + patternElem.name + ".json");
	}
}

void UsableMinoEditor::Deserialize() {
	for (const auto& directory : std::filesystem::directory_iterator("resource/json/minos/")) {
		nlohmann::json json = JsonSerializer::DeserializeJsonData(directory.path().string());

		UsePatternData data;
		if (json.contains("Name") && json["Name"].is_string()) {
			data.name = json["Name"];
		}
		if (json.contains("Type") && json["Type"].is_number()) {
			data.type = json["Type"];
		}
		if (json.contains("Minos") && json["Minos"].is_array()) {
			for (auto& [_, minoJson] : json["Minos"].items()) {
				MinoEditor::Data minoData;
				if (minoJson.contains("Name") && minoJson["Name"].is_string()) {
					minoData.name = minoJson["Name"];
				}
				if (minoJson.contains("Color") && minoJson["Color"].is_array() && minoJson["Color"].size() == 3) {
					auto& colorArray = minoJson["Color"];
					minoData.color = { colorArray[0].get<float>(), colorArray[1].get<float>(), colorArray[2].get<float>() };
				}
				if (minoJson.contains("NumMaxUse") && minoJson["NumMaxUse"].is_number()) {
					minoData.numMaxUse = minoJson["NumMaxUse"];
				}
				if (minoJson.contains("GenderType") && minoJson["GenderType"].is_number()) {
					minoData.gender = minoJson["GenderType"];
				}
				if (minoJson.contains("Blocks") && minoJson["Blocks"].is_array()) {
					minoData.blocks = minoJson["Blocks"];
				}

				auto& editor = data.editor.emplace_back();
				editor.Initialize();
				editor.SetData(minoData);
			}
		}

		patternData.emplace_back(std::move(data));
	}
}
