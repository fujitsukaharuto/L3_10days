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

private:
	// ひとまず進行方向
	Vector3 kMoveDir_ = { 1.0f,0.0f,0.0f };
	float speed_ = 0.01f;
};