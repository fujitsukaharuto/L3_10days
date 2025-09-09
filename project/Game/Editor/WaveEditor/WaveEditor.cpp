#include "WaveEditor.h"

#include "ImGuiManager/ImGuiManager.h"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

WaveEditor::WaveEditor() {
	LoadFile();
}

void WaveEditor::Draw() {
	ImGui::Begin("WaveEditor");
	if (ImGui::Button("SAVE")) {
		SaveFile();
	}
	ImGui::End();
}

std::vector<WaveData> WaveEditor::GetData() {
	return datas_;
}

void WaveEditor::LoadFile() {

}

void WaveEditor::SaveFile() {
	json root;
	root = json::object();

	uint32_t i = 0;
	for (auto& wavedata : datas_) {
		for (size_t j = 0; j < wavedata.enemyPopCycle_.size(); j++) {
			root[std::to_string(i)]["PopCycle"].push_back(wavedata.enemyPopCycle_[j]);
		}
		root[std::to_string(i)]["CoolTime"] = wavedata.enemyPopCoolTime_;
		i++;
	}

	// 書き込むjsonファイルのパスを指定
	std::string filePath = "resource/waveData.json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	ofs.open(filePath);
	// ファイルオープン失敗か？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "WaveEditor", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイル閉じる
	ofs.close();
}
