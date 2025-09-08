#include "MapField.h"

#include "Game/Collider/CollisionManager.h"
#include "Engine//Math/Random/Random.h"

#undef min
#undef max

MapField::MapField() {}

MapField::~MapField() {}

void MapField::Initialize() {
	panelTex_ = std::make_unique<Sprite>();
	panelTex_->Load("blockBox.png");
	panelTex_->SetPos({ 285.0f,102.5f,0.0f });
	panelTexturePosY_ = 102.5f;

	manPanelTex_ = std::make_unique<Sprite>();
	manPanelTex_->Load("choice.png");
	manPanelTex_->SetPos({ 285.0f, 35.0f,0.0f });
	frameTex_ = std::make_unique<Sprite>();
	frameTex_->Load("Frame.png");
	frameTex_->SetPos({ 285.0f, 400.0f,0.0f });
	completeTex_ = std::make_unique<Sprite>();
	completeTex_->Load("completed.png");
	completeTex_->SetPos({ 285.0f, 660.0f,0.0f });

	selectPanelTime_ = defaultSelectPanelTime_;

	selectorTex_ = std::make_unique<Sprite>();
	selectorTex_->Load("SquareFrame.png");
	selectorTex_->SetColor({ 1.0f,0.0f,0.0f,0.7f });

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

	mapSizeTex_ = std::make_unique<Sprite>();
	mapSizeTex_->Load("frameSize.png");
	mapSizeTex_->SetPos({ 285.0f, 170.0f,0.0f });
	mapSizeTex_->SetRange({ mapSizeNum_ * 40.0f,0.0f }, { 40.0f,50.0f });

	InitCells();
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
	manPanelTex_->Draw();
	frameTex_->Draw();
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
	//for (int i = 0; i < buttonTex_.size(); i++) {
	//	if (blockButtonNum_ == i) {
	//		buttonTex_[i]->Draw();
	//	}
	//}

	CellBackgroundDraw();
	CellRequiredSpriteDraw();
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
		Vector3 pos = manPanelTex_->GetPos();
		ImGui::DragFloat2("ManPanelPos", &pos.x);
		manPanelTex_->SetPos(pos);
		Vector2 size = manPanelTex_->GetSize();
		ImGui::DragFloat2("ManPanelSize", &size.x);
		manPanelTex_->SetSize(size);
		pos = frameTex_->GetPos();
		ImGui::DragFloat2("WomanPanelPos", &pos.x);
		frameTex_->SetPos(pos);
		size = frameTex_->GetSize();
		ImGui::DragFloat2("WomanPanelSize", &size.x);
		frameTex_->SetSize(size);
	}
#endif // _DEBUG
}

void MapField::BackDraw() {
	BackPanelTex_->Draw();
}

void MapField::FactoryDraw() {
	factoryBackPanelTex_->Draw();
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
			cell.background->Draw();
		}
	}
}

void MapField::CellRequiredSpriteDraw() {
	for (auto& rows : cellsData_) {
		for (auto& cell : rows) {
			if (cell.isRequired) {
				cell.required->Draw();
			}
		}
	}
}

void MapField::UpdateSelectPanel() {
	if (controlMino_) {
		return;
	}
	Vector2 mouse = Input::GetInstance()->GetMousePosition();

	// blockどれつかむか
	Vector3 pos = buttonTex_[blockButtonNum_]->GetPos();   // 中心座標
	Vector2 size = buttonTex_[blockButtonNum_]->GetSize(); // 幅・高さ
	float halfW = size.x * 0.5f;
	float halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		minoButtonNum_ = 0;
		if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
			if (!controlMino_) {
				AddMino(selectTypes_[blockButtonNum_]);
				return;
			}
		}
	}

	// 性別決める
	pos = manPanelTex_->GetPos();
	size = manPanelTex_->GetSize();
	halfW = size.x * 0.5f;
	halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
			if (!controlMino_) {
				gender_ = int(GenderType::Man);
				GenderColor();
			}
		}
	}
	if (mouse.x >= pos.x && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0) && !haveControlMino_) {
			if (!controlMino_) {
				gender_ = int(GenderType::Woman);
				GenderColor();
			}
		}
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
			}
		}
	}
	SelectMino();
}

