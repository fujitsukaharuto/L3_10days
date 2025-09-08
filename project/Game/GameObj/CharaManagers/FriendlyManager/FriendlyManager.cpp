#include "FriendlyManager.h"
#include "Engine/DX/FPSKeeper.h"
#include "Random/Random.h"

FriendlyManager::FriendlyManager() {
	friendlies_.clear();
}

void FriendlyManager::Update() {
	for (auto& obj : friendlies_) {
		obj->Update();
	}
}

void FriendlyManager::CheckIsTargetDead() {
	for (auto& obj : friendlies_) {
		obj->CheckIsTargetDead();
	}
}

void FriendlyManager::Draw() {
	for (auto& obj : friendlies_) {
		obj->Draw();
	}
}

void FriendlyManager::DeleteDeadObject() {
	std::erase_if(friendlies_, [](const std::unique_ptr<Friendly>& f) {
		return !f->GetIsAlive();
		});
}

void FriendlyManager::AddFriendly(const CharaStatus& status) {
	// TODO:モデル差し替える　座標はいい感じに設定する パズルの結果に応じて発生するキャラを変えられるようにする
	const float posZ = Random::GetFloat(minPopRangeZ_, maxPopRangeZ_);
	popPosition_.z = posZ;
	std::unique_ptr<Friendly> newObj = std::make_unique<Friendly>(status, popPosition_);
	newObj->SetEne(ene_);
	newObj->SetFri(this);
	friendlies_.push_back(std::move(newObj));
}

void FriendlyManager::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("FriendlyManager")) {
		CharaStatus status;
		status.hp = 10;
		status.name = "cube.obj";
		status.power = 2;
		if (ImGui::Button("PopFriendlyMEN")) {
			status.gender = MAN;
			AddFriendly(status);
		}
		if (ImGui::Button("PopFriendlyWOMEN")) {
			status.gender = WOMAN;
			AddFriendly(status);
		}
	}
#endif // _DEBUG
}

const std::vector<std::unique_ptr<Friendly>>& FriendlyManager::GetFriendlies() {
	return friendlies_;
}

void FriendlyManager::SetEne(EnemyManager* ene) {
	ene_ = ene;
}