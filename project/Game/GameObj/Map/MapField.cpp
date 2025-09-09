#include "MapField.h"

#include "Engine//Math/Random/Random.h"
#include "Engine/Input/Input.h"
#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"

#include "Engine/Editor/JsonSerializer.h"

#undef min
#undef max

MapField::MapField() {}

MapField::~MapField() {}

void MapField::Initialize() {
	manPanelTex_ = std::make_unique<Sprite>();
	manPanelTex_->Load("manFrame.png");
	manPanelTex_->SetPos({ 140.0f,110.0f,0.0f });
	womanPanelTex_ = std::make_unique<Sprite>();
	womanPanelTex_->Load("womanFrame.png");
	womanPanelTex_->SetPos({ 430.0f,110.0f,0.0f });
	panelTexturePosY_ = 102.5f;

	genderPanelTex_ = std::make_unique<Sprite>();
	genderPanelTex_->Load("choice.png");
	genderPanelTex_->SetPos({ 285.0f, 35.0f,0.0f });
	frameTex_ = std::make_unique<Sprite>();
	frameTex_->Load("Frame.png");
	frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
	subFrameTex_ = std::make_unique<Sprite>();
	subFrameTex_->Load("Frame.png");
	subFrameTex_->SetPos({ 285.0f, 400.0f,0.0f });
	completeTex_ = std::make_unique<Sprite>();
	completeTex_->Load("completed.png");
	completeTex_->SetPos({ 285.0f, 660.0f,0.0f });

	selectorTex_ = std::make_unique<Sprite>();
	selectorTex_->Load("SquareFrame.png");
	selectorTex_->SetColor({ 1.0f,0.0f,0.0f,0.7f });
	selectorTex_->SetPos({ 100.0f,110.0f, 0.0f });
	selectorMaxSize_ = { 95.0f + 30.0f,110.0f + 30.0f };
	selectorMinSize_ = { 95.0f + 10.0f,110.0f + 10.0f };

	nowSelectorTex_ = std::make_unique<Sprite>();
	nowSelectorTex_->Load("white2x2.png");
	nowSelectorTex_->SetColor({ 0.7f,0.7f,0.0f,0.6f });
	nowSelectorTex_->SetPos({ 100.0f,110.0f,0.0f });
	nowSelectorTex_->SetSize({ 102.0f,116.0f });

	BackPanelTex_ = std::make_unique<Sprite>();
	BackPanelTex_->Load("background.png");
	BackPanelTex_->SetPos({ 925.0f,360.0f,0.0f });
	factoryBackPanelTex_ = std::make_unique<Sprite>();
	factoryBackPanelTex_->Load("factoryBackground.png");
	factoryBackPanelTex_->SetSize({ 580.0f,720.0f });
	factoryBackPanelTex_->SetPos({ 285.0f,360.0f,0.0f });

	cursorTex_ = std::make_unique<Sprite>();
	cursorTex_->Load("normalCursor.png");
	cursorTex_->SetAnchor({ 0.25f,0.25f });
	grabCursorTex_ = std::make_unique<Sprite>();
	grabCursorTex_->Load("grabCursor.png");

	arrowLTex_ = std::make_unique<Sprite>();
	arrowLTex_->Load("arrow.png");
	arrowLTex_->SetFlipX(true);
	arrowLTex_->SetAnchor({ 0.5f,0.5f });
	arrowLTex_->SetPos({ 55.0f, 400.0f,0.0f });
	arrowRTex_ = std::make_unique<Sprite>();
	arrowRTex_->Load("arrow.png");
	arrowRTex_->SetPos({ 515.0f, 400.0f,0.0f });

	factoryTex_ = std::make_unique<Sprite>();
	factoryTex_->Load("myFactory.png");
	factoryTex_->SetPos({ 595.0f, 360.0f,0.0f });
	enemyFactoryTex_ = std::make_unique<Sprite>();
	enemyFactoryTex_->Load("enemyFactory.png");
	enemyFactoryTex_->SetPos({ 1255.0f, 360.0f,0.0f });

	mapSizeTex_ = std::make_unique<Sprite>();
	mapSizeTex_->Load("frameSize.png");
	mapSizeTex_->SetSize({ 40.0f, 50.0f });
	mapSizeTex_->SetPos({ 285.0f, 170.0f,0.0f });
	mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

	InitCells();

	LoadMinoTables();
}