void MapField::SelectMino() {
	if (selectPanelTime_ > 0.0f) selectPanelTime_ -= FPSKeeper::DeltaTime();
	if (selectPanelTime_ < 0.0f) selectPanelTime_ = 0.0f;

	if (minoButtonNum_ == 0) {
		selectorTex_->SetPos({ 185.0f,95.0f, 0.0f });
		selectorMaxSize_ = { 150.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 150.0f + 10.0f,40.0f + 10.0f };
	}
	else if (minoButtonNum_ == 1) {
		selectorTex_->SetPos({ 335.0f,95.0f, 0.0f });
		selectorMaxSize_ = { 50.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 50.0f + 10.0f,40.0f + 10.0f };
	}
	else if (minoButtonNum_ == 2) {
		selectorTex_->SetPos({ 435.0f,95.0f, 0.0f });
		selectorMaxSize_ = { 50.0f + 30.0f,40.0f + 30.0f };
		selectorMinSize_ = { 50.0f + 10.0f,40.0f + 10.0f };
	}
	selectorSizeTime_ += FPSKeeper::DeltaTime() * 0.25f;
	selectorSizeTime_ = fmodf(selectorSizeTime_, std::numbers::pi_v<float>);
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

void MapField::AddMino() {
	std::unique_ptr<Mino> mino;
	mino = std::make_unique<Mino>();
	mino->Initialize();
	mino->InitBlock(GenderType(gender_));

	controlMino_ = std::move(mino);
	controlMino_->GetTransform().translate = { cellsPos_.x + (cellNum_.x) * 2.0f,cellsPos_.y + (15.0f - cellNum_.y) * 2.0f, 0.0f };
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
	RemoveControlMino();

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
	i32 minRow = 100;
	i32 minColumn = 100;
	i32 maxRow = -1;
	i32 maxColumn = -1;
	for (auto& block : controlMino_->GetBlocks()) {
		auto [row, column] = CalcCellIndex(block->GetWorldPos());
		minRow = std::min(row, minRow);
		minColumn = std::min(column, minColumn);
		maxRow = std::max(row, maxRow);
		maxColumn = std::max(column, maxColumn);
	}

	// チェック
	for (auto& block : controlMino_->GetBlocks()) {
		auto [row, column] = CalcCellIndex(block->GetWorldPos());
		auto& cell = cellsData_[row][column];
		if (cell.isFilled) {
			cell.required->SetColor({ 1.0f,0.0f,0.0f,0.6f });
		}
	}

	cellNum_ = nextCell;

	controlMino_->GetTransform().translate = { cellsPos_.x + (cellNum_.x) * 2.0f,cellsPos_.y + (15.0f - cellNum_.y) * 2.0f + oldDistance,0.0f };
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

	for (auto& row : cellsData_) {
		for (auto& cell : row) {
			switch (cell.genderType) {
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

	maxB_.push_back(maxBlocks);
	manB_.push_back(manBlocks);
	womanB_.push_back(womanBlocks);

	for (auto& row : cellsData_) {
		for (auto& cell : row) {
			cell.genderType = GenderType::None;
		}
	}
}

std::pair<i32, i32> MapField::CalcCellIndex(const Vector3& position) const {
	i32 xIndex = static_cast<i32>((position.x - cellsPos_.x) / cellsSize_);
	i32 yIndex = static_cast<i32>((position.y - cellsPos_.y) / cellsSize_);
	return { xIndex, yIndex };
}

void MapField::RemoveControlMino() {
	auto& blocks = controlMino_->GetBlocks();

	Vector4 color = controlMino_->GetGender() == GenderType::Man ? Vector4(0.0f, 0.5f, 1.0f, 1.0f) : Vector4(1.0f, 0.5f, 0.8f, 1.0f);

	for (auto& block : blocks) {
		Vector3 position = block->GetWorldPos();

		auto [xIndex, yIndex] = CalcCellIndex(position);
		if (xIndex >= 0 && xIndex < kMapWidth_ && yIndex >= 0 && yIndex < kMapHeight_) {
			cellsData_[yIndex][xIndex].genderType = controlMino_->GetGender();
			cellsData_[yIndex][xIndex].block->SetColor(color);
		}
	}

	controlMino_ = nullptr;
}

void MapField::CellSpriteSetColor() {
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

	Vector2 cellSize = { cellsSize_ - 6.0f,cellsSize_ - 6.0f };
	cellsData_.resize(15, std::vector<CellData>(15));
	for (i32 i = 0; std::vector<CellData>& row : cellsData_) {
		for (i32 j = 0; CellData& cell : row) {
			cell.genderType = GenderType::None;
			cell.isRequired = requiresMap[i][j];

			Vector3 cellPosition = { cellsPos_.x + (j * cellsSize_),cellsPos_.y + ((i)*cellsSize_), 0 };

			cell.block = std::make_unique<Sprite>();
			cell.block->Load("white2x2.png");
			cell.block->SetSize(cellSize);
			cell.block->SetPos(cellPosition);
			cell.block->SetColor({ 0.2f, 0.2f, 0.2f, 0.4f });

			cell.background = std::make_unique<Sprite>();
			cell.background->Load("white2x2.png");
			cell.background->SetPos(cellPosition);
			if (cell.isRequired) {
				cell.background->SetColor({ 0.5f, 0.5f, 0.5f, 0.1f });
				cell.background->SetSize({ cellsSize_,cellsSize_ });
			}
			else {
				cell.background->SetColor({ 0.1f, 0.1f, 0.1f, 0.5f });
			}

			++j;
		}
		++i;
	}
}

void MapField::GenderColor() {
	for (i32 i = 0; std::vector<CellData>& row : cellsData_) {
		for (i32 j = 0; CellData& cell : row) {
			switch (cell.genderType) {
			default:
				break;
			}
			if (map_[i][j] >= 1) return;
			if (gender_ == int(GenderType::Man)) {
				arrangementCells_[i][j]->SetColor({ 0.0f,0.0f,1.0f,0.6f });
			}
			else if (gender_ == int(GenderType::Woman)) {
				arrangementCells_[i][j]->SetColor({ 1.0f, 0.08f, 0.58f, 0.6f });
			}
		}
	}
}
