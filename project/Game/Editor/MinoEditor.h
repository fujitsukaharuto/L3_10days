#pragma once

#include <string>
#include <vector>

#include "Game/GameObj/Map/MapField.h"

class MinoEditor {
public:
	struct Data {
		std::string name;
		std::vector<std::vector<i32>> blocks;
		GenderType gender = GenderType::Man;
	};

public:
	void Initialize();

	bool DrawGUI();

public:
	const Data& GetData() const { return data; }
	void SetData(const Data& newData) { data = newData; }

private:
	Data data;
};
