#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "GameObj/Chara/BaseChara/BaseChara.h"

struct EnemyData {
	std::string name = "NONAME";
	Gender gender = Gender::MAN;
	int32_t power = 10;
	int32_t hp = 10;
};

/// <summary>
/// 敵エディター
/// </summary>
class EnemyTableEditor {
public:
	EnemyTableEditor();
	~EnemyTableEditor() = default;

	void Draw();

	std::vector<EnemyData> GetTable();

private:
	void LoadFile();
	void SaveFile();

	std::string GenderToString(Gender g);
	Gender GenderFromString(const std::string& s);

private:
	std::vector<EnemyData> enemyTable_;
};
