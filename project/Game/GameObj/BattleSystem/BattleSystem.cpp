#include "BattleSystem.h"

BattleSystem::BattleSystem(FriendlyManager* fmPtr, EnemyManager* emPtr) {
	friendlyManager_ = fmPtr;
	enemyManager_ = emPtr;

	friendlyManager_->SetEne(enemyManager_);
	enemyManager_->SetFri(friendlyManager_);
}

void BattleSystem::Update() {

}

bool BattleSystem::CheckGameOver() {
	if (enemyManager_->GetIsWin()) {
		return true;
	}
	return false;
}

bool BattleSystem::CheckBattleOver() {
	if (friendlyManager_->GetIsWin()) {
		return true;
	}
	return false;
}
