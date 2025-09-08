#include "MapField.h"

#include "Game/GameObj/Climber/Climber.h"
#include "Game/Collider/CollisionManager.h"
#include "Engine//Math/Random/Random.h"
#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"


MapField::MapField() {}

MapField::~MapField() {}

void MapField::Initialize() {

	map_ = std::vector(15, std::vector<int>(kMapWidth_));

	panelTex_ = std::make_unique<Sprite>();
	panelTex_->Load("blockBox.png");
	panelTex_->SetPos({ 285.0f,102.5f,0.0f });
	for (int i = 0; i < 7; i++) {
		std::unique_ptr<Sprite> button;
		button = std::make_unique<Sprite>();
		button->Load("white2x2.png");
		if (i == 0) button->SetColor({ 1.0f,0.65f,0.0f,1.0f });
		if (i == 1) button->SetColor({ 0.55f,0.0f,0.55f,1.0f });
		if (i == 2) button->SetColor({ 0.0f,0.5f,0.0f,1.0f });
		if (i == 3) button->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		if (i == 4) button->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		if (i == 5) button->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		if (i == 6) button->SetColor({ 0.0f,0.8f,0.95f,1.0f });
		button->SetSize({ 55.0f,37.5f });
		button->SetPos({ 185.0f,95.0f,0.0f });
		buttonTex_.push_back(std::move(button));
	}
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

	for (int i = 0; i < 7; i++) {
		BlockType type = BlockType::L;
		if (i == 1) type = BlockType::T;
		if (i == 2) type = BlockType::S;
		if (i == 3) type = BlockType::Z;
		if (i == 4) type = BlockType::O;
		if (i == 5) type = BlockType::J;
		if (i == 6) type = BlockType::I;
		selectTypes_.push_back(type);
	}
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
}

void MapField::Update() {
	if (controlMino_) {
		haveControlMino_ = true;
	} else {
		haveControlMino_ = false;
	}
	UpdateControlMino();
	UpdateSelectPanel();

	for (auto& mino : minos_) {
		mino->Update();
	}

}

void MapField::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	if (controlMino_) {
		/*controlMino_->Draw();
		if (futureMino_ && canQuickDrop_) {
			futureMino_->DrawLine();
		}*/
	}
	/*for (auto& mino : minos_) {
		mino->Draw();
	}*/

	factoryTex_->Draw();
	enemyFactoryTex_->Draw();
}

