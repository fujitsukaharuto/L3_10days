#pragma once

#include <vector>
#include <optional>
#include <string>

#include "MinoEditor.h"

class UsableMinoEditor {
public:
	struct UsePatternData {
		std::string name;
		i32 type;
		std::vector<MinoEditor> editor;
	};

public:
	void Initialize();

	void DrawGUI();

private:
	void SerializeCheck();
	void SerializeErrorPopup();

	void Serialize() const;

	void Deserialize();

private:
	std::optional<i32> selectedIndex;
	std::optional<i32> selectedMinoIndex;

	std::vector<UsePatternData> patternData;

	std::vector<std::string> errorMessages;
};
