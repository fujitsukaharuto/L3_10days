#include "EnemyManager.h"

EnemyManager::EnemyManager() {}

void EnemyManager::Update() {
	// タイマー更新
	if (enemyPopTimer_ < kEnemyPopCoolTime_) {
		enemyPopTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
	} else {
		enemyPopTimer_ = 0.0f;
		AddEnemy();
	}

}

void EnemyManager::Draw() {

}

void EnemyManager::AddEnemy() {

}
