#pragma once

#include <string>
#include <vector>

#include <Engine/Math/Vector/Vector3.h>

#include "Game/GameObj/Map/MapField.h"

class MinoEditor {
public:
	struct Data {
		std::string name;
		Vector3 color;
		std::vector<std::vector<i32>> blocks;
		i32 numMaxUse{ 1 };
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
