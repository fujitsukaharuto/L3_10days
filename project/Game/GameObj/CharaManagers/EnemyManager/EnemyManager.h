#pragma once

#include <vector>
#include <memory>

#include "GameObj/Chara/Enemy/Enemy.h"

/// <summary>
/// 敵マネージャ
/// </summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager() = default;

	void Update();
	void Draw();

	void AddEnemy();

private:
	std::vector<std::unique_ptr<Enemy>> enemies_;

	// 敵の沸く座標
	const Vector3 kPopPosition_ = { 60.0f,10.0f,0.0f };

	// 敵の沸くクールタイム
	const float kEnemyPopCoolTime_ = 5.0f;
	float enemyPopTimer_ = 0.0f;

};