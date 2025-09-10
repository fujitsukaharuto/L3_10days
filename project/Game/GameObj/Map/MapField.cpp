#include "MapField.h"

#include "Engine//Math/Random/Random.h"
#include "Engine/Editor/JsonSerializer.h"
#include "Engine/Input/Input.h"

#include "Engine/Math/Animation/Easing.h"

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
	selectorTex_->SetPos({ 100.0f,120.0f, 0.0f });
	selectorMaxSize_ = { 95.0f + 30.0f,90.0f + 30.0f };
	selectorMinSize_ = { 95.0f + 10.0f,90.0f + 10.0f };

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
	cursorTex_->SetSize({ 40.0f,40.0f });
	grabCursorTex_ = std::make_unique<Sprite>();
	grabCursorTex_->Load("grabCursor.png");
	grabCursorTex_->SetSize({ 40.0f,40.0f });

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

	menuButtonTex_ = std::make_unique<Sprite>();
	menuButtonTex_->Load("menuButton.png");
	menuButtonTex_->SetSize({ 50.0f, 50.0f });
	menuButtonTex_->SetRange({ 0.0f, 0.0f }, { 50.0f,50.0f });
	menuButtonTex_->SetPos({ 60.0f, 660.0f,0.0f });

	menuTex_ = std::make_unique<Sprite>();
	menuTex_->Load("menu.png");
	menuTex_->SetSize({ 800.0f, 500.0f });
	menuTex_->SetPos({ -450.0f, 460.0f,0.0f });

	LoadMinoTables();

	moldManager.load();

	InitCells();

	// テーブルと鋳型をランダムに決定
	RandomizeTable();
	ResetMold();
	// Sound
	push = &AudioPlayer::GetInstance()->SoundLoadWave("push.wav");
	grab = &AudioPlayer::GetInstance()->SoundLoadWave("grab.wav");
	returnWav = &AudioPlayer::GetInstance()->SoundLoadWave("return.wav");
	machine = &AudioPlayer::GetInstance()->SoundLoadWave("machine.wav");

	arrangement.AnimationTime = 1.0f;
	arrangement.timer = arrangement.AnimationTime;
	dontPushWav = &AudioPlayer::GetInstance()->SoundLoadWave("dontPush.wav");

	arrangement.genderRatioSprite = std::make_unique<Sprite>();
	arrangement.genderRatioSprite->Load("manwoman.png");
	arrangement.genderRatioSprite->SetPos({ 285.0f, 360.0f,0.0f });
	arrangement.genderRatioSprite->SetSize({ 400,Arrangement::GenderSpriteHeight });
	arrangement.genderRatioSprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	arrangement.genderRatioSprite->SetAngle(-PI / 8); // -30度傾ける

	arrangement.humanRatioSprite = std::make_unique<Sprite>();
	arrangement.humanRatioSprite->Load("human.png");
	arrangement.humanRatioSprite->SetPos({ 350, 420,0.0f });
	arrangement.humanRatioSprite->SetSize({ 400,Arrangement::HumanRatioHeight });
	arrangement.humanRatioSprite->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	arrangement.humanRatioSprite->SetAngle(-PI / 8); // -30度傾ける

	ParticleManager::Load(ClickEmit_, "sphere2");
	ClickEmit_.frequencyTime_ = 0.0f;
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
	UpdateCells();
}

void MapField::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	//factoryTex_->Draw();
	//enemyFactoryTex_->Draw();
	menuTex_->Draw();
	menuButtonTex_->Draw();
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
	const std::vector<std::vector<bool>> mold = {
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

	for (i32 rowI = 0; auto& row : cellsData_) {
		for (i32 colI = 0; auto& cell : row) {
			cell->genderType = (GenderType)map_[rowI][colI];
			if (cell->genderType == GenderType::Man) {
				cell->block->SetColor({ 0,0,1,0.6f });
			}
			else if (cell->genderType == GenderType::Woman) {
				cell->block->SetColor({ 1.0f,0.08f,0.58f,0.6f });
			}
			cell->isRequired = mold[rowI][colI];
			if (cell->isRequired) {
				cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f });
			}
			else {
				cell->required->SetColor({ 0.0f,0.0f,0.0f,0.0f });
			}
			++colI;
		}
		++rowI;
	}
	titleCompleteTex_ = std::make_unique<Sprite>();
	titleCompleteTex_->Load("titleCompleted.png");
	titleCompleteTex_->SetPos({ 285.0f, 660.0f,0.0f });
	titleCompleteTex_->SetAnchor({ 0.5f, 1.0f });
	titleCompleteTex_->SetPos({ 285.0f, 710.0f,0.0f });

}