void MapField::Update() {
	if (controlMino_) {
		haveControlMino_ = true;
	}
	else {
		haveControlMino_ = false;
	}

	UpdateControlMino();
	UpdateSelectPanel();
}

void MapField::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	factoryTex_->Draw();
	enemyFactoryTex_->Draw();
}

void MapField::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("MapField")) {
		if (ImGui::Button("Complete")) {
			CompleteArrangement();
		}
		for (int i = 0; i < maxB_.size(); i++) {
			ImGui::Text("Max : %d", maxB_[i]); ImGui::SameLine();
			ImGui::Text("Man : %d", manB_[i]); ImGui::SameLine();
			ImGui::Text("Woman : %d", womanB_[i]);
		}
		Vector3 pos = genderPanelTex_->GetPos();
		ImGui::DragFloat2("ManPanelPos", &pos.x);
		genderPanelTex_->SetPos(pos);
		Vector2 size = genderPanelTex_->GetSize();
		ImGui::DragFloat2("ManPanelSize", &size.x);
		genderPanelTex_->SetSize(size);
		pos = frameTex_->GetPos();
		ImGui::DragFloat2("WomanPanelPos", &pos.x);
		frameTex_->SetPos(pos);
		size = frameTex_->GetSize();
		ImGui::DragFloat2("WomanPanelSize", &size.x);
		frameTex_->SetSize(size);
	}
#endif // _DEBUG
}

void MapField::TitleInit() {
	std::vector<std::vector<i32>> map_ = {
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,0,0,0,2,2,0,0,0,0},
			{0,0,0,1,1,0,2,2,0,0,2,2,0,0,0},
			{0,0,0,0,0,0,2,2,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,0,2,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,0,2,0,0,0,0,0,0},
			{0,0,0,0,0,1,1,0,2,2,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	for (i32 rowI = 0; auto& row : cellsData_) {
		for (i32 colI = 0; auto& cell : row) {
			cell->genderType = (GenderType)map_[rowI][colI];
			if (cell->genderType == GenderType::Man) {
				cell->block->SetColor({ 0,0,1,0.6f });
			}
			else if (cell->genderType == GenderType::Woman) {
				cell->block->SetColor({ 1.0f,0.08f,0.58f,0.6f });
			}
			++colI;
		}
		++rowI;
	}
}

void MapField::TitleUpdate() {
	if (controlMino_) {
		haveControlMino_ = true;
	}
	else {
		haveControlMino_ = false;
	}
	//UpdateControlMino();
	TitleUpdateSelectPanel();
}

void MapField::TitleUpdateSelectPanel() {
	if (!controlMino_) {
	}
	SelectMino();
}

void MapField::TitleDraw() {
	BackPanelTex_->Draw();

	factoryBackPanelTex_->Draw();
	frameTex_->Draw();
	completeTex_->Draw();
	factoryTex_->Draw();
	enemyFactoryTex_->Draw();
	manPanelTex_->Draw();
	womanPanelTex_->Draw();

	genderPanelTex_->Draw();

	DrawCells();
}

void MapField::DrawCells() {
	// 背景セル
	CellBackgroundDraw();
	// 人形セル
	CellRequiredSpriteDraw();

	// 配置済みセル
	for (auto& rows : cellsData_) {
		for (auto& cell : rows) {
			if (cell->block) {
				cell->block->Draw();
			}
		}
	}

	// 操作中のミノ
	if (controlMino_) {
		controlMino_->DrawBlocks();
	}
}

void MapField::BackDraw() {
	//BackPanelTex_->Draw();
}

void MapField::FactoryDraw() {
	// 
	factoryBackPanelTex_->Draw();
	frameTex_->Draw();
	subFrameTex_->Draw();
	completeTex_->Draw();
	if (mapSizeNum_ != 2) {
		arrowLTex_->Draw();
	}
	if (mapSizeNum_ != 0) {
		arrowRTex_->Draw();
	}
	mapSizeTex_->Draw();
	manPanelTex_->Draw();
	womanPanelTex_->Draw();
	if (haveControlMino_) {
		nowSelectorTex_->Draw();
	} else {
		selectorTex_->Draw();
	}
	genderPanelTex_->Draw();

	BackPanelTex_->Draw();

	if (frameMoveTime_ == 0.0f) {
		DrawCells();
	}
}

void MapField::CursorDraw() {
	if (haveControlMino_) {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		grabCursorTex_->SetPos({ mouse.x,mouse.y,0.0f });
		grabCursorTex_->Draw();
	}
	else {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });
		cursorTex_->Draw();
	}
}

