#pragma once

#include <Engine/Math/Matrix/MatrixCalculation.h>

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
	S,
	Z,
	O,
	J,
	I,
};

enum class BlockMode {
	Fall,
	Stay,
};

class CollisionManager;

class Mino {
public:
	Mino();
	~Mino();

	void Initialize();
	void Update();
	void Draw();
	void DebugGUI();

	void DrawLine();
	void InitBlock(BlockType type);

	//========================================================================*/
	//* Collision

	void OnCollisionEnter(const ColliderInfo& other);
	void OnCollisionStay(const ColliderInfo& other);
	void OnCollisionExit(const ColliderInfo& other);

	//========================================================================*/
	//* Getter
	const std::vector<std::unique_ptr<BaseBlock>>& GetBlocks() const {
		return blocks_;
	}
	std::vector<std::unique_ptr<BaseBlock>>& GetBlock() {
		return blocks_;
	}
	BlockType GetBlockType() {
		return blockType_;
	}
	BlockMode GetBlockMode() {
		return blockMode_;
	}

	Trans& GetTransform();
	const Trans& GetTransform() const;

	//========================================================================*/
	//* Setter
	void SetBlockMode(BlockMode mode) {
		blockMode_ = mode;
	}

	void SetCollisionMana(CollisionManager* cMana);

private:
	Trans transform;

	BlockType blockType_;
	BlockMode blockMode_;

	std::vector<std::unique_ptr<BaseBlock>> blocks_;

	CollisionManager* cMana_;
};
