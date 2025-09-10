#pragma once

#include "Game/OriginGameObject.h"
#include "SimpleAnimation/SimpleAnimation.h"

enum class FactoryState {
	IDLE,
	POP,
};

/// <summary>
/// 工場
/// </summary>
class Factory : public OriginGameObject {
public:
	Factory(const Vector3& pos, bool isfriend = true);
	~Factory()override = default;

	void Pop();
	void Update()override;

	void Draw(Material* mate = nullptr, bool is = false)override;


private:
	bool isFriend_ = false;
	bool isPop_ = false;
	const float popTime_ = 0.5f;
	float popTimer_ = 0.0f;

	Vector3 pos_;

	FactoryState state_;

	SimpleAnimation<Vector3> animation;
	Vector3 popMinScale_ = { 2.0f,3.0f,20.0f };
	Vector3 popMaxScale_ = { 4.0f,1.5f,20.0f };

};