#include "MapField.h"

MapField::MapField() {}

MapField::~MapField() {}

void MapField::Initialize() {

	map_ = std::vector(20, std::vector<int>(10));

	panelTex_ = std::make_unique<Sprite>();
	panelTex_->Load("white2x2.png");
	panelTex_->SetSize(panelSize_);
	panelTex_->SetPos({ 640.0f,-200.0f,0.0f });
	for (int i = 0; i < 6; i++) {
		std::unique_ptr<Sprite> button;
		button = std::make_unique<Sprite>();
		button->Load("white2x2.png");
		if (i == 0) button->SetColor({ 1.0f,0.65f,0.0f,1.0f });
		if (i == 1) button->SetColor({ 0.55f,0.0f,0.55f,1.0f });
		if (i == 2) button->SetColor({ 0.0f,0.5f,0.0f,1.0f });
		if (i == 3) button->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		if (i == 4) button->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		if (i == 5) button->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		buttonTex_.push_back(std::move(button));
	}
	panelTexturePosY_ = 70.0f;
	selectPanelTime_ = defaultSelectPanelTime_;

	selectorTex_ = std::make_unique<Sprite>();
	selectorTex_->Load("SquareFrame.png");
	selectorTex_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	for (int i = 0; i < 6; i++) {
		BlockType type = BlockType::L;
		if (i == 1) type = BlockType::T;
		if (i == 2) type = BlockType::S;
		if (i == 3) type = BlockType::Z;
		if (i == 4) type = BlockType::O;
		if (i == 5) type = BlockType::J;
		selectTypes_.push_back(type);
	}
}

void MapField::Update() {
	UpdateControlMino();
	UpdateSelectPanel();

	for (auto& mino : minos_) {
		mino->Update();
	}

}

void MapField::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	if (controlMino_) {
		controlMino_->Draw();
		if (futureMino_) {
			futureMino_->DrawLine();
		}
	}
	for (auto& mino : minos_) {
		mino->Draw();
	}

	panelTex_->Draw();
	selectorTex_->Draw();
	for (int i = 0; i < 6; i++) {
		buttonTex_[i]->Draw();
	}
}

void MapField::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("MapField")) {

		if (ImGui::Button("AddMino:L")) {
			if (!controlMino_) {
				AddMino(BlockType::L);
			}
		}ImGui::SameLine();
		if (ImGui::Button("AddMino:T")) {
			if (!controlMino_) {
				AddMino(BlockType::T);
			}
		}
		if (ImGui::Button("AddMino:S")) {
			if (!controlMino_) {
				AddMino(BlockType::S);
			}
		}ImGui::SameLine();
		if (ImGui::Button("AddMino:Z")) {
			if (!controlMino_) {
				AddMino(BlockType::Z);
			}
		}
		if (ImGui::Button("AddMino:O")) {
			if (!controlMino_) {
				AddMino(BlockType::O);
			}
		}ImGui::SameLine();
		if (ImGui::Button("AddMino:J")) {
			if (!controlMino_) {
				AddMino(BlockType::J);
			}
		}
		if (ImGui::Button("QuickDrop")) {
			QuickDrop();
		}
	}
#endif // _DEBUG
}

void MapField::UpdateSelectPanel() {
	if (!controlMino_) {
		if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
			minoButtonNum_--;
			if (minoButtonNum_ < 0) {
				minoButtonNum_ = int(selectTypes_.size()) - 1;
			}
		} else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
			minoButtonNum_++;
			if (minoButtonNum_ == int(selectTypes_.size())) {
				minoButtonNum_ = 0;
			}
		} else if (Input::GetInstance()->TriggerKey(DIK_UP)) {
			if (!controlMino_) {
				AddMino(selectTypes_[minoButtonNum_]);
				return;
			}
		}
	}
	SelectMino();
	ReturenSelectMino();
}