void MapField::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("MapField")) {
		if (ImGui::Button("Complete")) {
			CompleteArragement();
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
	map_ = {
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
	for (size_t i = 0; i < cells_.size(); i++) {
		for (size_t j = 0; j < cells_[i].size(); j++) {
			if (cells_[i][j] && typeCells_[i][j]) {
				if (map_[i][j] == 2) {
					arrangementCells_[i][j]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}
			}
		}
	}
}

void MapField::TitleUpdate() {
	if (controlMino_) {
		haveControlMino_ = true;
	} else {
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
	frameTex_->Draw();
	//completeTex_->Draw();
	if (mapSizeNum_ != 2) {
		arrowLTex_->Draw();
	}
	if (mapSizeNum_ != 0) {
		arrowRTex_->Draw();
	}
	factoryTex_->Draw();
	enemyFactoryTex_->Draw();
	mapSizeTex_->Draw();
	panelTex_->Draw();
	//selectorTex_->Draw();
	for (int i = 0; i < buttonTex_.size(); i++) {
		if (blockButtonNum_ == i) {
			buttonTex_[i]->Draw();
		}
	}
	genderPanelTex_->Draw();

	for (size_t i = 0; i < cells_.size(); i++) {
		for (size_t j = 0; j < cells_[i].size(); j++) {
			if (cells_[i][j] && typeCells_[i][j]) {
				cells_[i][j]->Draw();
				if (TypeMap_[i][j] == 1) {
					typeCells_[i][j]->Draw();
				}
			}
		}
	}
	ArrangementDraw();
	for (size_t i = 0; i < cells_.size(); i++) {
		for (size_t j = 0; j < cells_[i].size(); j++) {
			if (cells_[i][j] && typeCells_[i][j]) {
				if (map_[i][j] >= 1) {
					arrangementCells_[i][j]->Draw();
				}
			}
		}
	}
}

void MapField::BackDraw() {
	//BackPanelTex_->Draw();
}

void MapField::FactoryDraw() {
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
	panelTex_->Draw();
	selectorTex_->Draw();
	for (int i = 0; i < buttonTex_.size(); i++) {
		if (blockButtonNum_ == i) {
			buttonTex_[i]->Draw();
		}
	}
	genderPanelTex_->Draw();

	if (frameMoveTime_ == 0.0f) {
		for (size_t i = 0; i < cells_.size(); i++) {
			for (size_t j = 0; j < cells_[i].size(); j++) {
				if (cells_[i][j] && typeCells_[i][j]) {
					cells_[i][j]->Draw();
					if (TypeMap_[i][j] == 1) {
						typeCells_[i][j]->Draw();
					}
				}
			}
		}
		ArrangementDraw();
		for (size_t i = 0; i < cells_.size(); i++) {
			for (size_t j = 0; j < cells_[i].size(); j++) {
				if (cells_[i][j] && typeCells_[i][j]) {
					if (map_[i][j] >= 1) {
						arrangementCells_[i][j]->Draw();
					}
				}
			}
		}
	}

	BackPanelTex_->Draw();
}

void MapField::CursorDraw() {
	if (haveControlMino_) {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		grabCursorTex_->SetPos({ mouse.x,mouse.y,0.0f });
		grabCursorTex_->Draw();
	} else {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });
		cursorTex_->Draw();
	}
}

void MapField::ArrangementDraw() {
	if (!controlMino_) return;
	switch (controlMino_->GetBlockType()) {
		case BlockType::L:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 2.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1.0f)]->Draw();
			break;
		case BlockType::T:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1.0f)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1.0f)]->Draw();
			break;
		case BlockType::S:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1.0f)]->Draw();
			break;
		case BlockType::Z:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x - 1.0f)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1.0f)]->Draw();
			break;
		case BlockType::O:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1.0f)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)]->Draw();
			break;
		case BlockType::J:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 2.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1.0f)]->Draw();
			break;
		case BlockType::I:
			arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 1.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 2.0f)][int(cellNum_.x)]->Draw();
			arrangementCells_[int(cellNum_.y - 3.0f)][int(cellNum_.x)]->Draw();
			break;
		default:
			break;
	}
}

