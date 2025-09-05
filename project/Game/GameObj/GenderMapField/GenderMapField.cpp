#include "GenderMapField.h"

GenderMapField::GenderMapField() {
	// 初期化
	map_.resize(kMapHeight_);
	blocks_.resize(kMapHeight_);
	for (size_t i = 0; i < kMapHeight_; i++) {
		map_[i].resize(kMapWidth_);
		blocks_[i].resize(kMapWidth_);
		for (size_t j = 0; j < kMapWidth_; j++) {
			map_[i][j] = GenderType::NONE;
			blocks_[i][j] = std::make_unique<GenderBlock>(); 
		}
	}

	// ブロックに適用
	ApplyMapDataToBlock();
}

void GenderMapField::Update() {
	for (auto& blockH : blocks_) {
		for (auto& blockW : blockH) {
			blockW->Update();
		}
	}
}

void GenderMapField::Draw() {
	for (auto& blockH : blocks_) {
		for (auto& blockW : blockH) {
			blockW->Draw();
		}
	}
}

void GenderMapField::LoadMapFromCSV(const std::string& fileName) {
	fileName;
}

void GenderMapField::ApplyMapDataToBlock() {
	for (size_t i = 0; i < kMapHeight_; i++) {
		for (size_t j = 0; j < kMapWidth_; j++) {
			map_[i][j] = GenderType::NONE;
		}
	}
}

const std::vector<std::vector<GenderType>>& GenderMapField::GetMap() {
	return map_;
}