void MapField::TitleUpdate() {
	if (controlMino_) {
		haveControlMino_ = true;
	}
	else {
		haveControlMino_ = false;
	}
	TitleUpdateSelectPanel();
}

void MapField::TitleUpdateSelectPanel() {
	if (!controlMino_) {
	}

	{
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		// 完了を押す
		Vector3 pos = titleCompleteTex_->GetPos();
		Vector2 size = titleCompleteTex_->GetSize();
		r32 halfW = size.x * 0.5f;
		r32 halfH = size.y;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_) {
					isTitleToGame_ = true;
					//CompleteArrangement();
					AudioPlayer::GetInstance()->SoundPlayWave(*push);
					AudioPlayer::GetInstance()->SoundPlayWave(*machine);
					CharaStatus status;
					status.gender = Gender::WOMAN;
					status.name = "womanWalk.gltf";
					friendlyManager_->AddFriendly(status);
					ClickEmit_.pos_ = { 2.71f,9.09f,0.0f };
					ClickEmit_.Emit();
				}
			}
			titleCompleteTex_->SetColor({ 0.6f,0.6f,0.6f,1.0f });
		}
		else {
			titleCompleteTex_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}

		if (isTitleToGame_) {
			titleToGameTime_ -= FPSKeeper::DeltaTime();
		}
	}

	{
		arrowMoveTime_ += FPSKeeper::DeltaTime() * 0.1f;
		arrowMoveTime_ = std::fmod(arrowMoveTime_, std::numbers::pi_v<float>);
		// sin波で 0.0 ～ 1.0 に正規化
		float t = std::abs((sinf(arrowMoveTime_) + 1.0f) * 0.5f);
		Vector2 size;
		size.x = 250.0f + (280.0f - 250.0f) * t;
		size.y = 100.0f + (80.0f - 100.0f) * t;
		titleCompleteTex_->SetSize({ size.x ,size.y });
	}

	//SelectMino();
}

bool MapField::TitleToGame() {
	if (titleToGameTime_ <= 0.0f) {
		return true;
	}
	return false;
}

void MapField::TitleDraw() {
	BackPanelTex_->Draw();

	factoryBackPanelTex_->Draw();
	frameTex_->Draw();
	titleCompleteTex_->Draw();
	//factoryTex_->Draw();
	//enemyFactoryTex_->Draw();
	//manPanelTex_->Draw();
	//womanPanelTex_->Draw();

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
	factoryBackPanelTex_->Draw();//
	frameTex_->Draw();//
	subFrameTex_->Draw();//
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
	}
	else {
		selectorTex_->Draw();
	}
	genderPanelTex_->Draw();

	BackPanelTex_->Draw();

	for (auto& tableMino : minoTables[tableIndex].minos) {
		tableMino->DrawButton();
	}

	if (frameMoveTime_ == 0.0f) {
		DrawCells();
	}

	arrangement.genderRatioSprite->Draw();
	arrangement.humanRatioSprite->Draw();
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

	// 矢印関連
	if (hideArrowAnimationTimer <= 2.0f) {
		if ((!canChangeMoldType && hideArrowAnimationTimer < 1.0f) ||
			(canChangeMoldType && hideArrowAnimationTimer >= 1.0f)) {
			hideArrowAnimationTimer += FPSKeeper::DeltaTime();
		}
		r32 alpha = hideArrowAnimation.GetValue(hideArrowAnimationTimer);
		arrowLTex_->SetColorAlpha(alpha);
		arrowRTex_->SetColorAlpha(alpha);
	}
	else {
		arrowLTex_->SetColorAlpha(1.0f);
		arrowRTex_->SetColorAlpha(1.0f);
	}

	UpdateMold();

	if (!isPoseMenu_) {
		Vector2 mouse = Input::GetInstance()->GetMousePosition();
		// 完了を押す
		Vector3 pos = completeTex_->GetPos();
		Vector2 size = completeTex_->GetSize();
		r32 halfW = size.x * 0.5f;
		r32 halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_ && arrangement.timer >= 1.0f) {
				if (!controlMino_ && canComplete) {
					CompleteArrangement();
					AudioPlayer::GetInstance()->SoundPlayWave(*push);
					AudioPlayer::GetInstance()->SoundPlayWave(*machine, 0.15f);
					ClickEmit_.pos_ = { 2.71f,9.09f,0.0f };
					ClickEmit_.Emit();
				}
				else {
					AudioPlayer::GetInstance()->SoundPlayWave(*dontPushWav);
				}
			}
			completeTex_->SetColor({ 0.6f,0.6f,0.6f,1.0f });
		}
		else {
			if (canComplete) {
				completeTex_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			}
		}
	}
	// ポーズメニュー
	PoseMenu();

	// 矢印の選択
	ArrowUpdate();

	// 背景の枠の移動
	FrameUpdate();

	// 選択中のアニメーション
	SelectMino();
}