void MapField::UpdateSelectPanel() {
	if (!controlMino_) {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();

		// blockどれつかむか
		Vector3 pos = buttonTex_[blockButtonNum_]->GetPos();   // 中心座標
		Vector2 size = buttonTex_[blockButtonNum_]->GetSize(); // 幅・高さ
		float halfW = size.x * 0.5f;
		float halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			minoButtonNum_ = 0;
			if (manPanelTime_ == defaultSelectPanelTime_ || womanPanelTime_ == defaultSelectPanelTime_) {
				if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
					if (!controlMino_) {
						AddMino(selectTypes_[blockButtonNum_]);
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
		Vector3 pos2 = panelTex_->GetPos();
		Vector2 size2 = panelTex_->GetSize();
		float halfW2 = size.x * 0.5f;
		float halfH2 = size.y * 0.5f;
		if ((mouse.x >= pos.x - halfW && mouse.x <= pos.x && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) ||
			((mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2) && gender_ == int(GenderType::Man))) {
			if ((mouse.x >= pos.x - halfW && mouse.x <= pos.x && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH)) {
				gender_ = int(GenderType::Man);
				GenderColor();
			}
			if (womanPanelTime_ <= 0.0f) {
				manPanelTime_ += FPSKeeper::DeltaTime();
				manPanelTime_ = std::clamp(manPanelTime_, 0.0f, defaultSelectPanelTime_);
			}
		} else {
			manPanelTime_ -= FPSKeeper::DeltaTime();
			manPanelTime_ = std::clamp(manPanelTime_, 0.0f, defaultSelectPanelTime_);
		}
		if ((mouse.x >= pos.x && mouse.x <= pos.x + halfW && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) ||
			((mouse.x >= pos2.x - halfW2 && mouse.x <= pos2.x + halfW2 && mouse.y >= pos2.y - halfH2 && mouse.y <= pos2.y + halfH2) && gender_ == int(GenderType::Woman))) {
			if ((mouse.x >= pos.x && mouse.x <= pos.x + halfW && mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH)) {
				gender_ = int(GenderType::Woman);
				GenderColor();
			}
			if (manPanelTime_ <= 0.0f) {
				womanPanelTime_ += FPSKeeper::DeltaTime();
				womanPanelTime_ = std::clamp(womanPanelTime_, 0.0f, defaultSelectPanelTime_);
			}
		} else {
			womanPanelTime_ -= FPSKeeper::DeltaTime();
			womanPanelTime_ = std::clamp(womanPanelTime_, 0.0f, defaultSelectPanelTime_);
		}

		// 完了を押す
		pos = completeTex_->GetPos();
		size = completeTex_->GetSize();
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_) {
					CompleteArragement();

				}
			}
		}
		// マップサイズの変更
		pos = arrowLTex_->GetPos();
		size = arrowLTex_->GetSize();
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && minos_.size() == 0 && mapSizeNum_ != 2) {
					mapSizeNum_++;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
					isSmallChange_ = true;
				}
			}
		}
		pos = arrowRTex_->GetPos();
		size = arrowRTex_->GetSize();
		halfW = size.x * 0.5f;
		halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && minos_.size() == 0 && mapSizeNum_ != 0) {
					mapSizeNum_--;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
				}
			}
		}
	}
	ArrowUpdate();
	FrameUpdate();
	SelectMino();
	//ReturenSelectMino();
}

void MapField::SelectMino() {
	float panelTime = 0.0f;
	if (manPanelTime_ > 0.0f) panelTime = manPanelTime_;
	if (womanPanelTime_ > 0.0f) panelTime = womanPanelTime_;
	if (panelTime != 0.0f) {
		float t = (panelTime / defaultSelectPanelTime_);
		float posY = std::lerp(37.0f, panelTexturePosY_, t);
		panelTex_->SetPos({ 285.0f,posY,0.0f });

		posY = std::lerp(30.0f, 95.0f, t);
		for (int i = 0; i < selectTypes_.size(); i++) {
			buttonTex_[int(selectTypes_[i])]->SetPos({ 185.0f, posY, 0.0f });
		}

		if (minoButtonNum_ == 0) {
			selectorTex_->SetPos({ 185.0f,posY, 0.0f });
		} else if (minoButtonNum_ == 1) {
			selectorTex_->SetPos({ 335.0f,posY, 0.0f });
		} else if (minoButtonNum_ == 2) {
			selectorTex_->SetPos({ 435.0f,posY, 0.0f });
		}

	} else {
		panelTex_->SetPos({ 285.0f,37.0f,0.0f });
		for (int i = 0; i < selectTypes_.size(); i++) {
			buttonTex_[int(selectTypes_[i])]->SetPos({ 185.0f, 30.0f, 0.0f });
		}
		if (minoButtonNum_ == 0) {
			selectorTex_->SetPos({ 185.0f,30.0f, 0.0f });
		} else if (minoButtonNum_ == 1) {
			selectorTex_->SetPos({ 335.0f,30.0f, 0.0f });
		} else if (minoButtonNum_ == 2) {
			selectorTex_->SetPos({ 435.0f,30.0f, 0.0f });
		}
	}

	if (minoButtonNum_ == 0) {
		selectorMaxSize_ = { 150.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 150.0f + 10.0f,40.0f + 10.0f };
	} else if (minoButtonNum_ == 1) {
		selectorMaxSize_ = { 50.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 50.0f + 10.0f,40.0f + 10.0f };
	} else if (minoButtonNum_ == 2) {
		selectorMaxSize_ = { 50.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 50.0f + 10.0f,40.0f + 10.0f };
	}
	selectorSizeTime_ += FPSKeeper::DeltaTime() * 0.25f;
	selectorSizeTime_ = fmodf(selectorSizeTime_, std::numbers::pi_v<float>);
	// sin波で 0.0 ～ 1.0 に正規化
	float t = (sinf(selectorSizeTime_) + 1.0f) * 0.5f;
	// 最小サイズと最大サイズを補間
	Vector2 size;
	size.x = selectorMinSize_.x + (selectorMaxSize_.x - selectorMinSize_.x) * t;
	size.y = selectorMinSize_.y + (selectorMaxSize_.y - selectorMinSize_.y) * t;
	// サイズを反映
	selectorTex_->SetSize(size);
	selectorDeleteSize_ = size;
}

