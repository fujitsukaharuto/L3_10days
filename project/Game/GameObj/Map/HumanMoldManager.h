#pragma once

#include <vector>
#include <filesystem>

enum class MoldType {
	Small,
	Medium,
	Large,
};

class HumanMoldManager {
public:
	struct Mold {
		std::vector<std::vector<bool>> body;
		i32 size{ 0 };
	};

public:
	void load();

	const Mold& random_select(MoldType type);

private:
	MoldType load_mold(const std::filesystem::path& path, HumanMoldManager::Mold& result);

private:
	std::vector<Mold> smallMolds;
	std::vector<Mold> mediumMolds;
	std::vector<Mold> largeMolds;
};
