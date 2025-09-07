#pragma once

#include <Engine/Math/Matrix/MatrixCalculation.h>

#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"
#include "Game/GameObj/Block/BaseBlock.h"

enum class GenderType {
	Man,
	Woman,
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

	void InitBlock(GenderType gender);

	//========================================================================*/
	//* Getter
	const std::vector<std::unique_ptr<BaseBlock>>& GetBlocks() const {
		return blocks_;
	}
	std::vector<std::unique_ptr<BaseBlock>>& GetBlock() {
		return blocks_;
	}
	GenderType GetGender() { return gender_; }

	Trans& GetTransform();
	const Trans& GetTransform() const;

private:
	Trans transform;

	GenderType gender_;

	std::vector<std::unique_ptr<BaseBlock>> blocks_;
};
