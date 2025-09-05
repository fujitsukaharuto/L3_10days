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
class Climber;

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

	void OldLineCheck();

	void SetColliderManager(CollisionManager* cMana);
	void SetClimber(Climber* climber);

public:
	const std::vector<int>& GetMapRows(size_t row) const;
	const Mino* GetFeatureMino() const;

	// TODO : 定数にする
	size_t GetMapHeight() const { return map_.size(); }
	size_t GetMapWidth() const { return map_[0].size(); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <returns>[row, column]</returns>
	std::pair<int, int> CalcFieldGrid(const Vector3& pos) const;

	const float GetOldDistance() const;

private:
	void RemoveControlMino();
	void FutureMinoUpdate();
	void CameraMoveUpdate();

private:
	bool canQuickDrop_ = true;

	std::vector<std::vector<int>> map_;
	const uint32_t kMapWidth_ = 20;

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

	int oldLine_ = 15;
	int old_;
	float nextSpace_ = 10.0f;

	bool isCameraMove_ = false;
	float cameraHeight_ = 22.0f;
	float cameraMoveTime_ = 30.0f;

	CollisionManager* cMana_;

	Climber* climber_{ nullptr };
};
