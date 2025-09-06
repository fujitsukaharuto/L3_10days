#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"

/// <summary>
/// 友軍
/// </summary>
class Friendly: public BaseChara {
public:
	Friendly(const std::string& modelName, const Vector3& popPos);
	~Friendly()override = default;

	void Update()override;

private:

};