void MapField::CellBackgroundDraw() {
	for (auto& rows : cellsData_) {
		for (auto& cell : rows) {
			cell->background->Draw();
		}
	}
}

void MapField::CellRequiredSpriteDraw() {
	for (auto& rows : cellsData_) {
		for (auto& cell : rows) {
			if (cell->isRequired) {
				cell->required->Draw();
			}
		}
	}
}

void MapField::UpdateSelectPanel() {
	if (controlMino_) {
		UpdateSelectPanelControlling();
	}
	else {
		UpdateSelectPanelUncontrolling();
	}

	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	// 完了を押す
	Vector3 pos = completeTex_->GetPos();
	Vector2 size = completeTex_->GetSize();
	r32 halfW = size.x * 0.5f;
	r32 halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
			if (!controlMino_) {
				CompleteArrangement();
			}
		}
	}

	// 矢印の選択
	ArrowUpdate();

	// 背景の枠の移動
	FrameUpdate();

	// 選択中のアニメーション
	SelectMino();
}

void MapField::UpdateSelectPanelControlling() {
	Vector2 mouse = Input::GetInstance()->GetMousePosition();
}

void MapField::UpdateSelectPanelUncontrolling() {
	Vector2 mouse = Input::GetInstance()->GetMousePosition();

	// パネルの選択
	{
		Vector3 pos{};
		if (gender_ == int(GenderType::Man))   pos = { 100.0f,110.0f, 0.0f };//buttonTex_[blockButtonNum_]->GetPos();   // 中心座標
		if (gender_ == int(GenderType::Woman)) pos = { 465.0f,110.0f, 0.0f };//buttonTex_[blockButtonNum_]->GetPos();   // 中心座標
		Vector2 size = { 102.0f,116.0f }; //buttonTex_[blockButtonNum_]->GetSize(); // 幅・高さ
		float halfW = size.x * 0.5f;
		float halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			minoButtonNum_ = 0;
			if (manPanelTime_ == defaultSelectPanelTime_ || womanPanelTime_ == defaultSelectPanelTime_) {
				if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
					if (!controlMino_) {
						controlMino_ = minoTables[0].minos[0].get();
						// AddMino(selectTypes_[blockButtonNum_]);
						return;
					}
				}
			}
		}
		if (gender_ == int(GenderType::Man))   pos = { 195.0f,85.0f, 0.0f };
		if (gender_ == int(GenderType::Woman)) pos = { 370.0f,85.0f, 0.0f };
		size = { 68.0f,64.0f };
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			minoButtonNum_ = 1;
			if (manPanelTime_ == defaultSelectPanelTime_ || womanPanelTime_ == defaultSelectPanelTime_) {
				if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
					if (!controlMino_) {
						controlMino_ = minoTables[0].minos[0].get();
						// AddMino(selectTypes_[blockButtonNum_]);
						return;
					}
				}
			}
		}
		if (gender_ == int(GenderType::Man))   pos = { 195.0f,146.0f, 0.0f };
		if (gender_ == int(GenderType::Woman)) pos = { 370.0f,146.0f, 0.0f };
		size = { 67.0f,45.0f };
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			minoButtonNum_ = 2;
			if (manPanelTime_ == defaultSelectPanelTime_ || womanPanelTime_ == defaultSelectPanelTime_) {
				if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
					if (!controlMino_) {
						controlMino_ = minoTables[0].minos[0].get();
						// AddMino(selectTypes_[blockButtonNum_]);
						return;
					}
				}
			}
		}



		// 性別決める
		pos = genderPanelTex_->GetPos();
		size = genderPanelTex_->GetSize();
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		Vector3 pos2 = manPanelTex_->GetPos();
		Vector2 size2 = manPanelTex_->GetSize();
		float halfW2 = size2.x * 0.5f;
		float halfH2 = size2.y * 0.5f;
		if ((mouse.x >= pos.x - halfW && mouse.x <= pos.x && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) ||
			((mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2))) {
			if ((mouse.x >= pos.x - halfW && mouse.x <= pos.x && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) || (mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2)) {
				gender_ = int(GenderType::Man);
			}
			if (womanPanelTime_ <= 0.0f) {
				manPanelTime_ += FPSKeeper::DeltaTime();
				manPanelTime_ = std::clamp(manPanelTime_, 0.0f, defaultSelectPanelTime_);
			}
		}
		else {
			manPanelTime_ -= FPSKeeper::DeltaTime();
			manPanelTime_ = std::clamp(manPanelTime_, 0.0f, defaultSelectPanelTime_);
		}
		pos2 = womanPanelTex_->GetPos();
		size2 = womanPanelTex_->GetSize();
		halfW2 = size2.x * 0.5f;
		halfH2 = size2.y * 0.5f;
		if ((mouse.x >= pos.x && mouse.x <= pos.x + halfW && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) ||
			((mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2))) {
			if (/*(mouse.x >= pos.x && mouse.x <= pos.x + halfW && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) ||*/
				(mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2)) {
				gender_ = int(GenderType::Woman);
			}
			if (manPanelTime_ <= 0.0f) {
				womanPanelTime_ += FPSKeeper::DeltaTime();
				womanPanelTime_ = std::clamp(womanPanelTime_, 0.0f, defaultSelectPanelTime_);
			}
		}
		else {
			womanPanelTime_ -= FPSKeeper::DeltaTime();
			womanPanelTime_ = std::clamp(womanPanelTime_, 0.0f, defaultSelectPanelTime_);
		}
	}

	{
		// マップサイズの変更
		Vector3 pos = arrowLTex_->GetPos();
		Vector2 size = arrowLTex_->GetSize();
		float halfW = size.x * 0.5f;
		float halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && /*minos_.size() == 0 && */mapSizeNum_ != 2) {
					mapSizeNum_++;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
					subFrameTex_->SetPos({ -290.0f, 400.0f,0.0f });
					isSmallChange_ = true;
				}
			}
			arrowLTex_->SetColor({ 0.5f,0.3f,0.3f,1.0f });
		}
		else {
			arrowLTex_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		pos = arrowRTex_->GetPos();
		size = arrowRTex_->GetSize();
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && /*minos_.size() == 0 && */ mapSizeNum_ != 0) {
					mapSizeNum_--;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
					subFrameTex_->SetPos({ 880.0f, 400.0f,0.0f });
					isSmallChange_ = false;
				}
			}
			arrowRTex_->SetColor({ 0.5f,0.3f,0.3f,1.0f });
		}
		else {
			arrowRTex_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
	}
}

