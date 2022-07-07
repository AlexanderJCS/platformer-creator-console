#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


struct gameObject
{
	char symbol = 'a';
	float x = 0;
	float y = 0;
	float xVel = 0;
	float yVel = 0;
};


std::vector<std::string> read_file(std::string filename)
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


std::vector<gameObject> format_contents(std::vector<std::string> contents)
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