void MapField::UpdateSelectPanelControlling() {
}

void MapField::UpdateSelectPanelUncontrolling() {
	if (isPoseMenu_) return;
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
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_) {
					int idx = minoButtonNum_;
					if (gender_ == int(GenderType::Man)) { ClickEmit_.pos_ = { -6.1f,36.85f,0.0f }; ClickEmit_.Emit(); }
					if (gender_ == int(GenderType::Woman)) { idx += 3; ClickEmit_.pos_ = { 11.9f,37.00f,0.0f }; ClickEmit_.Emit(); }
					controlMino_ = minoTables[tableIndex].minos[idx].get();
					AudioPlayer::GetInstance()->SoundPlayWave(*grab);
					return;
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
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_) {
					int idx = minoButtonNum_;
					if (gender_ == int(GenderType::Man)) { ClickEmit_.pos_ = { -1.1f,38.35f,0.0f }; ClickEmit_.Emit(); }
					if (gender_ == int(GenderType::Woman)) { idx += 3; ClickEmit_.pos_ = { 7.3f,38.55f,0.0f }; ClickEmit_.Emit(); }
					controlMino_ = minoTables[tableIndex].minos[idx].get();
					AudioPlayer::GetInstance()->SoundPlayWave(*grab);
					return;
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
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_) {
					int idx = minoButtonNum_;
					if (gender_ == int(GenderType::Man)) { ClickEmit_.pos_ = { -1.7f,35.85f,0.0f }; ClickEmit_.Emit(); }
					if (gender_ == int(GenderType::Woman)) { idx += 3; ClickEmit_.pos_ = { 7.2f,35.55f,0.0f }; ClickEmit_.Emit(); }
					controlMino_ = minoTables[tableIndex].minos[idx].get();
					// AddMino(selectTypes_[blockButtonNum_]);
					AudioPlayer::GetInstance()->SoundPlayWave(*grab);
					return;
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
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH &&
			canChangeMoldType) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && /*minos_.size() == 0 && */mapSizeNum_ != 2) {
					mapSizeNum_++;
					--moldType;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					hideArrowAnimationTimer = 0.0f;
					hideMoldAnimationTimer = 0.0f;
					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
					subFrameTex_->SetPos({ -290.0f, 400.0f,0.0f });
					isSmallChange_ = true;
					AudioPlayer::GetInstance()->SoundPlayWave(*push);
					ClickEmit_.pos_ = { -8.9f,22.09f,0.0f };
					ClickEmit_.Emit();
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
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH &&
			canChangeMoldType) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!controlMino_ && /*minos_.size() == 0 && */ mapSizeNum_ != 0) {
					mapSizeNum_--;
					++moldType;
					mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

					hideArrowAnimationTimer = 0.0f;
					hideMoldAnimationTimer = 0.0f;
					frameMoveTime_ = 30.0f;
					frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
					subFrameTex_->SetPos({ 880.0f, 400.0f,0.0f });
					isSmallChange_ = false;
					AudioPlayer::GetInstance()->SoundPlayWave(*push);
					ClickEmit_.pos_ = { 15.1f,22.09f,0.0f };
					ClickEmit_.Emit();
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
		}
		else if (minoButtonNum_ == 1) {
			if (gender_ == int(GenderType::Man)) { selectorTex_->SetPos({ 195.0f,95.0f, 0.0f }); nowSelectorTex_->SetPos({ 195.0f,85.0f, 0.0f }); }
			if (gender_ == int(GenderType::Woman)) { selectorTex_->SetPos({ 375.0f,95.0f, 0.0f }); nowSelectorTex_->SetPos({ 375.0f,85.0f,0.0f }); }
			nowSelectorTex_->SetSize({ 68.0f,64.0f });
		}
		else if (minoButtonNum_ == 2) {
			if (gender_ == int(GenderType::Man)) { selectorTex_->SetPos({ 195.0f,146.0f, 0.0f }); nowSelectorTex_->SetPos({ 195.0f,146.0f, 0.0f }); }
			if (gender_ == int(GenderType::Woman)) { selectorTex_->SetPos({ 375.0f,146.0f, 0.0f }); nowSelectorTex_->SetPos({ 376.5f,146.0f,0.0f }); }
			nowSelectorTex_->SetSize({ 67.0f,45.0f });
		}

		if (minoButtonNum_ == 0) {
			selectorMaxSize_ = { 95.0f + 30.0f,90.0f + 30.0f };
			selectorMinSize_ = { 95.0f + 10.0f,90.0f + 10.0f };
		}
		else if (minoButtonNum_ == 1) {
			selectorMaxSize_ = { 63.0f + 30.0f,40.0f + 30.0f };
			selectorMinSize_ = { 63.0f + 10.0f,40.0f + 10.0f };
		}
		else if (minoButtonNum_ == 2) {
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
	else if (Input::GetInstance()->IsTriggerMouse(1) && haveControlMino_) {
		controlMino_ = nullptr;
		AudioPlayer::GetInstance()->SoundPlayWave(*returnWav);
		return;
	}
}

