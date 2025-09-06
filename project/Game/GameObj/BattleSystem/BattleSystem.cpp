#include "BattleSystem.h"

BattleSystem::BattleSystem(FriendlyManager* fmPtr, EnemyManager* emPtr) {
	friendlyManager_ = fmPtr;
	enemyManager_ = emPtr;

}

void BattleSystem::Update() {


	// 終了判定
	CheckBattleOver();
}

void BattleSystem::CheckBattleOver() {

}
