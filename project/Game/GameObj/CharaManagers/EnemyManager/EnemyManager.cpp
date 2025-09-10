#include "EnemyManager.h"
#include "Game/Editor/WaveEditor/WaveEditor.h"
#include "Game/Editor/EnemyTableEditor/EnemyTableEditor.h"
#include "Random/Random.h"
#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"

EnemyManager::EnemyManager() {
	enemies_.clear();

	factory_ = std::make_unique<Factory>(popPosition_, false);
}

void EnemyManager::Update() {
	factory_->Update();

	if (!we_)return;
	if (!ete_)return;

	if (we_->GetData().empty()) return;
	if (ete_->GetTable().empty())return;

	WaveData currentWaveData = we_->GetData()[currentWave_];
	// 敵のポップ処理
	if (enemyPopTimer_ < currentWaveData.enemyPopCoolTime_) {
		enemyPopTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
	} else {

		CharaStatus status;
		EnemyData data = ete_->GetTable()[currentWaveData.enemyPopCycle_[enemyPopCount_]];
		status.gender = data.gender;
		status.hp = data.hp;
		status.maxHp = data.hp;
		status.power = data.power;
		status.name = data.name;

		AddEnemy(status);
		enemyPopTimer_ = 0.0f;
		enemyPopCount_++;
		if (enemyPopCount_ == currentWaveData.enemyPopCycle_.size()) {
			enemyPopCount_ = 0;
		}
	}

	for (auto& obj : enemies_) {
		obj->Update();
	}
}

void EnemyManager::CheckIsTargetDead() {
	for (auto& obj : enemies_) {
		obj->CheckIsTargetDead();
	}
}

void EnemyManager::CSDispatch() {
	for (auto& obj : enemies_) {
		obj->CSDispatch();
	}
}

void EnemyManager::Draw() {
	factory_->Draw();

	for (auto& obj : enemies_) {
		obj->Draw();
	}
}

void EnemyManager::DeleteDeadObject() {
	std::erase_if(enemies_, [](const std::unique_ptr<Enemy>& f) {
		return !f->GetIsAlive();
		});
}

void EnemyManager::AddEnemy(const CharaStatus& status) {
	// TODO:モデル差し替える　座標はいい感じに設定する パズルの結果に応じて発生するキャラを変えられるようにする
	const float posZ = Random::GetFloat(minPopRangeZ_, maxPopRangeZ_);
	popPosition_.z = posZ;
	std::unique_ptr<Enemy> newObj = std::make_unique<Enemy>(status,
		Vector3(popPosition_.x - 1.0f, popPosition_.y, popPosition_.z));
	newObj->SetFri(fri_);
	newObj->SetEne(this);
	enemies_.push_back(std::move(newObj));
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::GetEnemies() {
	return enemies_;
}

void EnemyManager::SetFri(FriendlyManager* fri) {
	fri_ = fri;
}

void EnemyManager::SetWaveEditor(WaveEditor* waveEditor) {
	we_ = waveEditor;
}

void EnemyManager::SetEnemyTableEditor(EnemyTableEditor* enemyTableEditor) {
	ete_ = enemyTableEditor;
}

void EnemyManager::AllKill() {
	for (auto& obj : enemies_) {
		obj->SetIsAlive(false);
	}
}

void EnemyManager::Win() {
	isWin_ = true;
}

void EnemyManager::AddWave() {
	currentWave_++;
	fri_->AllKill();
	enemies_.clear();
}

bool EnemyManager::GetIsWin() {
	return isWin_;
}

int32_t EnemyManager::GetCurrentWave() {
	return currentWave_;
}