void MapField::UpdateCells() {
	// 完了時のアニメーション
	UpdateArrangementAnimation();
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
			block->sprite->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}
		else {
			if (controlMino_->GetGender() == GenderType::Man) {
				block->sprite->SetColor({ 0.0f,0.0f,1.0f,0.6f });
			}
			else {
				block->sprite->SetColor({ 1.0f,0.08f,0.58f,0.6f });
			}
		}
	}

	cellNum_ = nextCell;
}

void MapField::CellSet() {
	if (CanArrangement()) {
		controlMino_->Update();
		RemoveControlMino();
		if (canChangeMoldType) {
			hideArrowAnimationTimer = 0.0f;
		}
		canChangeMoldType = false; // ミノを配置したら鋳型変更不可にする

		AudioPlayer::GetInstance()->SoundPlayWave(*returnWav);
		return;
	}
	AudioPlayer::GetInstance()->SoundPlayWave(*dontPushWav, 0.6f);
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

void MapField::ResetBlocks() {
	for (auto& rows : cellsData_) {
		for (auto& cell : rows) {
			if (cell->block) {
				cell->block->SetColor({ 0.0f,0.0f,0.0f,0.0f });
			}
		}
	}
}

void MapField::CompleteArrangement() {
	if (controlMino_) return;
	int manBlocks = 0;
	int womanBlocks = 0;
	int stickOutBlocks = 0;

	for (auto& row : cellsData_) {
		for (auto& cell : row) {
			switch (cell->genderType) {
			case GenderType::Man:
				if (cell->isRequired) {
					++manBlocks;
				}
				else {
					++stickOutBlocks;
				}
				break;
			case GenderType::Woman:
				if (cell->isRequired) {
					++womanBlocks;
				}
				else {
					++stickOutBlocks;
				}
				break;
			default:
				break;
			}
		}
	}
	int maxBlocks = manBlocks + womanBlocks;

	CulGender(maxBlocks, manBlocks, womanBlocks, stickOutBlocks);

	maxB_.push_back(maxBlocks);
	manB_.push_back(manBlocks);
	womanB_.push_back(womanBlocks);

	i32 humanPower = std::clamp((maxBlocks * 5) / moldSize, 0, 4);
	ResetArrangementAnimation(humanPower);
}

void MapField::CulGender(int maxBlocks, int manBlocks, int womanBlocks, int stickOutBlocks) {
	// 最大パワー
	constexpr float maxPower = 50.0f;
	// 最小パワー
	constexpr float minPower = 0.0f;

	// 人間生成処理
	// HP処理
	i32 hp = kCellNum_ - moldSize - stickOutBlocks;
	float genderLevel = 0.0f;
	float t = 1.0f - (float(moldSize) - float(maxBlocks)) / float(moldSize);
	float power = Lerp(minPower, maxPower, t);

	// TODO: 5種類に増やす
	// 女ブロックの方が多い
	if (manBlocks < womanBlocks) {

		arrangement.status.hp = hp;
		arrangement.status.power = uint32_t(power);
		arrangement.status.gender = WOMAN;

		genderLevel = (float(womanBlocks) / float(maxBlocks)) * 100.0f;

		if (90.0f <= genderLevel) {	// とても女
			arrangement.status.name = "womanWalk.gltf";
			genderRatio = 4;
		}
		else { // 割と女
			arrangement.status.name = "womanWalk2.gltf";
			genderRatio = 3;
		}
	}
	else {
		arrangement.status.hp = hp;
		arrangement.status.power = uint32_t(power);
		arrangement.status.gender = MAN;
		genderLevel = (float(manBlocks) / float(maxBlocks)) * 100.0f;

		if (90.0f <= genderLevel) {	// とても男
			arrangement.status.name = "manWalk.gltf";
			genderRatio = 0;
		}
		else if (60.0f <= genderLevel) { // 割と男
			arrangement.status.name = "manWalk2.gltf";
			genderRatio = 1;
		}
		else {
			arrangement.status.name = "halfWalk.gltf"; // ハーフ
			genderRatio = 2;
		}
	}
}

void MapField::ResetArrangementAnimation(i32 humanPower) {
	arrangement.timer = 0.0f;
	arrangement.genderRatioSprite->SetRange(
		{ 0.0f,float(genderRatio) * Arrangement::GenderSpriteHeight },
		{ 400.0f, (r32)Arrangement::GenderSpriteHeight }
	);
	arrangement.humanRatioSprite->SetRange(
		{ 0.0f, float(humanPower) * Arrangement::HumanRatioHeight },
		{ 400.0f, (r32)Arrangement::HumanRatioHeight }
	);
}

void MapField::UpdateArrangementAnimation() {
	if (arrangement.timer >= arrangement.AnimationTime) {
		return;
	}

	arrangement.timer += FPSKeeper::DeltaTimeFrame();

	if (arrangement.timer < 0.5f) {
		r32 param = arrangement.timer / 0.5f;

		for (auto& row : cellsData_) {
			for (auto& cell : row) {
				if (cell->isRequired) {
					cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f * (1.0f - param) });
					cell->required->SetScale({ 1 - param, 1 - param });
				}
				if (cell->genderType != GenderType::None) {
					cell->block->SetColorAlpha((1.0f - param) * 0.6f);
				}
			}
		}
	}
	else {
		r32 param = (arrangement.timer - 0.5f) / (arrangement.AnimationTime - 0.5f);
		param = std::clamp(param, 0.0f, 1.0f);
		// ブロックが消えた際に色々する
		if (arrangement.timer - 0.5f < FPSKeeper::DeltaTimeFrame() && arrangement.timer >= 0.0f) {
			RandomizeTable();
			ResetMold();
			ResetBlocks();

			// 自軍発車
			if (friendlyManager_) {
				friendlyManager_->AddFriendly(arrangement.status);
			}
		}

		// 徐々に出現させる
		for (auto& row : cellsData_) {
			for (auto& cell : row) {
				if (cell->isRequired) {
					cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f * param });
					cell->required->SetScale({ param, param });
				}
			}
		}
	}

	{
		// 性別スプライトのアニメーション
		r32 InSeparateTime = 0.3f;
		r32 OutSeparateTime = 0.5f;
		if (arrangement.timer < InSeparateTime) {
			r32 param = arrangement.timer / InSeparateTime;
			param = std::clamp(param, 0.0f, 1.0f);

			arrangement.genderRatioSprite->SetColorAlpha(Easing::Out::Expo(param));
			r32 scaleBase = std::lerp(100.0f, 1.0f, Easing::Out::Expo(param));
			arrangement.genderRatioSprite->SetScale({ scaleBase, scaleBase });
		}
		else if (arrangement.timer >= OutSeparateTime) {
			r32 param = (arrangement.timer - OutSeparateTime) / (arrangement.AnimationTime - OutSeparateTime);
			param = std::clamp(param, 0.0f, 1.0f);

			// 性別スプライト
			arrangement.genderRatioSprite->SetColorAlpha(1 - param);
		}
	}

	{
		// 人間度スプライトのアニメーション
		r32 SeparateTime = 0.5f;
		if (arrangement.timer < SeparateTime) {
			r32 param = arrangement.timer / SeparateTime;
			param = std::clamp(param, 0.0f, 1.0f);

			arrangement.humanRatioSprite->SetColorAlpha(Easing::Out::Expo(param));
			r32 scaleBase = std::lerp(100.0f, 1.0f, Easing::Out::Expo(param));
			arrangement.humanRatioSprite->SetScale({ scaleBase, scaleBase });
		}
		else {
			arrangement.humanRatioSprite->SetScale({ 1, 1 });
			r32 param = (arrangement.timer - SeparateTime) / (arrangement.AnimationTime - SeparateTime);
			param = std::clamp(param, 0.0f, 1.0f);

			// 性別スプライト
			arrangement.humanRatioSprite->SetColorAlpha(1 - param);
		}
	}
}

