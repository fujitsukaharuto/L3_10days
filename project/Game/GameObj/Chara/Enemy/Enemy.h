#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"

/// <summary>
/// 敵キャラ
/// </summary>
class Enemy: public BaseChara {
public:
	Enemy(const std::string& modelName, const Vector3& popPos);
	~Enemy()override = default;

	void Update()override;

private:

};