#pragma once
#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"
#include "Game/GameObj/Block/Mino.h"

class CollisionManager;
class Climber;

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

	void MoveControlMino();
	void CellCheck();
	void QuickDrop();

	void SetColliderManager(CollisionManager* cMana);
	void SetClimber(Climber* climber);

public:
	const std::vector<int>& GetMapRows(size_t row) const;
	const Mino* GetFeatureMino() const;

	// TODO : 定数にする
	size_t GetMapHeight() const { return map_.size(); }
	size_t GetMapWidth() const { return map_[0].size(); }

private:

	void RemoveControlMino();
	void FutureMinoUpdate();

private:

	std::vector<std::vector<int>> map_;

	float downTime_ = 60.0f;

	Vector2 cellNum_;
	std::unique_ptr<Mino> controlMino_;
	std::unique_ptr<Mino> futureMino_;
	std::vector<std::unique_ptr<Mino>> minos_;

	CollisionManager* cMana_;

	Climber* climber_{ nullptr };
};
