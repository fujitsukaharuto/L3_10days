#include "EnemyTableEditor.h"

#include "ImGuiManager/ImGuiManager.h"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

EnemyTableEditor::EnemyTableEditor() {
	LoadFile();
}

void EnemyTableEditor::Draw() {
	ImGui::Begin("EnemyTableEditor");

	// 敵の追加ボタン
	if (ImGui::Button("AddEnemy")) {
		enemyTable_.push_back(EnemyData{});
	}

	// インデックス管理用
	int index = 0;
	for (auto& data : enemyTable_) {
		ImGui::PushID(index); // 各行を区別するためのID

		ImGui::Separator();
		ImGui::Text("Enemy %d", index);

		// 名前入力
		char buf[128];
		std::snprintf(buf, sizeof(buf), "%s", data.name.c_str());
		if (ImGui::InputText("Name", buf, sizeof(buf))) {
			data.name = buf;
		}

		// 性別選択（enum → int）
		int genderInt = static_cast<int>(data.gender);
		const char* genderItems[] = { "MAN", "WOMAN" };
		if (ImGui::Combo("Gender", &genderInt, genderItems, IM_ARRAYSIZE(genderItems))) {
			data.gender = static_cast<Gender>(genderInt);
		}

		// 数値入力
		ImGui::InputInt("Power", &data.power);
		ImGui::InputInt("HP", &data.hp);

		// 削除ボタン
		if (ImGui::Button("Delete")) {
			enemyTable_.erase(enemyTable_.begin() + index);
			ImGui::PopID();
			break; // eraseしたのでループ抜ける
		}

		ImGui::PopID();
		++index;
	}

	// 保存ボタン
	if (ImGui::Button("SaveFile")) {
		SaveFile();
	}

	ImGui::End();
}

std::vector<EnemyData> EnemyTableEditor::GetTable() {
	return enemyTable_;
}

void EnemyTableEditor::LoadFile() {
	const std::string filePath = "resource/EnemyTable.json";

	std::ifstream ifs(filePath);
	if (!ifs.is_open()) {
		return;
	}

	json root;
	try {
		ifs >> root;
	}
	catch (const std::exception& e) {
		std::cerr << "[EnemyTableEditor] JSON parse error: " << e.what() << std::endl;
		return;
	}

	// "enemies" 配列を期待。無ければ何もしない
	if (!root.contains("enemies") || !root["enemies"].is_array()) {
		std::cerr << "[EnemyTableEditor] Invalid JSON: 'enemies' array not found." << std::endl;
		return;
	}

	enemyTable_.clear();
	for (const auto& je : root["enemies"]) {
		EnemyData data{};

		if (je.contains("name") && je["name"].is_string()) {
			data.name = je["name"].get<std::string>();
		} else {
			data.name = "NONAME";
		}

		if (je.contains("gender") && je["name"].is_string()) {
			if (je["gender"].get<std::string>() == "MAN") {
				data.gender = Gender::MAN;
			} else {
				data.gender = Gender::WOMAN;
			}
		} else {
			data.gender = Gender::MAN;
		}

		if (je.contains("power") && je["power"].is_number_integer()) {
			data.power = je["power"].get<int32_t>();
		} else {
			data.power = 10;
		}

		if (je.contains("hp") && je["hp"].is_number_integer()) {
			data.hp = je["hp"].get<int32_t>();
		} else {
			data.hp = 10;
		}

		enemyTable_.push_back(std::move(data));
	}
}


void EnemyTableEditor::SaveFile() {
	const std::string filePath = "resource/EnemyTable.json";

	json root;
	root["enemies"] = json::array();

	for (const auto& e : enemyTable_) {
		json je;
		je["name"] = e.name;
		je["gender"] = GenderToString(e.gender); // 文字列で保存
		je["power"] = e.power;
		je["hp"] = e.hp;
		root["enemies"].push_back(std::move(je));
	}

	std::ofstream ofs;
	ofs.open(filePath);
	// ファイルオープン失敗か？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "EnemyTableEditor", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイル閉じる
	ofs.close();
}

std::string EnemyTableEditor::GenderToString(Gender g) {
	switch (g) {
	case Gender::MAN:   return "MAN";
	case Gender::WOMAN: return "WOMAN";
	default:            return "MAN";
	}
}

Gender EnemyTableEditor::GenderFromString(const std::string& s) {
	if (s == "WOMAN") return Gender::WOMAN;
	// 既定は MAN
	return Gender::MAN;
}
