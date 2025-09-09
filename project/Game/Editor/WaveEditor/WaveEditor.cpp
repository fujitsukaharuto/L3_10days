#include "WaveEditor.h"

#include "ImGuiManager.h"

#include <fstream>
#include <iomanip>   // std::setw
#include <iostream>  // std::cerr

using json = nlohmann::json;

WaveEditor::WaveEditor() {
	LoadFile();
}

void WaveEditor::Draw() {
#ifdef _DEBUG

	ImGui::Begin("WaveEditor");

	// 上部の操作ボタン群
	if (ImGui::Button("Add Wave")) {
		datas_.push_back(WaveData{});
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload")) {
		LoadFile();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		SaveFile();
	}

	ImGui::Separator();

	// 各 Wave の編集 UI
	for (int wi = 0; wi < static_cast<int>(datas_.size()); ++wi) {
		auto& w = datas_[wi];
		ImGui::PushID(wi);

		if (ImGui::CollapsingHeader((std::string("Wave ") + std::to_string(wi)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			// CoolTime
			ImGui::InputFloat("CoolTime", &w.enemyPopCoolTime_);

			// PopCycle 編集
			ImGui::Text("PopCycle");
			// 1行ごと編集 + 追加/削除
			for (int ci = 0; ci < static_cast<int>(w.enemyPopCycle_.size()); ++ci) {
				ImGui::PushID(ci);
				int v = w.enemyPopCycle_[ci];
				if (ImGui::InputInt("##cycle", &v)) {
					w.enemyPopCycle_[ci] = v;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Delete")) {
					w.enemyPopCycle_.erase(w.enemyPopCycle_.begin() + ci);
					ImGui::PopID();
					// erase 後は次の index が詰まるのでループ1回飛ばす
					goto next_wave;
				}
				ImGui::PopID();
			}
			if (ImGui::SmallButton("Add Cycle")) {
				w.enemyPopCycle_.push_back(0);
			}

			// Wave 削除
			ImGui::Separator();
			if (ImGui::Button("Delete Wave")) {
				datas_.erase(datas_.begin() + wi);
				ImGui::PopID();
				// erase 後は vector が詰まるので次の wave へ
				break;
			}
		}

		ImGui::PopID();
	next_wave:;
	}

	ImGui::End();
#endif // _DEBUG
}

std::vector<WaveData> WaveEditor::GetData() {
	return datas_;
}

void WaveEditor::LoadFile() {
	const std::string filePath = "resource/waveData.json";

	std::ifstream ifs(filePath);
	if (!ifs.is_open()) {
		// 初回などファイルが無い場合は空のままでOK
		// 必要ならデフォルトWaveを作る:
		// datas_.push_back(WaveData{});
		return;
	}

	json root;
	try {
		ifs >> root;
	}
	catch (const std::exception& e) {
		std::cerr << "[WaveEditor] JSON parse error: " << e.what() << std::endl;
		return;
	}

	// 期待形式は「オブジェクト」： { "0": {...}, "1": {...}, ... }
	if (!root.is_object()) {
		std::cerr << "[WaveEditor] Invalid JSON: root is not an object." << std::endl;
		return;
	}

	// キーが "0","1","2"... の順に入っている保証はないので、まず最大インデックスを調べる
	int maxIndex = -1;
	for (auto it = root.begin(); it != root.end(); ++it) {
		// 数字文字列以外は無視
		const std::string& key = it.key();
		try {
			int idx = std::stoi(key);
			if (idx > maxIndex) maxIndex = idx;
		}
		catch (...) {
			// 無視
		}
	}

	// インデックス順に読み直す
	std::vector<WaveData> loaded;
	loaded.reserve((maxIndex >= 0) ? (maxIndex + 1) : 0);

	for (int i = 0; i <= maxIndex; ++i) {
		const std::string k = std::to_string(i);
		if (!root.contains(k)) {
			// スキップされたインデックスは空Waveとして埋めてもいいが、ここでは飛ばす
			continue;
		}
		const json& jw = root[k];

		WaveData w{};
		// PopCycle
		if (jw.contains("PopCycle") && jw["PopCycle"].is_array()) {
			for (const auto& v : jw["PopCycle"]) {
				if (v.is_number_integer()) {
					w.enemyPopCycle_.push_back(v.get<int>());
				} else {
					// 数字じゃない要素はスキップ
				}
			}
		}

		// CoolTime（数値なら float に）
		if (jw.contains("CoolTime") && (jw["CoolTime"].is_number_float() || jw["CoolTime"].is_number_integer())) {
			w.enemyPopCoolTime_ = jw["CoolTime"].get<float>();
		} else {
			w.enemyPopCoolTime_ = 0.0f; // 既定値
		}

		loaded.push_back(std::move(w));
	}

	datas_ = std::move(loaded);
}

void WaveEditor::SaveFile() {
	json root = json::object();

	uint32_t i = 0;
	for (auto& wavedata : datas_) {
		// 配列を一旦空で作成してから push_back でも良いが、ここでは直接 push_back
		for (size_t j = 0; j < wavedata.enemyPopCycle_.size(); j++) {
			root[std::to_string(i)]["PopCycle"].push_back(wavedata.enemyPopCycle_[j]);
		}
		root[std::to_string(i)]["CoolTime"] = wavedata.enemyPopCoolTime_;
		i++;
	}

	const std::string filePath = "resource/waveData.json";
	std::ofstream ofs(filePath);
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "WaveEditor", 0);
		assert(0);
		return;
	}

	ofs << std::setw(4) << root << std::endl;
	ofs.close();
}