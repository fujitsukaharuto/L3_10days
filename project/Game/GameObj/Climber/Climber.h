#pragma once

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

/// <summary>
/// 人間
/// </summary>
class Climber {
public:
	Climber();
	~Climber() = default;

	void Update();

	void Draw();



};