#pragma once

#include <vector>

#include "GameObj/Chara/Friendly/Friendly.h"
#include "GameObj/Factory/Factory.h"

class EnemyManager;

/// <summary>
/// 味方マネージャ
/// </summary>
class FriendlyManager {
public:
	FriendlyManager();
	~FriendlyManager() = default;

	void Update();
	void CheckIsTargetDead();

	void CSDispatch();

	void Draw();

	void DeleteDeadObject();

	void AddFriendly(const CharaStatus& status);

	void DebugGUI();

	const std::vector<std::unique_ptr<Friendly>>& GetFriendlies();

	void SetEne(EnemyManager* ene);

	void Win();

	bool GetIsWin();
private:
	std::vector<std::unique_ptr<Friendly>> friendlies_;

	// 敵の沸く座標
	Vector3 popPosition_ = { 20.0f,10.0f,-15.0f };

	// 沸く位置の乱数用変数
	float maxPopRangeZ_ = -5.0f;
	float minPopRangeZ_ = -25.0f;

	// 勝利フラグ
	bool isWin_ = false;

	// 工場
	std::unique_ptr<Factory> factory_;

private:
	EnemyManager* ene_;

};