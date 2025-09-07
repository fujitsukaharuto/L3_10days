#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"

/// <summary>
/// 敵キャラ
/// </summary>
class Enemy: public BaseChara {
public:
	Enemy(const CharaStatus& status, const Vector3& popPos);
	~Enemy()override = default;

	void Update()override;

private:
	// ひとまず進行方向
	Vector3 kMoveDir_ = { -1.0f,0.0f,0.0f };
	float speed_ = 0.01f;
};