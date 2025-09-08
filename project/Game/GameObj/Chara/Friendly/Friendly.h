#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"


class EnemyManager;

/// <summary>
/// 友軍
/// </summary>
class Friendly: public BaseChara {
public:
	Friendly(const CharaStatus& status, const Vector3& popPos);
	~Friendly()override = default;

	void Update()override;

	void Search()override;

	void SetEne(EnemyManager* ene);

private:
	EnemyManager* ene_;
};