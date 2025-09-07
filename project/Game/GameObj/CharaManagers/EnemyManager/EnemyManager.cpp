#include "EnemyManager.h"

EnemyManager::EnemyManager() {}

void EnemyManager::Update() {
	// 敵のポップ処理
	if (enemyPopTimer_ < kEnemyPopCoolTime_) {
		enemyPopTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
	} else {
		enemyPopTimer_ = 0.0f;
		AddEnemy();
	}

	for (auto& obj : enemies_) {
		obj->Update();
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
	enemies_.push_back(std::move(newObj));
}