void MapField::ReturenSelectMino() {
	/*if (controlMino_) {
		float panelTime = 0.0f;
		if (manPanelTime_ > 0.0f) panelTime = manPanelTime_;
		if (womanPanelTime_ > 0.0f) panelTime = womanPanelTime_;
		if (panelTime != 0.0f) {
			float t = 1.0f - (selectPanelTime_ / defaultSelectPanelTime_);
			float posY = std::lerp(panelTexturePosY_, -200.0f, t);
			panelTex_->SetPos({ 320.0f,posY,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 320.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
			for (int i = 0; i < selectTypes_.size(); i++) {
				buttonTex_[int(selectTypes_[i])]->SetSize({ (panelSize_.x / float(selectTypes_.size())) * 0.8f ,panelSize_.y * 0.8f });

				float x = startX + i * space;
				buttonTex_[int(selectTypes_[i])]->SetPos({ x, posY, 0.0f });
			}

			float x = startX + float(minoButtonNum_) * space;
			selectorTex_->SetPos({ x, posY, 0.0f });
			selectorDeleteSize_.x = std::lerp(selectorDeleteSize_.x, 10.0f, 0.01f);
			selectorDeleteSize_.y = std::lerp(selectorDeleteSize_.y, 10.0f, 0.01f);
			selectorTex_->SetSize(selectorDeleteSize_);
		} else {
			panelTex_->SetPos({ 320.0f,-200.0f,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 320.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
			for (int i = 0; i < selectTypes_.size(); i++) {
				buttonTex_[int(selectTypes_[i])]->SetSize({ (panelSize_.x / float(selectTypes_.size())) * 0.8f ,panelSize_.y * 0.8f });
				float x = startX + i * space;
				buttonTex_[int(selectTypes_[i])]->SetPos({ x, -200.0f, 0.0f });
			}

			float x = startX + float(minoButtonNum_) * space;
			selectorTex_->SetPos({ x, -200.0f, 0.0f });
			selectorDeleteSize_.x = std::lerp(selectorDeleteSize_.x, 10.0f, 0.01f);
			selectorDeleteSize_.y = std::lerp(selectorDeleteSize_.y, 10.0f, 0.01f);
			selectorTex_->SetSize(selectorDeleteSize_);
		}
	}*/
}

