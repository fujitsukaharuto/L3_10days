#include "FriendlyManager.h"
#include "Engine/DX/FPSKeeper.h"

FriendlyManager::FriendlyManager() {
	friendlies_.clear();
}

void FriendlyManager::Update() {
	for (auto& obj : friendlies_) {
		obj->Update();
	}

}

void FriendlyManager::Draw() {
	for (auto& obj : friendlies_) {
		obj->Draw();
	}
}

void FriendlyManager::AddFriendly(const CharaStatus& status) {
	// TODO:モデル差し替える　座標はいい感じに設定する パズルの結果に応じて発生するキャラを変えられるようにする
	std::unique_ptr<Friendly> newObj = std::make_unique<Friendly>(status, kPopPosition_);
	friendlies_.push_back(std::move(newObj));
}

void FriendlyManager::DebugGUI() {
	if (ImGui::CollapsingHeader("FriendlyManager")) {
		CharaStatus status;
		status.hp = 10;
		status.name = "cube.obj";
		status.power = 2;
		if (ImGui::Button("PopFriendlyMEN")) {
			status.gender = MEN;
			AddFriendly(status);
		}
		if (ImGui::Button("PopFriendlyWOMEN")) {
			status.gender = WOMEN;
			AddFriendly(status);
		}
	}
}