void MapField::SelectMino() {
	float panelTime = 0.0f;
	if (manPanelTime_ > 0.0f) panelTime = manPanelTime_;
	if (womanPanelTime_ > 0.0f) panelTime = womanPanelTime_;
	if (preMinoButtonNum_ != minoButtonNum_ || preGender_ != gender_) {
		preMinoButtonNum_ = minoButtonNum_;
		preGender_ = gender_;
		if (minoButtonNum_ == 0) {
			if (gender_ == int(GenderType::Man)) { selectorTex_->SetPos({ 100.0f,115.0f, 0.0f }); nowSelectorTex_->SetPos({ 105.0f,110.0f,0.0f }); }
			if (gender_ == int(GenderType::Woman)) { selectorTex_->SetPos({ 465.0f,115.0f, 0.0f }); nowSelectorTex_->SetPos({ 467.5f,110.0f,0.0f }); }
			nowSelectorTex_->SetSize({ 102.0f,116.0f });
		} else if (minoButtonNum_ == 1) {
			if (gender_ == int(GenderType::Man)) { selectorTex_->SetPos({ 195.0f,90.0f, 0.0f }); nowSelectorTex_->SetPos({ 195.0f,85.0f, 0.0f }); }
			if (gender_ == int(GenderType::Woman)) { selectorTex_->SetPos({ 375.0f,90.0f, 0.0f }); nowSelectorTex_->SetPos({ 375.0f,85.0f,0.0f }); }
			nowSelectorTex_->SetSize({ 68.0f,64.0f });
		} else if (minoButtonNum_ == 2) {
			if (gender_ == int(GenderType::Man)) { selectorTex_->SetPos({ 195.0f,146.0f, 0.0f }); nowSelectorTex_->SetPos({ 195.0f,146.0f, 0.0f }); }
			if (gender_ == int(GenderType::Woman)) { selectorTex_->SetPos({ 375.0f,146.0f, 0.0f }); nowSelectorTex_->SetPos({ 376.5f,146.0f,0.0f }); }
			nowSelectorTex_->SetSize({ 67.0f,45.0f });
		}

		if (minoButtonNum_ == 0) {
			selectorMaxSize_ = { 95.0f + 30.0f,100.0f + 30.0f };
			selectorMinSize_ = { 95.0f + 10.0f,100.0f + 10.0f };
		} else if (minoButtonNum_ == 1) {
			selectorMaxSize_ = { 63.0f + 30.0f,50.0f + 30.0f };
			selectorMinSize_ = { 63.0f + 10.0f,50.0f + 10.0f };
		} else if (minoButtonNum_ == 2) {
			selectorMaxSize_ = { 62.0f + 30.0f,40.0f + 30.0f };
			selectorMinSize_ = { 62.0f + 10.0f,40.0f + 10.0f };
		}
	}

	if (!haveControlMino_) {
		selectorSizeTime_ += FPSKeeper::DeltaTime() * 0.25f;
		selectorSizeTime_ = std::fmod(selectorSizeTime_, std::numbers::pi_v<float>);
		// sin波で 0.0 ～ 1.0 に正規化
		float t = (std::sin(selectorSizeTime_) + 1.0f) * 0.5f;
		// 最小サイズと最大サイズを補間
		Vector2 size;
		size.x = selectorMinSize_.x + (selectorMaxSize_.x - selectorMinSize_.x) * t;
		size.y = selectorMinSize_.y + (selectorMaxSize_.y - selectorMinSize_.y) * t;
		// サイズを反映
		selectorTex_->SetSize(size);
		selectorDeleteSize_ = size;
	}
}

