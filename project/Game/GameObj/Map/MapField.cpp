#include "MapField.h"

MapField::MapField() {
}

MapField::~MapField() {
}

void MapField::Initialize() {

	map_ = std::vector(20, std::vector<int>(10));

}

void MapField::Update() {



}

void MapField::DebugGUI() {
#ifdef _DEBUG

#endif // _DEBUG
}

void MapField::AddMino(BlockType type) {
	if (controlMino_) return;
	std::unique_ptr<Mino> mino;
	mino = std::make_unique<Mino>();
	mino->Initialize();
	mino->InitBlock(type);
	controlMino_ = std::move(mino);
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

	//minos_.push_back(std::move(mino));
}

void MapField::UpdateControlMino() {
	if (!controlMino_) return;

	downTime_ -= FPSKeeper::DeltaTime();
	if (downTime_ <= 0.0f) {
		downTime_ = 60.0f;
		CellCheck();
	}

	controlMino_->Update();
}

void MapField::CellCheck() {
	switch (controlMino_->GetBlockType()) {
	case BlockType::L:

		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x)] == 1) {
		
			return;
		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x + 1.0f)] == 1) {

			return;
		}

		break;
	case BlockType::T:

		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x)] == 1) {

		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x + 1.0f)] == 1) {

		}
		if (map_[int(cellNum.y + 1.0f)][int(cellNum.x - 1.0f)] == 1) {

		}

		break;
	default:
		break;
	}

	cellNum.y++;
}
