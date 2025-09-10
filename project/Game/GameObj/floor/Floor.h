#pragma once

#include "Game/OriginGameObject.h"

/// <summary>
/// 床
/// </summary>
class Floor: public OriginGameObject {
public:
	Floor();
	~Floor() = default;

	void Update()override;

	void Draw(Material* mate = nullptr, bool is = false)override;

	void DebugDraw();

private:

};