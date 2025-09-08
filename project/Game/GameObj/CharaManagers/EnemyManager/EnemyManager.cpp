#include "EnemyManager.h"

EnemyManager::EnemyManager() {}

void EnemyManager::Update() {
	// 敵のポップ処理
	if (enemyPopTimer_ < kEnemyPopCoolTime_) {
		enemyPopTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
	} else {
		enemyPopTimer_ = 0.0f;

		// TODO:ポップテーブルから沸きの設定をおこなえるようにする
		CharaStatus status;
		status.hp = 10;
		status.name = "cube.obj";
		status.power = 2;
		status.gender = MEN;

		AddEnemy(status);
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

void EnemyManager::Draw() {
	for (auto& obj : enemies_) {
		obj->Draw();
	}
}

void EnemyManager::AddEnemy(const CharaStatus& status) {
	// TODO:モデル差し替える　座標はいい感じに設定する パズルの結果に応じて発生するキャラを変えられるようにする
	std::unique_ptr<Enemy> newObj = std::make_unique<Enemy>(status, kPopPosition_);
	newObj->SetFri(fri_);
	enemies_.push_back(std::move(newObj));
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::GetEnemies() {
	return enemies_;
}

void EnemyManager::SetFri(FriendlyManager* fri) {
	fri_ = fri;
}
