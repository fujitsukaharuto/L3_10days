#pragma once

#include "GameObj/Chara/BaseChara/BaseChara.h"
#include "Engine/Audio/AudioPlayer.h"

/// <summary>
/// 敵キャラ
/// </summary>
class Enemy: public BaseChara {
public:
	Enemy(const CharaStatus& status, const Vector3& popPos);
	~Enemy()override = default;

	void Update()override;

	void Search()override;

private:
	float winXPos_ = 22.0f;
	SoundData* break_;
};