void MapField::AddMino(BlockType type) {
	if (controlMino_) return;
	switch (type) {
		case BlockType::L:
			cellNum_ = { 4.0f, 2.0f };
			break;
		case BlockType::T:
			cellNum_ = { 4.0f, 1.0f };
			break;
		case BlockType::S:
			cellNum_ = { 4.0f, 1.0f };
			break;
		case BlockType::Z:
			cellNum_ = { 4.0f, 1.0f };
			break;
		case BlockType::O:
			cellNum_ = { 4.0f, 1.0f };
			break;
		case BlockType::J:
			cellNum_ = { 4.0f, 2.0f };
			break;
		case BlockType::I:
			cellNum_ = { 4.0f, 3.0f };
			break;
		default:
			break;
	}
	//if (map_[int(cellNum_.y)][int(cellNum_.x)] == 1) return;
	std::unique_ptr<Mino> mino;
	mino = std::make_unique<Mino>();
	mino->Initialize();
	mino->SetCollisionMana(cMana_);
	mino->InitBlock(type, GenderType(gender_));

	controlMino_ = std::move(mino);
	float oldDistance = GetOldDistance();
	controlMino_->GetTransform().translate = { cellsPos_.x + (cellNum_.x) * 2.0f,cellsPos_.y + (15.0f - cellNum_.y) * 2.0f + oldDistance,0.0f };

	futureMino_ = std::make_unique<Mino>();
	futureMino_->Initialize();
	futureMino_->InitBlock(type, GenderType(gender_));
	futureMino_->GetTransform().translate = { (cellNum_.x) * 2.0f,(15.0f - cellNum_.y) * 2.0f + oldDistance,0.0f };
	FutureMinoUpdate();

	//selectPanelTime_ = defaultSelectPanelTime_;
}

void MapField::UpdateControlMino() {
	if (!controlMino_) return;

	/*downTime_ -= FPSKeeper::DeltaTime();
	if (downTime_ <= 0.0f) {
		downTime_ = 60.0f;
		CellCheck();
		controlMino_->GetModel()->transform.translate = { (cellNum_.x) * 2.0f,(20.0f - cellNum_.y) * 2.0f,0.0f };
	}*/

	MoveControlMino();

	controlMino_->Update();
	if (Input::GetInstance()->IsTriggerMouse(0) && haveControlMino_) {
		CellSet();
		return;
	}
	if (futureMino_) {
		futureMino_->Update();
	}
	RemoveControlMino();

}

