#include "pch.h"
#include "level/GameLevel.h"
#include "manager/ResourceManager.h"

GameLevel::GameLevel()
{
	GameObject::ResetSidCounter();
}

GameLevel::~GameLevel()
{
	std::println("[GameLevel] Cleaning and deleting resources...");
	for (auto ent : m_Entities)
	{
		std::println("	- deleting object: {}", ent->GetID());
		delete ent;
	}
	std::println("[GameLevel] Cleaned up.");
}


void RemoveTrailingNewline(const std::string& filename) {
	std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
	if (!file.is_open()) return;

	file.seekg(0, std::ios::end);
	std::streamoff fileSize = file.tellg();

	if (fileSize == 0) return; // empty file

	file.seekg(fileSize - 1);
	char lastChar;
	file.get(lastChar);

	file.close(); // close before truncating
	if (lastChar == '\n') {
		// truncate the last newline
#ifdef _WIN32
		FILE* fp = nullptr;
		if (fopen_s(&fp, filename.c_str(), "rb+") == 0 && fp != nullptr) {
			_chsize_s(_fileno(fp), fileSize - 1);
			fclose(fp);
		}
#else
		truncate(filename.c_str(), fileSize - 1);
#endif
	}
}


void GameLevel::LoadLevel(const char* filePath, unsigned int lvlWidth, unsigned int lvlHeight)
{
	// clear old data
	m_Entities.clear();

	std::ifstream levelFile;
	std::vector<std::vector<int>> tileData;
	RemoveTrailingNewline(filePath);
	levelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		int row{};
		//int	col{};
		levelFile.open(filePath);
		std::string line;
		while (!levelFile.eof() && std::getline(levelFile, line))
		{
			std::stringstream ss{ line };
			std::vector<int> rowTileData;
			int num;
			//int tempCol{ 0 };

			while (ss >> num)
			{
				rowTileData.push_back(num);
				//std::cout << ch;
				//tempCol++;
			}
			row++;
			//col = std::max(col, tempCol);
			tileData.push_back(rowTileData);
		}
		//std::println("row: {}, col: {}", row, col);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error loading level file: " << filePath << "\n" << e.what() << std::endl;
	}
	levelFile.close();

	Init(tileData, lvlWidth, lvlHeight);
}

void GameLevel::Init(std::vector<std::vector<int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight)
{
	int rows{ static_cast<int>(tileData.size()) };
	int cols{ static_cast<int>(tileData[0].size()) };

	glm::vec2 pos{}/*, velocity{}*/, size{ lvlWidth / cols, lvlHeight / rows };
	//float rot{ 0.0f };
	glm::vec3 color{ 1.0f };
	const char* textureName{};

	//std::println("LEVEL: ");
	int y{ 0 };
	for (const auto& rowTiles : tileData)
	{
		int x{ 0 };
		for (const auto& tile : rowTiles)
		{
			//std::print("{}, ", tile);
			BrickType type{ static_cast<BrickType>(tile) };

			if (type == BrickType::EMPTY || tile > 5)
			{
				++x;
				continue;
			}

			switch (type)
			{
			case BrickType::SOLID_BRICK:
				color = glm::vec3(0.8f, 0.8f, 0.7f);
				textureName = "block_solid";
				break;
			case BrickType::RED_BRICK:
				color = glm::vec3(1.0f, 0.5f, 0.0f);
				textureName = "block";
				break;
			case BrickType::BLUE_BRICK:
				color = glm::vec3(0.2f, 0.6f, 1.0f);
				textureName = "block";
				break;
			case BrickType::YELLOW_BRICK:
				color = glm::vec3(0.8f, 0.8f, 0.4f);
				textureName = "block";
				break;
			case BrickType::GREEN_BRICK:
				color = glm::vec3(0.0f, 0.7f, 0.0f);
				textureName = "block";
				break;
			default:
				break;
			}

			pos = glm::vec2(x * size.x, y * size.y);
			m_Entities.push_back(new GameObject(pos, size, GET_TEXTURE(textureName), color));

			x++;
		}
		//std::println("");
		y++;
	}
	std::println("");
}

bool GameLevel::IsCompleted()
{
	return true;
}

void GameLevel::Draw(SpriteRenderer* renderer)
{
	for (const auto& ent : m_Entities)
		renderer->Draw(ent->m_Texture, ent->m_Position, ent->m_Rotation, ent->m_Size, ent->m_Color);
}