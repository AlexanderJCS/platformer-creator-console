#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>


struct gameObject
{
	char symbol = 'a';
	float x = 0;
	float y = 0;
	float xVel = 0;
	float yVel = 0;
};


std::vector<std::string> readFile(std::string filename)
{
	std::ifstream file(filename);
	
	if (!file.is_open())
	{
		std::cout << "File " << filename << " could not open." << std::endl;
		exit(1);
	}

	std::vector<std::string> contents;
	std::string line;
	
	while (getline(file, line))
	{
		contents.push_back(line);
	}

	file.close();
	
	return contents;
}


std::vector<gameObject> formatContents(std::vector<std::string> contents)
{
	std::vector<gameObject> formatted_contents;
	
	for (int y = 0; y < contents.size(); y++)
	{
		for (int x = 0; x < contents[y].size(); x++)
		{
			if (contents[y][x] == ' ')
			{
				continue;
			}

			gameObject obj;
			obj.symbol = contents[y][x];
			obj.x = x;
			obj.y = y;
			
			formatted_contents.push_back(obj);
		}
	}

	// Sort the objects by their x position to allow binary search
	std::sort(formatted_contents.begin(), formatted_contents.end(), [](gameObject a, gameObject b) { return a.x < b.x; });

	return formatted_contents;
}


std::map<std::string, std::string> readConfig(std::string filepath)
{
	std::map<std::string, std::string> config;
	std::vector<std::string> contents = readFile(filepath);
	
	for (int i = 0; i < contents.size(); i++)
	{
		std::string line = contents[i];
		std::string key = line.substr(0, line.find(' '));
		std::string value = line.substr(line.find(' ') + 1);
		config[key] = value;
	}
	
	return config;
}


void writeConfig(std::map<std::string, std::string> map, std::string filename)
{
	std::ofstream file(filename);
	
	if (!file.is_open())
	{
		std::cout << "Could not open file " << filename << " for writing." << std::endl;
		return;
	}

	for (auto const& [key, value] : map)
	{
		file << key << " " << value << std::endl;
	}
	
	file.close();
}


std::vector<std::string> getDirectories(std::string filepath)
{
	std::vector<std::string> directories;

	for (const auto& file : std::filesystem::directory_iterator{ filepath })
	{
		std::cout << file.path().string().substr(7, file.path().string().length() - 7) << std::endl;
	}
	
	return directories;
}