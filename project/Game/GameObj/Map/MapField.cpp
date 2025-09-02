#include "MapField.h"

MapField::MapField() {
}

MapField::~MapField() {
}

void MapField::Initialize() {

	map_ = std::vector(20, std::vector<int>(10));

}

void MapField::Update() {
	UpdateControlMino();
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
}

void MapField::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("MapField")) {

		if (ImGui::Button("AddMino:L")) {
			if (!controlMino_) {
				AddMino(BlockType::L);
			}
		}
		if (ImGui::Button("AddMino:T")) {
			if (!controlMino_) {
				AddMino(BlockType::T);
			}
		}
		if (ImGui::Button("QuickDrop")) {
			QuickDrop();
		}
	}
#endif // _DEBUG
}

void MapField::AddMino(BlockType type) {
	if (controlMino_) return;
	switch (type) {
	case BlockType::L:
		cellNum = { 4.0f, 2.0f };
		break;
	case BlockType::T:
		cellNum = { 4.0f, 1.0f };
		break;
	default:
		break;
	}
	if (map_[int(cellNum.y)][int(cellNum.x)] == 1) return;
	std::unique_ptr<Mino> mino;
	mino = std::make_unique<Mino>();
	mino->Initialize();
	mino->InitBlock(type);
	controlMino_ = std::move(mino);
	controlMino_->GetModel()->transform.translate = { (cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };

	futureMino_ = std::make_unique<Mino>();
	futureMino_->Initialize();
	futureMino_->InitBlock(type);
	futureMino_->GetModel()->transform.translate = { (cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
	FutureMinoUpdate();

	//minos_.push_back(std::move(mino));
}

void MapField::UpdateControlMino() {
	if (!controlMino_) return;

	downTime_ -= FPSKeeper::DeltaTime();
	if (downTime_ <= 0.0f) {
		downTime_ = 60.0f;
		CellCheck();
		controlMino_->GetModel()->transform.translate = { (cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
	}
	MoveControlMino();

	controlMino_->Update();
	if (futureMino_) {
		futureMino_->Update();
	}
	RemoveControlMino();
}

void MapField::MoveControlMino() {
	if (!controlMino_) return;

	Vector2 nextCell = cellNum;
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
		}

		break;
	case BlockType::T:

		if (int(nextCell.x) == 0 || int(nextCell.x) == 9) {
			isMove = false;
		}

		break;
	default:
		break;
	}

	if (!isMove) return;
	cellNum = nextCell;
	controlMino_->GetModel()->transform.translate = { (cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
	FutureMinoUpdate();
}

void MapField::CellCheck() {
	switch (controlMino_->GetBlockType()) {
	case BlockType::L:

		if (int(cellNum.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	case BlockType::T:

		if (int(cellNum.y + 1.0f) == 20) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x + 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x - 1.0f)] == 1) {
			controlMino_->SetBlockMode(BlockMode::Stay);
			return;
		}

		break;
	default:
		break;
	}

	cellNum.y++;
}

void MapField::QuickDrop() {
	if (controlMino_) {
		while (controlMino_->GetBlockMode() == BlockMode::Fall) {
			CellCheck();
		}
		controlMino_->GetModel()->transform.translate = { (cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
		controlMino_->Update();
		RemoveControlMino();
	}
}

void MapField::RemoveControlMino() {
	if (controlMino_->GetBlockMode() == BlockMode::Stay) {
		switch (controlMino_->GetBlockType()) {
		case BlockType::L:
			map_[int(cellNum.y)][int(cellNum.x)] = 1;
			map_[int(cellNum.y - 1.0f)][int(cellNum.x)] = 1;
			map_[int(cellNum.y - 2.0f)][int(cellNum.x)] = 1;
			map_[int(cellNum.y)][int(cellNum.x + 1.0f)] = 1;
			break;
		case BlockType::T:
			map_[int(cellNum.y)][int(cellNum.x)] = 1;
			map_[int(cellNum.y - 1.0f)][int(cellNum.x)] = 1;
			map_[int(cellNum.y)][int(cellNum.x - 1.0f)] = 1;
			map_[int(cellNum.y)][int(cellNum.x + 1.0f)] = 1;
			break;
		default:
			break;
		}
		minos_.push_back(std::move(controlMino_));
		controlMino_ = nullptr;
	}
}

void MapField::FutureMinoUpdate() {
	if (!futureMino_) return;
	Vector2 cell = cellNum;
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
