#include "pch.h"
#include "LevelTester.h"
#include "level/GameLevel.h"
namespace LevelTester
{


	// Utility to convert BrickType to int (for file I/O)
	inline int ToInt(BrickType type) {
		return static_cast<int>(type);
	}

	using LevelGrid = std::vector<std::vector<int>>; // ← Use plain ints instead of BrickType

	// Generate a random level of given dimensions
	LevelGrid GenerateRandomLevel(unsigned int width, unsigned int height) {
		LevelGrid level(height, std::vector<int>(width));

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> dist(0, 5); // values from 0 to 5

		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				level[y][x] = dist(rng); // ← No enum, just raw integers
			}
		}
		return level;
	}


	// Save level to file
	void SaveLevelToFile(const std::string& filename, const LevelGrid& level) {
		std::ofstream out(filename);
		if (!out) {
			std::cerr << "Failed to write file.\n";
			return;
		}

		for (const auto& row : level)
		{
			for (auto value : row)
			{
				out << value << ' ';
			}
			out << '\n';
		}
		out.close();
	}

	// Benchmark a loader function using a callback
	void BenchmarkLoader(const fn_t loader, const std::string& path) {
		auto start = std::chrono::high_resolution_clock::now();
		loader(path.c_str());
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;

		std::cout << "Loader executed in " << elapsed.count() << " seconds.\n";
	}


	int Driver(fn_t Loader) {
		const std::string filename = "levels/generated_level.lvl";
		const unsigned int width = 100;
		const unsigned int height = 100;

		// Generate and save the level
		LevelGrid level = GenerateRandomLevel(width, height);
		SaveLevelToFile(filename, level);

		// Benchmark the level loader
		BenchmarkLoader(Loader, filename);

		return 0;
	}
}
