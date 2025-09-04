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
	auto mapRow = CalcGroundBlocks();

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

void Climber::AvoidFeatureBlock() {
	// 落下前に地面に埋まらないよう移動
	auto mapRow = CalcGroundBlocks();

	int row = static_cast<int>((model_->transform.translate.y - mapField_->GetOldDistance()) / 2);

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

	// 地面がある位置に移動
	auto  itr = std::find(mapRow.begin(), mapRow.end(), 1);

	// 位置を移動
	if (itr != mapRow.end()) {
		int index = static_cast<int>(std::distance(mapRow.begin(), itr));
		model_->transform.translate.x = index * 2.0f;
	}
}

void Climber::OnDropped() {
	// 到達可能な最大高に移動
	int row = (int)mapField_->GetMapHeight() - static_cast<int>((model_->transform.translate.y - mapField_->GetOldDistance()) / 2);
	int column = static_cast<int>(model_->transform.translate.x / 2);

	int minRow = row;
	int movedColumn = column;

	// 探索
	std::vector<std::vector<bool>> seen(mapField_->GetMapHeight(), std::vector<bool>(mapField_->GetMapWidth(), false));
	std::list<std::pair<int, int>> queue;
	queue.emplace_back(row, column);
	seen[row][column] = true;

	// 移動用lambda
	auto moveFunc = [&](int r, int c, int moveColumnDir) {
		// 地面
		std::vector<int> ground = (r + 1 == mapField_->GetMapHeight()) ?
			std::vector<int>((int32_t)mapField_->GetMapWidth(), 1) :
			mapField_->GetMapRows(r + 1);

		int reachedColumn = c + moveColumnDir;
		// 範囲外チェック
		if (reachedColumn >= 0 && reachedColumn < static_cast<int>(mapField_->GetMapWidth())) {
			// 地面かつ壁でない
			if (mapField_->GetMapRows(r)[reachedColumn] == 0 &&
				ground[reachedColumn] == 1) {
				// 未探索なら追加
				if (!seen[r][reachedColumn]) {
					queue.emplace_back(r, reachedColumn);
					seen[r][reachedColumn] = true;
				}
			}
			// 1マス上に登れるか
			else if (r - 1 >= 0 &&
				mapField_->GetMapRows(r)[reachedColumn] == 1 &&
				mapField_->GetMapRows(r-1)[reachedColumn] == 0) {
				// 未探索なら追加
				int up = r - 1;
				if (!seen[up][reachedColumn]) {
					queue.emplace_back(up, reachedColumn);
					seen[up][reachedColumn] = true;
					// 最小高更新
					if (minRow > up) {
						minRow = up;
						movedColumn = reachedColumn;
					}
				}
			}
		}
	};

	while (!queue.empty()) {
		auto [currentRow, currentColumn] = queue.front();
		queue.pop_front();

		// 左
		moveFunc(currentRow, currentColumn, -1);
		// 右
		moveFunc(currentRow, currentColumn, 1);
	}

	// 位置を移動
	model_->transform.translate.x = movedColumn * 2.0f;
	float oldDistance = mapField_->GetOldDistance();
	model_->transform.translate.y = ((int)mapField_->GetMapHeight() - minRow) * 2.0f + oldDistance;
}

void Climber::OldUp() {
	model_->transform.translate.y = mapField_->GetOldDistance();
	model_->transform.translate += kStartPos_;
}

std::vector<int> Climber::CalcGroundBlocks() {
	const size_t row = mapField_->GetMapHeight() - static_cast<int32_t>((model_->transform.translate.y - mapField_->GetOldDistance()) / 2);
	std::vector<int> mapRow((int32_t)mapField_->GetMapRows(0).size(), 1);
	// 一つ下の行
	if (row + 1 != mapField_->GetMapHeight()) {
		mapRow = mapField_->GetMapRows(row + 1);
	}

	// 壁になるミノ
	const std::vector<int> wallBlockRow = mapField_->GetMapRows(row);

	// 行動できない範囲の地面を0にする
	{	// 左
		int32_t left = static_cast<int32_t>(model_->transform.translate.x / 2);
		bool isMoveable = true;
		for (; left >= 0; --left) {
			if (!isMoveable) {
				mapRow[left] = 0;
			}
			else {
				if (mapRow[left] == 0) {
					isMoveable = false;
					mapRow[left] = 0;
				}
			}
		}
	}
	{	// 右
		int32_t right = static_cast<int32_t>(model_->transform.translate.x / 2);
		bool isMoveable = true;
		for (; right < static_cast<int32_t>(mapRow.size()); ++right) {
			if (!isMoveable) {
				mapRow[right] = 0;
			}
			else {
				if (mapRow[right] == 0) {
					isMoveable = false;
					mapRow[right] = 0;
				}
			}
		}
	}

	// 壁以降の範囲を地面なしにする
	{	// 左
		int32_t left = static_cast<int32_t>(model_->transform.translate.x / 2);
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
		int32_t right = static_cast<int32_t>(model_->transform.translate.x / 2);
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
