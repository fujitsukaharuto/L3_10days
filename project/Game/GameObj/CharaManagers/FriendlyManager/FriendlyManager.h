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

	void AddFriendly();

private:
	std::vector<std::unique_ptr<Friendly>> friendlies_;

};