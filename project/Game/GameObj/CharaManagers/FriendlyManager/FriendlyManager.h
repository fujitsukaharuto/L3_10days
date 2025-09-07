#pragma once

#include <vector>

#include "GameObj/Chara/Friendly/Friendly.h"

/// <summary>
/// 味方マネージャ
/// </summary>
class FriendlyManager {
public:
	FriendlyManager();
	~FriendlyManager() = default;

	void Update();
	void Draw();

	void AddFriendly(const CharaStatus& status);

	void DebugGUI();

private:
	std::vector<std::unique_ptr<Friendly>> friendlies_;

	// 敵の沸く座標
	const Vector3 kPopPosition_ = { 22.0f,10.0f,0.0f };

};