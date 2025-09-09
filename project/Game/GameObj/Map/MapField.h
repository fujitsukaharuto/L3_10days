#pragma once

#include <memory>
#include <vector>

#include "Engine/Model/Sprite.h"

#include "Game/GameObj/Block/Mino.h"

#include "HumanMoldManager.h"

class CollisionManager;
class Climber;
class FriendlyManager;


class MapField {
public:
	MapField();
	~MapField();

	void Initialize();
	void Update();
	void Draw(Material* mate = nullptr, bool is = false);
	void DebugGUI();

	void TitleInit();
	void TitleUpdate();
	void TitleUpdateSelectPanel();
	bool TitleToGame();
	void TitleDraw();

	void DrawCells();

	void BackDraw();
	void FactoryDraw();
	void CursorDraw();

	void CellBackgroundDraw();
	void CellRequiredSpriteDraw();

	void UpdateSelectPanel();

	void UpdateSelectPanelControlling();
	void UpdateSelectPanelUncontrolling();

	void SelectMino();
	void AddMino();
	void UpdateControlMino();

	void ArrowUpdate();
	void FrameUpdate();

	void MoveControlMino();
	void CellSet();
	bool CanArrangement(); // マウスで置く際のチェック用

	void CompleteArrangement();

	void RandomizeTable();
	void ResetMold();

public:
	// TODO : 定数にする
	size_t GetMapHeight() const { return 15; }
	size_t GetMapWidth() const { return 15; }

	std::pair<i32, i32> CalcCellIndex(const Vector3& position) const;

	float GetCellSize() const { return cellsSize_; }

	void SetFriendlyManager(FriendlyManager* const friendlyManager) { friendlyManager_ = friendlyManager; }

	const Vector2& GetCellPosition() const { return cellsPos_; }

private:
	void RemoveControlMino();
	void CellSpriteSetColor();

	void InitCells();

	void LoadMinoTables();

private:
	/// <summary>
	/// マップ上の各セルデータ
	/// </summary>
	struct CellData {
		GenderType genderType{ GenderType::None }; // ブロックが配置されているか
		bool isRequired; // 攻撃に関与するセル
		std::unique_ptr<Sprite> background; // セルの背景
		std::unique_ptr<Sprite> required;
		std::unique_ptr<Sprite> block; // セルに配置されているブロック
	};

	/// <summary>
	/// 1テーブルあたりのミノデータ
	/// </summary>
	struct MinoTable {
		MoldType friendlyType;

		i32 numManMino;
		i32 numWomanMino;

		std::vector<std::unique_ptr<Mino>> minos;
	};

private:
	bool haveControlMino_ = false;
	bool isSmallChange_ = false;
	bool isTitleToGame_ = false;

	const i32 kMapWidth_ = 15;
	const i32 kMapHeight_ = 15;

	std::vector<std::vector<std::unique_ptr<CellData>>> cellsData_;

	i32 tableIndex;
	std::optional<i32> useMinoIndex;
	std::vector<MinoTable> minoTables;

	Vector2 cellNum_;

	Mino* controlMino_;

	// この３つはデバッグ表示用
	// これは何…
	// 多分男ブロックと音場ブロックの累積をメモってる
	std::vector<int> maxB_;
	std::vector<int> manB_;
	std::vector<int> womanB_;

	int preGender_ = 0;
	int gender_ = 0;

	int preMinoButtonNum_ = 0;
	int minoButtonNum_ = 0;
	int mapSizeNum_ = 2;
	int blockButtonNum_ = 0;
	Vector2 panelSize_ = { 400.0f,65.0f };
	float manPanelTime_ = 0.0f;
	float womanPanelTime_ = 0.0f;
	float defaultSelectPanelTime_ = 20.0f;
	float panelTexturePosY_;
	float selectorSizeTime_;
	float arrowMoveTime_;
	float frameMoveTime_;
	float titleToGameTime_ = 90.0f;
	Vector2 selectorMaxSize_;
	Vector2 selectorMinSize_;
	Vector2 selectorDeleteSize_;

	std::unique_ptr<Sprite> manPanelTex_;
	std::unique_ptr<Sprite> womanPanelTex_;
	std::unique_ptr<Sprite> genderPanelTex_;
	std::unique_ptr<Sprite> frameTex_;
	std::unique_ptr<Sprite> subFrameTex_;
	std::unique_ptr<Sprite> completeTex_;

	std::unique_ptr<Sprite> mapSizeTex_;
	std::unique_ptr<Sprite> arrowLTex_;
	std::unique_ptr<Sprite> arrowRTex_;

	std::unique_ptr<Sprite> factoryTex_;
	std::unique_ptr<Sprite> enemyFactoryTex_;

	std::unique_ptr<Sprite> BackPanelTex_;
	std::unique_ptr<Sprite> factoryBackPanelTex_;
	std::unique_ptr<Sprite> cursorTex_;
	std::unique_ptr<Sprite> grabCursorTex_;

	std::unique_ptr<Sprite> selectorTex_;
	std::unique_ptr<Sprite> nowSelectorTex_;

	float nextSpace_ = 10.0f;

	Vector2 cellsPos_; // セルの左下の位置
	float cellsSize_ = 18.0f; // セル一つの大きさ

	HumanMoldManager moldManager;

	FriendlyManager* friendlyManager_ = nullptr;
};
