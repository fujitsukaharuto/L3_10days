#include "GenderMapField.h"

GenderMapField::GenderMapField() {
	// 初期化
	for (size_t i = 0; i < kMapHeight_; i++) {
		for (size_t j = 0; j < kMapWidth_; j++) {
			map_[j][i] = GenderType::NONE;
		}
	}

	// ブロックに適用
	ApplyMapDataToBlock();
}

void GenderMapField::Update() {

}

void GenderMapField::Draw() {

}

void GenderMapField::LoadMapFromCSV(const std::string& fileName) {

}

void GenderMapField::ApplyMapDataToBlock() {
	
}

const std::vector<std::vector<GenderType>>& GenderMapField::GetMap() {
	return map_;
}