void MapField::RandomizeTable() {
	// テーブルの選択
	tableIndex = Random::GetInt(0, (int)minoTables.size() - 1);
	useMinoIndex = std::nullopt;
	for (auto& tableMino : minoTables[tableIndex].minos) {
		tableMino->OnSelectedTable();
	}
}

void MapField::ResetMold() {
	canChangeMoldType = true;
	canComplete = false;
	// セルの選択
	useMoldIndex = moldManager.random_select();

	completeTex_->SetColor({ 0.6f,0.6f,0.6f,1.0f });

	WriteMold();
}

void MapField::UpdateMold() {
	if (hideMoldAnimationTimer >= 2.0f) {
		return;
	}

	hideMoldAnimationTimer += FPSKeeper::DeltaTime();
	hideMoldAnimationTimer = std::clamp(hideMoldAnimationTimer, 0.0f, 2.0f);

	if (hideMoldAnimationTimer < 0.3f) {
		r32 param = hideMoldAnimationTimer / 0.3f;
		for (auto& row : cellsData_) {
			for (auto& cell : row) {
				if (cell->isRequired) {
					cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f * (1.0f - param) });
					cell->required->SetScale({ 1 - param, 1 - param });
				}
			}
		}
	}
	else if (hideMoldAnimationTimer >= 1.7f) {
		r32 param = (hideMoldAnimationTimer - 1.7f) / (2.0f - 1.7f);
		param = std::clamp(param, 0.0f, 1.0f);
		for (auto& row : cellsData_) {
			for (auto& cell : row) {
				if (cell->isRequired) {
					cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f * param });
					cell->required->SetScale({ param, param });
				}
			}
		}
	}
	else if (hideMoldAnimationTimer - 0.3f < FPSKeeper::DeltaTime() && hideMoldAnimationTimer >= 0.0f) {
		WriteMold();
	}
}