void MapField::SelectMino() {
	if (!controlMino_) {
		if (selectPanelTime_ > 0.0f) selectPanelTime_ -= FPSKeeper::DeltaTime();
		if (selectPanelTime_ < 0.0f) selectPanelTime_ = 0.0f;
		if (selectPanelTime_ != 0.0f) {
			float t = 1.0f - (selectPanelTime_ / defaultSelectPanelTime_);
			float posY = std::lerp(-200.0f, panelTexturePosY_, t);
			panelTex_->SetPos({ 640.0f,posY,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 640.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
			
			for (int i = 0; i < selectTypes_.size(); i++) {
				Vector2 sSize = { (panelSize_.x / float(selectTypes_.size())) * 0.8f ,panelSize_.y * 0.8f };
				buttonTex_[int(selectTypes_[i])]->SetSize(sSize);
				selectorMaxSize_ = { sSize.x + 30.0f,sSize.y + 30.0f };
				selectorMinSize_ = { sSize.x + 10.0f,sSize.y + 10.0f };

				float x = startX + i * space;
				buttonTex_[int(selectTypes_[i])]->SetPos({ x, posY, 0.0f });
			}

			float x = startX + float(minoButtonNum_) * space;
			selectorTex_->SetPos({ x, posY, 0.0f });

		} else {
			panelTex_->SetPos({ 640.0f,panelTexturePosY_,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 640.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
			for (int i = 0; i < selectTypes_.size(); i++) {
				Vector2 sSize = { (panelSize_.x / float(selectTypes_.size())) * 0.8f ,panelSize_.y * 0.8f };
				buttonTex_[int(selectTypes_[i])]->SetSize(sSize);
				selectorMaxSize_ = { sSize.x + 30.0f,sSize.y + 30.0f };
				selectorMinSize_ = { sSize.x + 10.0f,sSize.y + 10.0f };

				float x = startX + i * space;
				buttonTex_[int(selectTypes_[i])]->SetPos({ x, panelTexturePosY_, 0.0f });
			}

			float x = startX + float(minoButtonNum_) * space;
			selectorTex_->SetPos({ x, panelTexturePosY_, 0.0f });
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
}

void MapField::ReturenSelectMino() {
	if (controlMino_) {
		if (selectPanelTime_ > 0.0f) selectPanelTime_ -= FPSKeeper::DeltaTime();
		if (selectPanelTime_ < 0.0f) selectPanelTime_ = 0.0f;
		if (selectPanelTime_ != 0.0f) {
			float t = 1.0f - (selectPanelTime_ / defaultSelectPanelTime_);
			float posY = std::lerp(panelTexturePosY_, -200.0f, t);
			panelTex_->SetPos({ 640.0f,posY,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 640.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
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
			panelTex_->SetPos({ 640.0f,-200.0f,0.0f });

			float space = 570.0f / float(selectTypes_.size());
			float startX = 640.0f - (space * (float(selectTypes_.size()) - 1.0f)) / 2.0f;
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
	}
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
	default:
		break;
	}
	if (map_[int(cellNum_.y)][int(cellNum_.x)] == 1) return;
	std::unique_ptr<Mino> mino;
	mino = std::make_unique<Mino>();
	mino->Initialize();
	mino->SetCollisionMana(cMana_);
	mino->InitBlock(type);

	controlMino_ = std::move(mino);
	controlMino_->GetModel()->transform.translate = { (cellNum_.x) * 2.0f,(20.0f - cellNum_.y) * 2.0f,0.0f };

	futureMino_ = std::make_unique<Mino>();
	futureMino_->Initialize();
	futureMino_->InitBlock(type);
	futureMino_->GetModel()->transform.translate = { (cellNum_.x) * 2.0f,(20.0f - cellNum_.y) * 2.0f,0.0f };
	FutureMinoUpdate();

	selectPanelTime_ = defaultSelectPanelTime_;

	//minos_.push_back(std::move(mino));
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
	if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		QuickDrop();
		return;
	}
	if (futureMino_) {
		futureMino_->Update();
	}
	RemoveControlMino();

}

void MapField::MoveControlMino() {
	if (!controlMino_) return;

	Vector2 nextCell = cellNum_;
	bool isMove = false;
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		nextCell.x--;
		isMove = true;
	} else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		nextCell.x++;
		isMove = true;
	}

	if (!isMove) return;

	switch (controlMino_->GetBlockType()) {
	case BlockType::L:
		if (int(nextCell.x) == -1 || int(nextCell.x) == 9) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y - 2.0f)][int(nextCell.x)] == 1 || map_[int(nextCell.y)][int(nextCell.x + 1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	case BlockType::T:
		if (int(nextCell.x) == 0 || int(nextCell.x) == 9) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y)][int(nextCell.x - 1.0f)] == 1 || map_[int(nextCell.y)][int(nextCell.x + 1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	case BlockType::S:
		if (int(nextCell.x) == 0 || int(nextCell.x) == 9) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)] == 1 || map_[int(nextCell.y)][int(nextCell.x + -1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	case BlockType::Z:
		if (int(nextCell.x) == 0 || int(nextCell.x) == 9) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y - .0f)][int(nextCell.x - 1.0f)] == 1 || map_[int(nextCell.y)][int(nextCell.x + 1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	case BlockType::O:
		if (int(nextCell.x) == -1 || int(nextCell.x) == 9) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y - 1.0f)][int(nextCell.x + 1.0f)] == 1 || map_[int(nextCell.y)][int(nextCell.x + 1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	case BlockType::J:
		if (int(nextCell.x) == 0 || int(nextCell.x) == 10) {
			isMove = false;
			break;
		}
		if (map_[int(nextCell.y)][int(nextCell.x)] == 1 || map_[int(nextCell.y - 1.0f)][int(nextCell.x)] == 1
			|| map_[int(nextCell.y - 2.0f)][int(nextCell.x)] == 1 || map_[int(nextCell.y)][int(nextCell.x - 1.0f)] == 1) {
			isMove = false;
			break;
		}
		break;
	default:
		break;

	}

	if (!isMove) return;
	cellNum_ = nextCell;
	controlMino_->GetModel()->transform.translate = { (cellNum_.x) * 2.0f,(20.0f - cellNum_.y) * 2.0f,0.0f };
	FutureMinoUpdate();
}

void MapField::CellCheck() {
	switch (controlMino_->GetBlockType()) {
	case BlockType::L:


		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::T:

		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::S:

		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::Z:

		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::O:

		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::J:

		if (int(cellNum_.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum_.y + 1.0f)][int(cellNum_.x - 1.0f)] == 1) {
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
	if (controlMino_) {
		while (controlMino_->GetBlockMode() == BlockMode::Fall) {
			CellCheck();
		}
		controlMino_->GetModel()->transform.translate = { (cellNum_.x) * 2.0f,(20.0f - cellNum_.y) * 2.0f,0.0f };
		controlMino_->Update();
		RemoveControlMino();
	}
}

void MapField::SetColliderManager(CollisionManager* cMana) {
	cMana_ = cMana;
}

void MapField::RemoveControlMino() {
	if (controlMino_->GetBlockMode() == BlockMode::Stay) {
		switch (controlMino_->GetBlockType()) {
		case BlockType::L:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
			break;
		case BlockType::T:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
			break;
		case BlockType::S:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
			break;
		case BlockType::O:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x + 1.0f)] = 1;
			break;
		case BlockType::J:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 2.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x - 1.0f)] = 1;
			break;
		case BlockType::Z:
			map_[int(cellNum_.y)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x)] = 1;
			map_[int(cellNum_.y - 1.0f)][int(cellNum_.x - 1.0f)] = 1;
			map_[int(cellNum_.y)][int(cellNum_.x + 1.0f)] = 1;
			break;
		default:
			break;

		}
		minos_.push_back(std::move(controlMino_));
		controlMino_ = nullptr;
		selectPanelTime_ = defaultSelectPanelTime_;
	}
}

void MapField::FutureMinoUpdate() {
	if (!futureMino_) return;
	Vector2 cell = cellNum_;
	futureMino_->SetBlockMode(BlockMode::Fall);
	while (futureMino_->GetBlockMode() == BlockMode::Fall) {
		switch (futureMino_->GetBlockType()) {
		case BlockType::L:

			if (int(cell.y + 1.0f) == 20) {
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

			if (int(cell.y + 1.0f) == 20) {
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

			if (int(cell.y + 1.0f) == 20) {
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

			if (int(cell.y + 1.0f) == 20) {
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

			if (int(cell.y + 1.0f) == 20) {
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

			if (int(cell.y + 1.0f) == 20) {
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
		default:
			break;
		}

		if (futureMino_->GetBlockMode() != BlockMode::Stay) {
			cell.y++;
		}
	}
	futureMino_->GetModel()->transform.translate = { (cell.x) * 2.0f,(20.0f - cell.y) * 2.0f,0.0f };
	futureMino_->Update();
}
