#pragma once

#include "Game/OriginGameObject.h"

enum class FactoryState {
	IDLE,
	POP,
};

/// <summary>
/// 工場
/// </summary>
class Factory : public OriginGameObject {
public:
	Factory(const Vector3& pos);
	~Factory()override = default;

	void Update()override;

	void Draw(Material* mate = nullptr, bool is = false)override;

private:
	FactoryState state_;

};