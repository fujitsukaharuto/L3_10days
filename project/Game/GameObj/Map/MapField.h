#pragma once
#include "Game/OriginGameObject.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>
#include "Game/Collider/BaseCollider.h"
#include "Game/Collider/AABBCollider.h"
#include "Model/Line3dDrawer.h"
#include "Engine/Model/Sprite.h"
#include "Game/GameObj/Block/Mino.h"

class CollisionManager;

class MapField {
public:
	MapField();
	~MapField();

	void Initialize();
	void Update();
	void Draw(Material* mate = nullptr, bool is = false);
	void DebugGUI();

	void UpdateSelectPanel();
	void SelectMino();
	void ReturenSelectMino();
	void AddMino(BlockType type);
	void UpdateControlMino();

	void MoveControlMino();
	void CellCheck();
	void QuickDrop();

	void SetColliderManager(CollisionManager* cMana);

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

	int minoButtonNum_ = 0;
	Vector2 panelSize_ = { 640.0f,100.0f };
	float selectPanelTime_ = 0.0f;
	float defaultSelectPanelTime_ = 30.0f;
	float panelTexturePosY_;
	float selectorSizeTime_;
	Vector2 selectorMaxSize_;
	Vector2 selectorMinSize_;
	Vector2 selectorDeleteSize_;
	std::vector<BlockType> selectTypes_;
	std::unique_ptr<Sprite> panelTex_;
	std::vector<std::unique_ptr<Sprite>> buttonTex_;
	std::unique_ptr<Sprite> selectorTex_;

	CollisionManager* cMana_;
};
