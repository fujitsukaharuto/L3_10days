#pragma once
#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"
#include "Game/GameObj/Block/Mino.h"


class MapField {
public:
	MapField();
	~MapField();

	void Initialize();
	void Update();
	void Draw(Material* mate = nullptr, bool is = false);
	void DebugGUI();

	void AddMino(BlockType type);
	void UpdateControlMino();

	void CellCheck();
	void QuickDrop();

private:

	void RemoveControlMino();

private:

	std::vector<std::vector<int>> map_;

	float downTime_ = 60.0f;

	Vector2 cellNum;
	std::unique_ptr<Mino> controlMino_;
	std::vector<std::unique_ptr<Mino>> minos_;

};
