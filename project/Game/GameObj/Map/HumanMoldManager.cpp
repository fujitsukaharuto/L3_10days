#include "HumanMoldManager.h"

#include <fstream>

#include <Engine/Math/Random/Random.h>

void HumanMoldManager::load() {
	for (auto& entry : std::filesystem::directory_iterator("resource/Json/HumanMold/")) {
		Mold mold;
		MoldType type = load_mold(entry.path(), mold);
		switch (type) {
		case MoldType::Small:
			smallMolds.emplace_back(std::move(mold));
			break;
		case MoldType::Medium:
			mediumMolds.emplace_back(std::move(mold));
			break;
		case MoldType::Large:
			largeMolds.emplace_back(std::move(mold));
			break;
		}
	}
}

const HumanMoldManager::Mold& HumanMoldManager::random_select(MoldType type) {
	switch (type) {
	case MoldType::Small:
		if (!smallMolds.empty()) {
			int index = Random::GetInt(0, static_cast<int>(smallMolds.size()) - 1);
			return smallMolds[index];
		}
		break;
	case MoldType::Medium:
		if (!mediumMolds.empty()) {
			int index = Random::GetInt(0, static_cast<int>(mediumMolds.size()) - 1);
			return mediumMolds[index];
		}
		break;
	case MoldType::Large:
		if (!largeMolds.empty()) {
			int index = Random::GetInt(0, static_cast<int>(largeMolds.size()) - 1);
			return largeMolds[index];
		}
		break;
	}

	static Mold empty;
	return empty;
}

MoldType HumanMoldManager::load_mold(const std::filesystem::path& path, HumanMoldManager::Mold& result) {
	std::ifstream file(path);

	std::string line;
	std::getline(file, line, '\n');

	MoldType type = static_cast<MoldType>(line[0] - '0');

	i32 size = 0;
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);
		std::string value;
		auto& row = result.body.emplace_back();
		while (std::getline(ss, value, ',')) {
			if (value == "1") {
				row.emplace_back(true);
				++size;
			}
			else if (value == "0") {
				row.emplace_back(false);
			}
		}
	}

	result.size = size;

	return type;
}
