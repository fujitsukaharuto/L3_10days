#include "Mino.h"

#include "Game/GameObj/Map/MapField.h"

#undef min
#undef max

void Mino::Initialize() {
	transform.scale = { 1.0f,1.0f,1.0f };
	buttonPosition = { 0.0f,0.0f,0.0f };
}

void Mino::Load(const nlohmann::json& minoJson, MapField* const mapField) {
	Vector4 color;

	if (minoJson.contains("GenderType") && minoJson["GenderType"].is_number()) {
		gender_ = static_cast<GenderType>(minoJson["GenderType"]);

		if (gender_ == GenderType::Man) {
			color = { 0.0f,0.5f,1.0f,1.0f };
		}
		else if (gender_ == GenderType::Woman) {
			color = { 1.0f,0.5f,0.8f,1.0f };
		}
		else {
			color = { 1.0f,1.0f,1.0f,0.0f };
		}

	}
	else {
		gender_ = GenderType::None;
	}

	if(minoJson.contains("NumMaxUse") && minoJson["NumMaxUse"].is_number()) {
		numMaxUse = minoJson["NumMaxUse"];
		numUseRest = numMaxUse;
	}

	float cellSize = mapField->GetCellSize();

	minRow = std::numeric_limits<i32>::max();
	minColumn = std::numeric_limits<i32>::max();
	maxRow = 0;
	maxColumn = 0;

	if (minoJson.contains("Blocks") && minoJson["Blocks"].is_array()) {
		// arrayからブロックを取得
		// Indexを持ってくる
		for (i32 rowI = 0; const nlohmann::json& row : minoJson["Blocks"]) {
			for (i32 colI = 0; const auto& col : row) {
				if (col == 1) {
					auto block = std::make_unique<Block>();

					block->offset = { static_cast<float>(colI * cellSize),static_cast<float>(rowI * cellSize),0.0f };

					block->sprite = std::make_unique<Sprite>();
					block->sprite->Load("white2x2.png");
					block->sprite->SetSize({ cellSize, cellSize });
					block->sprite->SetColor(color);
					block->sprite->SetAnchor({ 0.5f,0.5f });

					block->buttonTexture = std::make_unique<Sprite>();
					block->buttonTexture->Load("white2x2.png");
					block->buttonTexture->SetSize({ cellSize, cellSize });
					block->buttonTexture->SetColor(color);
					block->buttonTexture->SetAnchor({ 0.5f,0.5f });
					block->buttonTexture->SetPos(block->offset);
					blocks_.emplace_back(std::move(block));

					minRow = std::min(minRow, rowI);
					minColumn = std::min(minColumn, colI);
					maxRow = std::max(maxRow, rowI);
					maxColumn = std::max(maxColumn, colI);
				}
				++colI;
			}
			++rowI;
		}
	}
}

void Mino::Update() {
	for (auto& block : blocks_) {
		block->sprite->SetPos(transform.translate + block->offset);
		block->sprite->SetScale({ transform.scale.x,transform.scale.y });
	
		block->buttonTexture->SetPos(buttonPosition + block->offset);
	}
}

void Mino::DrawBlocks() {
	for (auto& block : blocks_) {
		block->sprite->Draw();
	}
}

void Mino::DrawButton() {
	for (auto& block : blocks_) {
		block->buttonTexture->Draw();
	}
}

void Mino::DebugGUI() {
#ifdef _DEBUG

#endif // _DEBUG
}

void Mino::OnUsedMino() {
	if (numUseRest > 0) {
		--numUseRest;
	}
}

void Mino::OnSelectedTable() {
	numUseRest = numMaxUse;
}

void Mino::AdjustPosition(MapField* const mapField, i32 rowSize, i32 colSize) {
	auto [rowI, colI] = mapField->CalcCellIndex(transform.translate);

	rowI = std::clamp(rowI, -minRow, rowSize - 1 - maxRow);
	colI = std::clamp(colI, -minColumn, colSize - 1 - maxColumn);

	transform.translate = { colI * mapField->GetCellSize(), rowI * mapField->GetCellSize(), 0.0f };

	Update();
}

Trans& Mino::GetTransform() {
	return transform;
}

const Trans& Mino::GetTransform() const {
	return transform;
}
