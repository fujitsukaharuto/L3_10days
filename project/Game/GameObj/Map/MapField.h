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

	void BackDraw();
	void FactoryDraw();
	void CursorDraw();
	void ArrangementDraw();

	void UpdateSelectPanel();
	void SelectMino();
	void ReturenSelectMino();
	void AddMino(BlockType type);
	void UpdateControlMino();

	void MoveControlMino();
	void CellCheck(); // Dropする時に置けるかどうかのチェック用
	void QuickDrop();
	void CellSet();
	bool ArrangementCheck(); // マウスで置く際のチェック用

	void CompleteArragement();

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
	void CellSpriteSetColor();

	void InitCells();
	void GenderColor();

private:
	bool canQuickDrop_ = true;
	bool isCameraMove_ = false;
	bool haveControlMino_ = false;

	std::vector<std::vector<int>> map_;
	std::vector<std::vector<int>> TypeMap_;
	const uint32_t kMapWidth_ = 15;

	Vector2 cellNum_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> cells_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> typeCells_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> arrangementCells_;

	std::unique_ptr<Mino> controlMino_;
	std::unique_ptr<Mino> futureMino_;
	std::vector<std::unique_ptr<Mino>> minos_;

	// この３つはデバッグ表示用
	std::vector<int> maxB_;
	std::vector<int> manB_;
	std::vector<int> womanB_;

	int gender_ = 0;
	int minoButtonNum_ = 0;
	int mapSizeNum_ = 2;
	int blockButtonNum_ = 0;
	Vector2 panelSize_ = { 400.0f,65.0f };
	float selectPanelTime_ = 0.0f;
	float defaultSelectPanelTime_ = 30.0f;
	float panelTexturePosY_;
	float selectorSizeTime_;
	Vector2 selectorMaxSize_;
	Vector2 selectorMinSize_;
	Vector2 selectorDeleteSize_;
	std::vector<BlockType> selectTypes_;
	std::unique_ptr<Sprite> panelTex_;
	std::unique_ptr<Sprite> manPanelTex_;
	std::unique_ptr<Sprite> frameTex_;
	std::unique_ptr<Sprite> completeTex_;

	std::unique_ptr<Sprite> mapSizeTex_;
	std::unique_ptr<Sprite> arrowLTex_;
	std::unique_ptr<Sprite> arrowRTex_;

	std::unique_ptr<Sprite> BackPanelTex_;
	std::unique_ptr<Sprite> factoryBackPanelTex_;
	std::unique_ptr<Sprite> cursorTex_;
	std::unique_ptr<Sprite> grabCursorTex_;
	std::vector<std::unique_ptr<Sprite>> buttonTex_;
	std::unique_ptr<Sprite> selectorTex_;

	int oldLine_ = 15;
	int old_;
	float nextSpace_ = 10.0f;

	float cameraHeight_ = 22.0f;
	float cameraMoveTime_ = 30.0f;

	Vector2 cellsPos_;
	float cellsSize_ = 18.0f;

	CollisionManager* cMana_;

	Climber* climber_{ nullptr };
};