void MapField::WriteMold() {
	const auto& mold = moldManager.get_mold(static_cast<MoldType>(moldType), useMoldIndex[moldType]);
	// 書き込み&再度初期化
	for (i32 rowI = 0; rowI < kMapHeight_; ++rowI) {
		for (i32 colI = 0; colI < kMapWidth_; ++colI) {
			auto& cell = cellsData_[rowI][colI];
			cell->genderType = GenderType::None;
			cell->isRequired = mold.body[rowI][colI];

			if (cell->isRequired) {
				cell->required->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f });
			}
			else {
				cell->required->SetColor({ 0.0f,0.0f,0.0f,0.0f });
			}
		}
	}

	// 穴の大きさを取得
	moldSize = mold.size;
}

void MapField::PoseMenu() {

	if (menuMoveTime_ > 0.0f) {
		menuMoveTime_ -= FPSKeeper::DeltaTime();

		float t = 1.0f - (menuMoveTime_ / 40.0f);
		float posX = 0.0f;
		if (isPoseMenu_) {
			posX = std::lerp(-450.0f, 400.0f, t);
		}
		else {
			posX = std::lerp(400.0f, -450.0f, t);
		}
		menuTex_->SetPos({ posX, 460.0f,0.0f });

		if (menuMoveTime_ <= 0.0f) {
			menuMoveTime_ = 0.0f;
			if (isPoseMenu_) {
				menuTex_->SetPos({ 400.0f, 460.0f,0.0f });
			}
			else {
				menuTex_->SetPos({ -450.0f, 460.0f,0.0f });
			}
		}
	}

	if (controlMino_) return;
	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	// PoseMenu
	{
		Vector3 pos = menuButtonTex_->GetPos();
		Vector2 size = menuButtonTex_->GetSize();
		float halfW = size.x * 0.5f;
		float halfH = size.y * 0.5f;
		if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
			mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
			if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
				if (!isPoseMenu_) {
					isPoseMenu_ = true;
					menuMoveTime_ = 40.0f;
					menuButtonTex_->SetRange({ 50.0f, 0.0f }, { 50.0f,50.0f });
				}
				else {
					isPoseMenu_ = false;
					menuMoveTime_ = 40.0f;
					menuButtonTex_->SetRange({ 0.0f, 0.0f }, { 50.0f,50.0f });
				}
				ClickEmit_.pos_ = { -8.1f,9.09f,0.0f };
				ClickEmit_.Emit();
				AudioPlayer::GetInstance()->SoundPlayWave(*push);
			}
			menuButtonTex_->SetColor({ 0.5f,0.5f,0.5f,1.0f });
		}
		else {
			menuButtonTex_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
	}
}

