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
	void AddMino();
	void UpdateControlMino();

	void MoveControlMino();
	void CellCheck(); // Dropする時に置けるかどうかのチェック用
	void CellSet();
	bool ArrangementCheck(); // マウスで置く際のチェック用

	void CompleteArragement();

	void SetClimber(Climber* climber);

public:
	const std::vector<int>& GetMapRows(size_t row) const;
	const Mino* GetFeatureMino() const;

	// TODO : 定数にする
	size_t GetMapHeight() const { return map_.size(); }
	size_t GetMapWidth() const { return map_[0].size(); }

private:
	void RemoveControlMino();
	void CellSpriteSetColor();

	void InitCells();
	void GenderColor();

private:
	/// <summary>
	/// マップ上の各セルデータ
	/// </summary>
	struct CellData {
		bool isFilled = false; // ブロックが配置されているか
		bool required; // 攻撃に関与するセル
		std::unique_ptr<Sprite> background; // セルの背景
		std::unique_ptr<Sprite> block; // セルに配置されているブロック
	};

	/// <summary>
	/// 1テーブルあたりのミノデータ
	/// </summary>
	struct MinoTable {
		std::vector<std::unique_ptr<Mino>> minos;
	};

private:
	bool haveControlMino_ = false;

	const uint32_t kMapWidth_ = 15;

	std::vector<std::vector<CellData>> cellsData_;

	std::vector<MinoTable> minoTables;

	Vector2 cellNum_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> cells_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> typeCells_;
	std::vector<std::vector<std::unique_ptr<Sprite>>> arrangementCells_;

	Mino* controlMino_;

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

	float nextSpace_ = 10.0f;

	Vector2 cellsPos_;
	float cellsSize_ = 18.0f;
};