void MapField::UpdateControlMino() {
	if (!controlMino_) return;

	MoveControlMino();

	controlMino_->Update();
	if (Input::GetInstance()->IsTriggerMouse(0) && haveControlMino_) {
		CellSet();
		return;
	}
	//RemoveControlMino();

}

void MapField::ArrowUpdate() {
	arrowMoveTime_ += FPSKeeper::DeltaTime() * 0.1f;
	arrowMoveTime_ = std::fmod(arrowMoveTime_, std::numbers::pi_v<float>);
	// sin波で 0.0 ～ 1.0 に正規化
	float t = std::abs((sinf(arrowMoveTime_) + 1.0f) * 0.5f);
	Vector2 pos;
	pos.x = 55.0f + (45.0f - 55.0f) * t;
	pos.y = 515.0f + (525.0f - 515.0f) * t;
	arrowLTex_->SetPos({ pos.x ,400.0f,0.0f });
	arrowRTex_->SetPos({ pos.y ,400.0f,0.0f });
}

void MapField::FrameUpdate() {
	if (frameMoveTime_ > 0.0f) {
		frameMoveTime_ -= FPSKeeper::DeltaTime();

		if (frameMoveTime_ <= 0.0f) {
			frameMoveTime_ = 0.0f;
		}
		float t = 1.0f - (frameMoveTime_ / 30.0f);
		if (isSmallChange_) {
			float mainPos = std::lerp(285.0f, 880.0f, t);
			float subPos = std::lerp(-290.0f, 285.0f, t);

			frameTex_->SetPos({ mainPos,400.0f,0.0f });
			subFrameTex_->SetPos({ subPos,400.0f,0.0f });
		}
		else {
			float mainPos = std::lerp(285.0f, -290.0f, t);
			float subPos = std::lerp(880.0f, 285.0f, t);

			frameTex_->SetPos({ mainPos,400.0f,0.0f });
			subFrameTex_->SetPos({ subPos,400.0f,0.0f });
		}
		if (frameMoveTime_ <= 0.0f) {
			frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
			frameTex_->SetPos({ -290.0f, 400.0f,0.0f });
			isSmallChange_ = false;
		}
	}
}

