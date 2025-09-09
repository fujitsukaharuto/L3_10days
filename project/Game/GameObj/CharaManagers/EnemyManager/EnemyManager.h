#pragma once

#include <vector>
#include <memory>

#include "GameObj/Chara/Enemy/Enemy.h"

class FriendlyManager;

class WaveEditor;
class EnemyTableEditor;

/// <summary>
/// 敵マネージャ
/// </summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager() = default;

	void Initialzie();
	void Update();
	void CheckIsTargetDead();

	void CSDispatch();

	void DeleteDeadObject();

	void Draw();

	void AddEnemy(const CharaStatus& status);

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies();

	void SetFri(FriendlyManager* fri);
	void SetWaveEditor(WaveEditor* waveEditor);
	void SetEnemyTableEditor(EnemyTableEditor* enemyTableEditor);

	void Win();
	void AddWave();

	bool GetIsWin();

private:
	std::vector<std::unique_ptr<Enemy>> enemies_;

	// 敵の沸く座標
	Vector3 popPosition_ = { 50.0f,10.0f,0.0f };

	// 沸く位置の乱数用変数
	float maxPopRangeZ_ = -5.0f;
	float minPopRangeZ_ = -25.0f;

	// 敵の沸くクールタイム
	float enemyPopTimer_ = 0.0f;

	int32_t enemyPopCount_ = 0;

	bool isWin_ = false;

	int32_t currentWave_ = 0;

private:
	FriendlyManager* fri_;
	EnemyTableEditor* ete_;
	WaveEditor* we_;
};