void MapField::ArrowUpdate() {
	arrowMoveTime_ += FPSKeeper::DeltaTime() * 0.1f;
	arrowMoveTime_ = fmodf(arrowMoveTime_, std::numbers::pi_v<float>);
	// sin波で 0.0 ～ 1.0 に正規化
	float t = fabsf((sinf(arrowMoveTime_) + 1.0f) * 0.5f);
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
			frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
			frameTex_->SetPos({ -290.0f, 400.0f,0.0f });
			isSmallChange_ = false;
			return;
		}

		float t = 1.0f - (frameMoveTime_ / 30.0f);
		if (isSmallChange_) {
			float mainPos = std::lerp(285.0f, 880.0f, t);
			float subPos = std::lerp(-290.0f, 285.0f, t);

			frameTex_->SetPos({ mainPos,400.0f,0.0f });
			subFrameTex_->SetPos({ subPos,400.0f,0.0f });
		} else {
			float mainPos = std::lerp(285.0f, -290.0f, t);
			float subPos = std::lerp(880.0f, 285.0f, t);

			frameTex_->SetPos({ mainPos,400.0f,0.0f });
			subFrameTex_->SetPos({ subPos,400.0f,0.0f });
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

	switch (controlMino_->GetBlockType()) {
		case BlockType::L:
			if (int(nextCell.x) <= -1 || int(nextCell.x) >= 14
				|| int(nextCell.y) <= 1) {
				nextCell.x = std::clamp(nextCell.x, 0.0f, float(gridSize - 2));
				nextCell.y = std::clamp(nextCell.y, 2.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 2.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 2.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::T:
			if (int(nextCell.x) <= 0 || int(nextCell.x) >= 14
				|| int(nextCell.y) <= 0) {
				nextCell.x = std::clamp(nextCell.x, 1.0f, float(gridSize - 2));
				nextCell.y = std::clamp(nextCell.y, 1.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x - 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x - 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::S:
			if (int(nextCell.x) <= 0 || int(nextCell.x) >= 14
				|| int(nextCell.y) <= 0) {
				nextCell.x = std::clamp(nextCell.x, 1.0f, float(gridSize - 2));
				nextCell.y = std::clamp(nextCell.y, 1.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x - 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x - 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::Z:
			if (int(nextCell.x) <= 0 || int(nextCell.x) >= 14
				|| int(nextCell.y) <= 0) {
				nextCell.x = std::clamp(nextCell.x, 1.0f, float(gridSize - 2));
				nextCell.y = std::clamp(nextCell.y, 1.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x - 1.0f)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x - 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::O:
			if (int(nextCell.x) <= -1 || int(nextCell.x) >= 14
				|| int(nextCell.y) <= 0) {
				nextCell.x = std::clamp(nextCell.x, 0.0f, float(gridSize - 2));
				nextCell.y = std::clamp(nextCell.y, 1.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x + 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x + 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::J:
			if (int(nextCell.x) <= 0 || int(nextCell.x) >= 15
				|| int(nextCell.y) <= 1) {
				nextCell.x = std::clamp(nextCell.x, 1.0f, float(gridSize - 1));
				nextCell.y = std::clamp(nextCell.y, 2.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 2.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 2.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y)][int(nextCell.x - 1.0f)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x - 1.0f)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		case BlockType::I:
			if (int(nextCell.x) <= -1 || int(nextCell.x) >= 15
				|| int(nextCell.y) <= 2) {
				nextCell.x = std::clamp(nextCell.x, 0.0f, float(gridSize - 1));
				nextCell.y = std::clamp(nextCell.y, 3.0f, float(gridSize));
			}
			if (map_[int(nextCell.y)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 1.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 1.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 2.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 2.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			if (map_[int(nextCell.y - 3.0f)][int(nextCell.x)] >= 1) arrangementCells_[int(nextCell.y - 3.0f)][int(nextCell.x)]->SetColor({ 1.0f,0.0f,0.0f,0.6f });
			break;
		default:
			break;
	}

	cellNum_ = nextCell;

	float oldDistance = GetOldDistance();
	controlMino_->GetTransform().translate = { cellsPos_.x + (cellNum_.x) * 2.0f,cellsPos_.y + (15.0f - cellNum_.y) * 2.0f + oldDistance,0.0f };
	FutureMinoUpdate();

	if (climber_) {
		if (climber_->CanAvoidBlock()) {
			climber_->AvoidFeatureBlock();
			canQuickDrop_ = true;
		} else {
			canQuickDrop_ = false;
		}
	}
}

void MapField::CellCheck() {
	switch (controlMino_->GetBlockType()) {
		case BlockType::L:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::T:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::S:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1 || map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::Z:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1 || map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::O:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::J:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		case BlockType::I:

			if (int(cellNum_.y + 1.0f) == 15 || map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
				controlMino_->SetBlockMode(BlockMode::Stay);
				return;
			}
			break;
		default:
			break;
	}
	cellNum_.y++;
}

void MapField::QuickDrop() {
	if (!controlMino_ || !canQuickDrop_) return;

	if (controlMino_) {
		while (controlMino_->GetBlockMode() == BlockMode::Fall) {
			CellCheck();
		}
		float oldDistance = GetOldDistance();
		controlMino_->GetTransform().translate = { cellsPos_.x + (cellNum_.x) * 2.0f,cellsPos_.y + (15.0f - cellNum_.y) * 2.0f + oldDistance,0.0f };
		controlMino_->Update();
		RemoveControlMino();
	}
}

void MapField::CellSet() {
	if (!controlMino_) return;

	if (controlMino_) {
		if (!ArrangementCheck()) {
			return;
		}
		controlMino_->SetBlockMode(BlockMode::Stay);
		controlMino_->Update();
		RemoveControlMino();
	}
}

bool MapField::ArrangementCheck() {
	bool result = false;
	switch (controlMino_->GetBlockType()) {
		case BlockType::L:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 2)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x + 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::T:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x - 1)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x + 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::S:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x + 1)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x - 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::Z:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x - 1)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x + 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::O:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x + 1)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x + 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::J:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 2)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y)][int(cellNum_.x - 1)] >= 1) {
				return result;
			}
			break;
		case BlockType::I:
			if (int(cellNum_.y) == 15 ||
				map_[int(cellNum_.y)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 1)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 2)][int(cellNum_.x)] >= 1 || map_[int(cellNum_.y - 3)][int(cellNum_.x)] >= 1) {
				return result;
			}
			break;
		default:
			break;
	}
	result = true;
	return result;
}

