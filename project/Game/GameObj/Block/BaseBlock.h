#pragma once
#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"

class CollisionManager;

class BaseBlock : public OriginGameObject {
public:
	BaseBlock();
	~BaseBlock()override;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;

	//========================================================================*/
	//* Collision

	void OnCollisionEnter(const ColliderInfo& other);
	void OnCollisionStay(const ColliderInfo& other);
	void OnCollisionExit(const ColliderInfo& other);

	//========================================================================*/
	//* Getter
	AABBCollider* GetCollider() { return collider_.get(); }

	void SetCollisionManager(CollisionManager* cMana);

private:

	std::unique_ptr<AABBCollider> collider_;


	CollisionManager* cMana_;
};
