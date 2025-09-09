#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"

/// <summary>
/// 友軍
/// </summary>
class Friendly: public BaseChara {
public:
	Friendly(const CharaStatus& status, const Vector3& popPos);
	~Friendly()override = default;

	void Update()override;

	void Search()override;

private:
	float winXPos_ = 50.0f;
};