void MapField::MoveControlMino() {
	if (!controlMino_) return;

	Vector2 nextCell = cellNum_;
	bool isMove = false;
	Vector2 mouse = Input::GetInstance()->GetMousePosition();

	const int gridSize = 15;
	const int cellSize = int(cellsSize_ - 1.0f); // 1マスのサイズ（px）
	float localX = mouse.x - cellsPos_.x;
	float localY = mouse.y - cellsPos_.y;
	// セル番号に変換
	int cellX = static_cast<int>(localX / cellSize);
	int cellY = static_cast<int>(localY / cellSize);
	cellX = std::clamp(cellX, 0, gridSize - 1);
	cellY = std::clamp(cellY, 0, gridSize - 1);
	if (cellX >= 0 && cellX < gridSize && cellY >= 0 && cellY < gridSize) {
		nextCell = { float(cellX),float(cellY) };
		isMove = true;
	}

	if (!isMove) return;

	CellSpriteSetColor();

	// controlMinoを範囲外に出ないようにする
	controlMino_->AdjustPosition(this, cellY, cellX);

	// チェック
	for (auto& block : controlMino_->GetBlocks()) {
		auto [row, column] = CalcCellIndex(block->sprite->GetPos());
		auto& cell = cellsData_[row][column];
		if (cell->genderType != GenderType::None) {
			cell->required->SetColor({ 1.0f,0.0f,0.0f,0.6f });
		}
	}

	cellNum_ = nextCell;
}

void MapField::CellSet() {
	if (CanArrangement()) {
		controlMino_->Update();
		RemoveControlMino();
	}
}

bool MapField::CanArrangement() {
	if (!controlMino_) {
		return false;
	}

	auto& blocks = controlMino_->GetBlocks();
	for (auto& block : blocks) {
		auto [row, column] = CalcCellIndex(block->sprite->GetPos());

		// 範囲外
		if (row < 0 || row >= kMapHeight_ || column < 0 || column >= kMapWidth_) {
			return false;
		}
		// 既に配置されている
		auto& cell = cellsData_[row][column];
		if (cell->genderType != GenderType::None) {
			return false;
		}
	}

	return true;
}

void MapField::CompleteArrangement() {
	if (controlMino_) return;
	int manBlocks = 0;
	int womanBlocks = 0;

	for (auto& row : cellsData_) {
		for (auto& cell : row) {
			switch (cell->genderType) {
			case GenderType::Man:
				++manBlocks;
				break;
			case GenderType::Woman:
				++womanBlocks;
				break;
			default:
				break;
			}
		}
	}
	int maxBlocks = manBlocks + womanBlocks;

	// 人間生成処理

	// 女ブロックの方が多い
	if (manBlocks < womanBlocks) {
		CharaStatus status;
		status.hp = maxBlocks;
		status.name = "cube.obj";
		status.power = womanBlocks;
		status.gender = WOMAN;

		// 自軍発車
		if (friendlyManager_) {
			friendlyManager_->AddFriendly(status);
		}
	}
	else {
		CharaStatus status;
		status.hp = maxBlocks;
		status.name = "cube.obj";
		status.power = manBlocks;
		status.gender = MAN;

		// 自軍発車
		if (friendlyManager_) {
			friendlyManager_->AddFriendly(status);
		}
	}

	maxB_.push_back(maxBlocks);
	manB_.push_back(manBlocks);
	womanB_.push_back(womanBlocks);

	// 再度初期化
	for (auto& row : cellsData_) {
		for (auto& cell : row) {
			cell->genderType = GenderType::None;
			cell->isRequired = false;
		}
	}
	blockButtonNum_ = Random::GetInt(0, 6);
}

std::pair<i32, i32> MapField::CalcCellIndex(const Vector3& position) const {
	i32 xIndex = static_cast<i32>((position.x - cellsPos_.x) / cellsSize_);
	i32 yIndex = static_cast<i32>((position.y - cellsPos_.y) / cellsSize_);
	return { yIndex, xIndex };
}

