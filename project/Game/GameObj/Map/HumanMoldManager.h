#pragma once

#include <array>
#include <filesystem>
#include <vector>

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

	std::array<i32, 3> random_select();

	const Mold& get_mold(MoldType type, i32 index);

private:
	MoldType load_mold(const std::filesystem::path& path, HumanMoldManager::Mold& result);

private:
	std::vector<Mold> smallMolds;
	std::vector<Mold> mediumMolds;
	std::vector<Mold> largeMolds;
};
