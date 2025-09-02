#pragma once
#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"
#include "Game/GameObj/Block/BaseBlock.h"

enum class BlockType {
	L,
	T,
};

enum class BlockMode {
	Fall,
	Stay,
};

class Mino : public OriginGameObject {
public:
	Mino();
	~Mino()override;

	void Initialize()override;
	void Update()override;
	void Draw(Material* mate = nullptr, bool is = false)override;
	void DebugGUI()override;

	void InitBlock(BlockType type);

	//========================================================================*/
	//* Collision

	void OnCollisionEnter(const ColliderInfo& other);
	void OnCollisionStay(const ColliderInfo& other);
	void OnCollisionExit(const ColliderInfo& other);

	//========================================================================*/
	//* Getter
	std::vector<std::unique_ptr<BaseBlock>>& GetBlock() { return blocks_; }
	AABBCollider* GetCollider() { return collider_.get(); }
	BlockType GetBlockType() { return blockType_; }
	BlockMode GetBlockMode() { return blockMode_; }

	//========================================================================*/
	//* Setter
	void SetBlockMode(BlockMode mode) { blockMode_ = mode; }

private:

	BlockType blockType_;
	BlockMode blockMode_;

	std::vector<std::unique_ptr<BaseBlock>> blocks_;
	std::unique_ptr<AABBCollider> collider_;

};
