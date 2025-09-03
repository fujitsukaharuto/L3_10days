#include "Climber.h"

#include "Game/GameObj/Map/MapField.h"

Climber::Climber(MapField* mapField) {
	mapField_ = mapField;
}

void Climber::Initialize() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");

	// 初期位置を設定
	model_->transform.translate = kStartPos_;
}

void Climber::Update() {
}

void Climber::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	OriginGameObject::Draw(mate, is);
}

void Climber::DebugGUI() {
#ifdef _DEBUG
#endif // _DEBUG
}

bool Climber::CanAvoidBlock() {
	auto mapRow = CalcAvoidBlocks();

	int row = static_cast<int>(model_->GetWorldPos().y / 2);

	const auto& featureMino = mapField_->GetFeatureMino();
	const auto& featureMinoBlocks = featureMino->GetBlocks();

	for (const auto& block : featureMinoBlocks) {
		int blockRow = static_cast<int>(block->GetWorldPos().y / 2);
		int blockColumn = static_cast<int>(block->GetWorldPos().x / 2);
		// 同じ高さなら無視
		if (blockRow == row) {
			continue;
		}
		// 1マス上なら地面判定を消す
		else if (blockRow == row + 1) {
			mapRow[blockColumn] = 0;
		}
	}

	return std::find(mapRow.begin(), mapRow.end(), 1) != mapRow.end();
}

void Climber::AvoidBlock() {
}

std::vector<int> Climber::CalcAvoidBlocks() {
	const size_t row = static_cast<size_t>(model_->GetWorldPos().y / 2);
	std::vector<int> mapRow(1, (int32_t)mapField_->GetMapRow(0).size());
	// 地面にいる場合は計算しない
	if (row != 0) {
		mapRow = mapField_->GetMapRow(row - 1);
	}

	// 壁になるミノ
	const std::vector<int> wallBlockRow = mapField_->GetMapRow(row);

	// 行動できない範囲の地面を0にする
	{	// 左
		int32_t left = static_cast<int32_t>(model_->GetWorldPos().x / 2);
		bool isMoveable = false;
		for (; left >= 0; --left) {
			if (!isMoveable) {
				mapRow[left] = 0;
			}
			else {
				if (mapRow[left] == 0) {
					isMoveable = true;
					mapRow[left] = 0;
				}
			}
		}
	}
	{	// 右
		int32_t right = static_cast<int32_t>(model_->GetWorldPos().x / 2);
		bool isMoveable = false;
		for (; right < static_cast<int32_t>(mapRow.size()); ++right) {
			if (!isMoveable) {
				mapRow[right] = 0;
			}
			else {
				if (mapRow[right] == 0) {
					isMoveable = true;
					mapRow[right] = 0;
				}
			}
		}
	}

	// 壁以降の範囲を地面なしにする
	{	// 左
		int32_t left = static_cast<int32_t>(model_->GetWorldPos().x / 2);
		bool isWall = false;
		for (; left >= 0; --left) {
			if (!isWall) {
				if (wallBlockRow[left] == 1) {
					isWall = true;
					mapRow[left] = 0;
				}
			}
			else {
				mapRow[left] = 0;
			}
		}
	}
	{	// 右
		int32_t right = static_cast<int32_t>(model_->GetWorldPos().x / 2);
		bool isWall = false;
		for (; right < static_cast<int32_t>(wallBlockRow.size()); ++right) {
			if (!isWall) {
				if (wallBlockRow[right] == 1) {
					isWall = true;
					mapRow[right] = 0;
				}
			}
			else {
				mapRow[right] = 0;
			}
		}
	}

	return mapRow;
}
