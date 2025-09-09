#pragma once

#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"
#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

class FriendlyManager;
class EnemyManager;

/// <summary>
/// 戦闘システム
/// </summary>
class BattleSystem {
public:
	BattleSystem(FriendlyManager* fmPtr, EnemyManager* emPtr);
	~BattleSystem() = default;

	void Update();
	void Draw();

	// 戦闘終了判定を取る
	bool CheckBattleOver();

private:

	float friendlyFactoryXLine_;
	float enemyFactryXLine_;

	FriendlyManager* friendlyManager_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;

};