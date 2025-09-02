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
	controlMino_->GetModel()->transform.translate = { (10.0f - cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };

	//minos_.push_back(std::move(mino));
}

void MapField::UpdateControlMino() {
	if (!controlMino_) return;

	downTime_ -= FPSKeeper::DeltaTime();
	if (downTime_ <= 0.0f) {
		downTime_ = 60.0f;
		CellCheck();
		controlMino_->GetModel()->transform.translate = { (10.0f - cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
	}

	controlMino_->Update();
	RemoveControlMino();
}

void MapField::CellCheck() {
	switch (controlMino_->GetBlockType()) {
	case BlockType::L:

		if (int(cellNum.y + 1.0f) == 20){
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
		controlMino_->GetModel()->transform.translate = { (10.0f - cellNum.x) * 2.0f,(20.0f - cellNum.y) * 2.0f,0.0f };
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