void MapField::CompleteArragement() {
	if (controlMino_) return;
	int manBlocks = 0;
	int womanBlocks = 0;

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (TypeMap_[i][j] == 1 && map_[i][j] == 1) {
				manBlocks++;
			} else if (TypeMap_[i][j] == 1 && map_[i][j] == 2) {
				womanBlocks++;
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
	} else {
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

	map_ = std::vector(15, std::vector<int>(kMapWidth_));
	minos_.clear();
	blockButtonNum_ = Random::GetInt(0, 6);
}

void MapField::SetColliderManager(CollisionManager* cMana) {
	cMana_ = cMana;
}

void MapField::SetClimber(Climber* climber) {
	climber_ = climber;
}

void MapField::SetFriendlyManager(FriendlyManager* friendlyManager) {
	friendlyManager_ = friendlyManager;
}

const std::vector<int>& MapField::GetMapRows(size_t row) const {
	return map_[row];
}

const Mino* MapField::GetFeatureMino() const {
	return futureMino_.get();
}

std::pair<int, int> MapField::CalcFieldGrid(const Vector3& pos) const {
	return {
		static_cast<int>(GetMapHeight() - (pos.y - GetOldDistance()) / 2.0f),
		static_cast<int>(pos.x / 2.0f)
	};
}

const float MapField::GetOldDistance() const {
	float growLine = float(oldLine_) * 2.0f;
	growLine *= float(old_);
	growLine += float(old_) * nextSpace_;
	return (growLine);
}

void MapField::RemoveControlMino() {
	if (controlMino_->GetBlockMode() == BlockMode::Stay) {
		switch (controlMino_->GetBlockType()) {
			case BlockType::L:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 2;
				}

				break;
			case BlockType::T:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 2;
				}

				break;
			case BlockType::S:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 2;
				}

				break;
			case BlockType::Z:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x - 1.0f)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x - 1.0f)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 2;
				}

				break;
			case BlockType::O:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 2;
				}

				break;
			case BlockType::J:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 2;
				}

				break;
			case BlockType::I:
				if (controlMino_->GetGender() == GenderType::Man) {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 1;
					map_[int(cellNum_.y - 3.0f)][int(cellNum_.x)] = 1;
				} else {
					map_[int(cellNum_.y)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 2;
					map_[int(cellNum_.y - 3.0f)][int(cellNum_.x)] = 2;
				}

				break;
			default:
				break;
		}

		if (climber_) {
			climber_->OnDropped();
		}

		minos_.push_back(std::move(controlMino_));
		controlMino_ = nullptr;
		//selectPanelTime_ = defaultSelectPanelTime_;
	}
}

void MapField::FutureMinoUpdate() {
	if (!futureMino_) return;
	Vector2 cell = cellNum_;
	futureMino_->SetBlockMode(BlockMode::Fall);
	while (futureMino_->GetBlockMode() == BlockMode::Fall) {
		switch (futureMino_->GetBlockType()) {
			case BlockType::L:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x + 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::T:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x + 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x - 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::S:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x - 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y)][int(cell.x + 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::Z:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x + 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y)][int(cell.x - 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::O:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x + 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::J:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x - 1.0f)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			case BlockType::I:

				if (int(cell.y + 1.0f) == 15) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}
				if (map_[int(cell.y + 1.0f)][int(cell.x)] == 1) {
					futureMino_->SetBlockMode(BlockMode::Stay);
					break;
				}

				break;
			default:
				break;
		}

		if (futureMino_->GetBlockMode() != BlockMode::Stay) {
			cell.y++;
		}
	}
	float oldDistance = GetOldDistance();
	futureMino_->GetTransform().translate = { (cell.x) * 2.0f,(15.0f - cell.y) * 2.0f + oldDistance,0.0f };
	futureMino_->Update();
}

