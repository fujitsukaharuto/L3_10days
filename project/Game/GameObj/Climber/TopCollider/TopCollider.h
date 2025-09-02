#pragma once

#include "Game/OriginGameObject.h"
#include "Game/Collider/AABBCollider.h"

class Climber;

/// <summary>
/// 人間のコライダー用のオブジェクト
/// </summary>
class ClimberTopCollider: public OriginGameObject {
public:
	ClimberTopCollider(Climber* climber);
	~ClimberTopCollider()override = default;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;

	void OnCollisionEnter(const ColliderInfo& other);
	void OnCollisionStay(const ColliderInfo& other);
	void OnCollisionExit(const ColliderInfo& other);

	AABBCollider* GetCollider() {
		return collider_.get();
	}

private:
	const float kBlockSize_ = 2.0f;
	// 親
	Climber* climber_ = nullptr;

	// コライダー
	std::unique_ptr<AABBCollider> collider_;

};