std::pair<i32, i32> MapField::CalcCellIndex(const Vector3& position) const {
	i32 xIndex = static_cast<i32>((position.x - cellsPos_.x) / cellsSize_);
	i32 yIndex = static_cast<i32>((position.y - cellsPos_.y) / cellsSize_);
	return { yIndex, xIndex };
}

void MapField::RemoveControlMino() {
	auto& blocks = controlMino_->GetBlocks();

	Vector4 color = controlMino_->GetGender() == GenderType::Man ?
		Vector4(0.0f, 0.0f, 1.0f, 0.6f) :
		Vector4(1.0f, 0.08f, 0.58f, 0.6f);

	// 設置したやつをマップに反映
	for (auto& block : blocks) {
		auto [rowI, colI] = CalcCellIndex(block->sprite->GetPos());

		auto& cell = cellsData_[rowI][colI];

		cell->genderType = controlMino_->GetGender();
		cell->block->SetColor(color);

		if (cell->isRequired) {
			canComplete = true;
		}
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
	cellsPos_ = { 158.0f,280.0f };

	Vector2 cellSize = { cellsSize_,cellsSize_ };
	cellsData_.resize(15);
	for (i32 i = 0; std::vector<std::unique_ptr<CellData>>& row : cellsData_) {
		row.resize(15);

		for (i32 j = 0; std::unique_ptr<CellData>& cell : row) {
			cell = std::make_unique<CellData>();
			cell->genderType = GenderType::None;
			cell->isRequired = false;

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

		// ボタン位置の設定
		if (table.minos.size() == 6) {
			table.minos[0]->SetupButtonPosition({ 105,119,0 });
			table.minos[1]->SetupButtonPosition({ 193,92,0 });
			table.minos[2]->SetupButtonPosition({ 193,148,0 });
			table.minos[3]->SetupButtonPosition({ 470,119,0 });
			table.minos[4]->SetupButtonPosition({ 372,92,0 });
			table.minos[5]->SetupButtonPosition({ 372,148,0 });
		}
	}
}
