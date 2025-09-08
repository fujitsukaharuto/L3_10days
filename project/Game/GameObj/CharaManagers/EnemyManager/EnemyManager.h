#pragma once

#include <vector>
#include <memory>

#include "GameObj/Chara/Enemy/Enemy.h"

class FriendlyManager;

/// <summary>
/// 敵マネージャ
/// </summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager() = default;

	void Update();
	void CheckIsTargetDead();

	void DeleteDeadObject();

	void Draw();

	void AddEnemy(const CharaStatus& status);

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies();

	void SetFri(FriendlyManager* fri);

private:
	std::vector<std::unique_ptr<Enemy>> enemies_;

	// 敵の沸く座標
	Vector3 popPosition_ = { 60.0f,10.0f,0.0f };

	// 沸く位置の乱数用変数
	float maxPopRangeZ_ = -5.0f;
	float minPopRangeZ_ = -25.0f;

	// 敵の沸くクールタイム
	const float kEnemyPopCoolTime_ = 5.0f;
	float enemyPopTimer_ = 0.0f;

private:
	FriendlyManager* fri_;

};