void MapField::RemoveControlMino() {
	auto& blocks = controlMino_->GetBlocks();

	Vector4 color = controlMino_->GetGender() == GenderType::Man ? Vector4(0.0f, 0.5f, 1.0f, 1.0f) : Vector4(1.0f, 0.5f, 0.8f, 1.0f);

	// 設置したやつをマップに反映
	for (auto& block : blocks) {
		auto [rowI, colI] = CalcCellIndex(block->sprite->GetPos());

		auto& cell = cellsData_[rowI][colI];

		cell->genderType = controlMino_->GetGender();
		cell->block->SetColor(color);
	}

	controlMino_ = nullptr;
}

void MapField::CellSpriteSetColor() {
	auto& blocks = controlMino_->GetBlocks();
	for (auto& block : blocks) {
		auto [row, column] = CalcCellIndex(block->sprite->GetPos());

		// 範囲外
		if (row < 0 || row >= kMapHeight_ || column < 0 || column >= kMapWidth_) {
			continue;
		}
		// 既に配置されている
		auto& cell = cellsData_[row][column];
		if (cell->genderType != GenderType::None) {
			block->sprite->SetColor({ 1.0f,0.0f,0.0f,0.8f });
		}
		else {
			if (controlMino_->GetGender() == GenderType::Man) {
				block->sprite->SetColor({ 0,0,1,0.6f });
			}
			else if (controlMino_->GetGender() == GenderType::Woman) {
				block->sprite->SetColor({ 1.0f,0.08f,0.58f,0.6f });
			}
		}
	}
}

void MapField::InitCells() {
	const std::vector<std::vector<bool>> requiresMap = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,0,1,1,1,0,1,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	cellsPos_ = { 158.0f,280.0f };

	Vector2 cellSize = { cellsSize_,cellsSize_ };
	cellsData_.resize(15);
	for (i32 i = 0; std::vector<std::unique_ptr<CellData>>& row : cellsData_) {
		row.resize(15);

		for (i32 j = 0; std::unique_ptr<CellData>& cell : row) {
			cell = std::make_unique<CellData>();
			cell->genderType = GenderType::None;
			cell->isRequired = requiresMap[i][j];

			Vector3 cellPosition = { cellsPos_.x + (j * cellsSize_),cellsPos_.y + ((i)*cellsSize_), 0 };

			cell->block = std::make_unique<Sprite>();
			cell->block->Load("white2x2.png");
			cell->block->SetSize(cellSize);
			cell->block->SetPos(cellPosition);
			cell->block->SetColor({ 0.2f, 0.2f, 0.2f, 0.0f });

			cell->background = std::make_unique<Sprite>();
			cell->background->Load("white2x2.png");
			cell->background->SetPos(cellPosition);
			cell->background->SetColor({ 0.5f, 0.5f, 0.5f, 0.5f });
			cell->background->SetSize(cellSize / 2);

			cell->required = std::make_unique<Sprite>();
			cell->required->Load("white2x2.png");
			cell->required->SetPos(cellPosition);
			cell->required->SetSize(cellSize);
			if (cell->isRequired) {
				cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f });
			}
			else {
				cell->required->SetColor({ 0.0f,0.0f,0.0f,0.0f });
			}

			++j;
		}
		++i;
	}
}

void MapField::LoadMinoTables() {
	minoTables.clear();

	for (auto& file : std::filesystem::directory_iterator("resource/Json/minos/")) {
		nlohmann::json json = JsonSerializer::DeserializeJsonData(file.path().string());

		auto& table = minoTables.emplace_back();
		table.friendlyType = json["Type"];
		for (auto& minoJson : json["Minos"]) {
			std::unique_ptr<Mino> newMino = std::make_unique<Mino>();

			newMino->Initialize();
			newMino->Load(minoJson, this);

			if (newMino->GetGender() == GenderType::Man) {
				++table.numManMino;
			}
			else {
				++table.numWomanMino;
			}

			table.minos.emplace_back(std::move(newMino));
		}

		std::ranges::sort(table.minos, [](const std::unique_ptr<Mino>& lhs, const std::unique_ptr<Mino>& rhs) -> bool {
			if (lhs->GetGender() != rhs->GetGender()) {
				// 性別が異なる場合、男性を優先
				return lhs->GetGender() == GenderType::Man;
			}
			// ブロック数が異なる場合、ブロック数の多い方を優先
			return lhs->GetBlocks().size() >= rhs->GetBlocks().size();
		});
	}
}
