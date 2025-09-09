#pragma once

#include <memory>
#include <vector>

#include <Engine/Math/Matrix/MatrixCalculation.h>
#include <Engine/Model/Sprite.h>

#include <json.hpp>

enum class GenderType {
	None = 0,
	Man = 1,
	Woman = 2,
};

class MapField;

class Mino {
public:
	struct Block {
		Vector3 offset;
		std::unique_ptr<Sprite> sprite;
		std::unique_ptr<Sprite> buttonTexture;
	};

public:
	Mino() = default;
	~Mino() = default;

	void Initialize();

	void Load(const nlohmann::json& minoJson, MapField* const mapField);

	void Update();

	void DrawBlocks();

	void DrawButton();

	void DebugGUI();

	void OnUsedMino();

	void OnSelectedTable();

	void AdjustPosition(MapField* const mapField, i32 rowSize, i32 colSize);

	//========================================================================*/
	//* Getter
	const std::vector<std::unique_ptr<Block>>& GetBlocks() const {
		return blocks_;
	}
	std::vector<std::unique_ptr<Block>>& GetBlock() {
		return blocks_;
	}
	GenderType GetGender() { return gender_; }

	Trans& GetTransform();
	const Trans& GetTransform() const;

	void SetButtonPosition(const Vector3& pos) { buttonPosition = pos; }

private:
	Trans transform;

	Vector3 buttonPosition;

	GenderType gender_;

	i32 minRow;
	i32 minColumn;
	i32 maxRow;
	i32 maxColumn;

	std::vector<std::unique_ptr<Block>> blocks_;

	i32 numMaxUse;
	i32 numUseRest;
};