void MapField::CellSpriteSetColor() {
	for (size_t i = 0; i < cells_.size(); i++) {
		for (size_t j = 0; j < cells_[i].size(); j++) {
			if (map_[i][j] == 1) {
				arrangementCells_[i][j]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
			}
			if (map_[i][j] == 2) {
				arrangementCells_[i][j]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
			}
		}
	}
	if (controlMino_) {
		switch (controlMino_->GetBlockType()) {
			case BlockType::L:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}

				break;
			case BlockType::T:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}

				break;
			case BlockType::S:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}


				break;
			case BlockType::Z:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x - 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x - 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}


				break;
			case BlockType::O:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x + 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}


				break;
			case BlockType::J:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x - 1)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}


				break;
			case BlockType::I:
				if (controlMino_->GetGender() == GenderType::Man) {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
					arrangementCells_[int(cellNum_.y - 3)][int(cellNum_.x)]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
				} else {
					arrangementCells_[int(cellNum_.y)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 1)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 2)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
					arrangementCells_[int(cellNum_.y - 3)][int(cellNum_.x)]->SetColor({ 1.0f,0.08f,0.58f,0.6f });
				}

				break;
			default:
				break;
		}
	}
}

void MapField::InitCells() {
	cellsPos_ = { 158.0f,280.0f };
	cells_.resize(15);
	for (int i = 0; i < 15; i++) {
		cells_[i].resize(15);
		for (int j = 0; j < 15; j++) {
			// unique_ptr の生成
			auto cell = std::make_unique<Sprite>();
			cell->Load("white2x2.png");
			cell->SetColor({ 0.2f,0.2f,0.2f,0.4f });
			cell->SetSize({ cellsSize_ - 6.0f,cellsSize_ - 6.0f });
			cell->SetPos({ cellsPos_.x + (j * cellsSize_),cellsPos_.y + ((i)*cellsSize_),0.0f });
			cells_[i][j] = std::move(cell);
		}
	}

	TypeMap_ = {
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
	typeCells_.resize(15);
	for (int i = 0; i < 15; i++) {
		typeCells_[i].resize(15);
		for (int j = 0; j < 15; j++) {
			// unique_ptr の生成
			auto cell = std::make_unique<Sprite>();
			cell->Load("white2x2.png");
			cell->SetColor({ 0.5f,0.5f,0.5f,1.0f });
			cell->SetSize({ 2.0f,2.0f });
			if (TypeMap_[i][j] == 1) {
				cell->SetSize({ cellsSize_,cellsSize_ });
				cell->SetColor({ 0.1f,0.1f,0.1f,0.5f });
			}
			cell->SetPos({ cellsPos_.x + (j * cellsSize_),cellsPos_.y + ((i)*cellsSize_),0.0f });
			typeCells_[i][j] = std::move(cell);
		}
	}

	arrangementCells_.resize(15);
	for (int i = 0; i < 15; i++) {
		arrangementCells_[i].resize(15);
		for (int j = 0; j < 15; j++) {
			// unique_ptr の生成
			auto cell = std::make_unique<Sprite>();
			cell->Load("white2x2.png");
			cell->SetColor({ 0.0f,0.0f,1.0f,0.6f });
			cell->SetSize({ cellsSize_,cellsSize_ });
			cell->SetPos({ cellsPos_.x + (j * cellsSize_),cellsPos_.y + ((i)*cellsSize_),0.0f });
			arrangementCells_[i][j] = std::move(cell);
		}
	}
}

void MapField::GenderColor() {
	for (size_t i = 0; i < cells_.size(); i++) {
		for (size_t j = 0; j < cells_[i].size(); j++) {
			if (map_[i][j] >= 1) return;
			if (gender_ == int(GenderType::Man)) {
				arrangementCells_[i][j]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
			} else if (gender_ == int(GenderType::Woman)) {
				arrangementCells_[i][j]->SetColor({ 1.0f, 0.08f, 0.58f, 0.6f });
			}
		}
	}
}
