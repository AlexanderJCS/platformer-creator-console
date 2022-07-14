#include <windows.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include "level_utils.h"

const short SCREEN_WIDTH = 30;
const short SCREEN_HEIGHT = 10;
const float GRAVITY = 0.35;
const float JUMP_VEL = 1.9;


void swapVars(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}


void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}


class Game
{
	// Allow for multiple player objects
	// All player objects have the same controls, this is not multiplayer
	// Managing two players can make for some interesting levels, though
	std::vector<gameObject> players;
	std::vector<gameObject> gameObjects;
	std::vector<gameObject> goals;
	std::vector<gameObject> roombas;
	std::vector<gameObject> spikes;
	
	std::map<std::string, std::string> colorData;

	int width = 0;
	int height = 0;

	// Finds the char of the object at x, y
	// Mainly used for the draw method
	char findCharAtCoords(int x, int y, std::vector<gameObject> objects)
	{
		if (x < 0 || x > width || y < 0 || y > height)
		{
			return ' ';
		}

		// Find the index of the x value
		int xIndex = findItemIndex(x, 0, objects.size(), objects);

		// If it could not find the index, return nothing
		if (xIndex == -1)
		{
			return ' ';
		}

		// Go to the first index with the x value
		for (int i = xIndex; x > 0 && objects[i].x == x; i--)
		{
			xIndex = i;
		}

		// Var index is now at the first value
		// Now, iterate through all x values to see if the y matches
		for (int i = xIndex; i < objects.size() && i >= 0 && round(objects[i].x) == x; i++)
		{
			if (round(objects[i].y) == y)
			{
				return objects[i].symbol;
			}
		}

		return ' ';
	}

	// Finds the index of an item with a certain x value
	int findItemIndex(int target, int start, int end, std::vector<gameObject> objects)
	{
		unsigned int middle = (start + end) / 2;

		if (objects[middle].x == target)
		{
			return middle;
		}

		if (start + 1 == end)
		{
			return -1;
		}

		if (objects[middle].x > target)
		{
			return findItemIndex(target, start, middle, objects);
		}

		if (objects[middle].x < target)
		{
			return findItemIndex(target, middle, end, objects);
		}

		// Not needed but prevents a compiler warning
		return -1;
	}

	bool validMove(int x, int y, std::vector<gameObject> colliders)
	{
		if (x <= 0 || x > width)
		{
			return false;
		}

		if (findCharAtCoords(x, y, colliders) == ' ')
		{
			return true;
		}

		return false;
	}

	void draw()
	{
		std::vector<gameObject> allObjects = gameObjects;
		allObjects.insert(allObjects.end(), goals.begin(), goals.end());
		allObjects.insert(allObjects.end(), players.begin(), players.end());
		allObjects.insert(allObjects.end(), roombas.begin(), roombas.end());

		std::sort(allObjects.begin(), allObjects.end(), [](gameObject a, gameObject b) { return a.x < b.x; });

		for (auto& object : allObjects)
		{
			object.x = round(object.x);
			object.y = round(object.y);
		}

		std::cout << "\033[1;1H";  // Move cursor to top left

		// Draw top border
		for (short i = 0; i <= SCREEN_WIDTH + 1; i++)
		{
			std::cout << "##";
		}

		std::cout << "\n";

		// Print the main screen
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			std::cout << "#";

			for (int x = players[0].x - SCREEN_WIDTH / 2;
				x <= players[0].x + SCREEN_WIDTH / 2;
				x++)
			{
				char symbol = findCharAtCoords(x, y, allObjects);

				if (colorData.find(std::string(1, symbol)) != colorData.end())
				{
					std::string ansi_code = colorData[std::string(1, symbol)];
					std::cout << "\u001b[" + ansi_code;
				}

				std::cout << symbol << " \u001b[0m";
			}
			std::cout << "#\n";
		}

		// Draw bottom border
		for (short i = 0; i <= SCREEN_WIDTH + 1; i++)
		{
			std::cout << "##";
		}
	}

	void applyGravity()
	{
		for (auto& player : players)
		{
			player.yVel += GRAVITY;
		}
	}

	void moveObjects(std::vector<gameObject>& objects)
	{
		for (auto& object : objects)
		{
			float originalX = object.x;
			float originalY = object.y;
			int x1 = round(object.x);
			int y1 = round(object.y);
			int x2 = round(object.x + object.xVel);
			int y2 = round(object.y + object.yVel);

			if (x1 > x2)
			{
				swapVars(x1, x2);
			}

			if (y1 > y2)
			{
				swapVars(y1, y2);
			}

			for (int i = x1; i <= x2; i++)
			{
				if (!validMove(i, round(object.y), gameObjects))
				{
					object.xVel = 0;
					break;
				}
			}

			if (object.xVel != 0)
			{
				object.x += object.xVel;
			}

			for (int i = y1; i <= y2; i++)
			{
				if (!validMove(round(object.x), i, gameObjects))
				{
					object.yVel = 0;
					break;
				}

				object.y = i;
			}

			if (object.yVel != 0)  // If the loop was not broken out of
			{
				object.y = originalY + object.yVel;
			}
		}
	}

	void getInput()
	{
		// Jump
		if (GetKeyState('W') & 0x8000)
		{
			for (auto& player : players)
			{
				if (!validMove(round(player.x), std::floor(player.y + 1), gameObjects) && player.yVel == 0)
				{
					player.yVel -= JUMP_VEL;
				}
			}
		}

		// Move right
		if (GetKeyState('D') & 0x8000)
		{
			for (auto& player : players)
			{
				player.xVel = 1;
			}
		}

		// Move left
		else if (GetKeyState('A') & 0x8000)
		{
			for (auto& player : players)
			{
				player.xVel = -1;
			}
		}

		else
		{
			for (auto& player : players)
			{
				player.xVel = 0;
			}
		}
	}

	bool died()
	{
		for (auto& player : players)
		{
			if (player.y > height && player.y > SCREEN_HEIGHT)
			{
				return true;
			}
		}

		return false;
	}

	/*
	Check if a game object is outside the screen. If it is,
	expand the width or height to fit it.
	*/

	void checkIfNewSize(gameObject gameObj)
	{
		if (gameObj.x > width)
		{
			width = gameObj.x;
		}

		if (gameObj.y > height)
		{
			height = gameObj.y;
		}
	}

	bool won()
	{
		for (auto& player : players)
		{
			for (auto& goal : goals)
			{
				if (round(player.x) == round(goal.x) &&
					round(player.y) == round(goal.y))
				{
					return true;
				}
			}
		}

		return false;
	}

	/*
	Reverse the velocities of the roombas if it is at vel = 0.
	*/

	void turnRoombas()
	{
		for (auto& roomba : roombas)
		{
			if (roomba.xVel == 0)
			{
				return;
			}

			if (!validMove(round(roomba.x - 1), round(roomba.y), gameObjects))
			{
				roomba.xVel = 0.25;
			}

			else if (!validMove(round(roomba.x + 1), round(roomba.y), gameObjects))
			{
				roomba.xVel = -0.25;
			}
		}
	}

	/*
	Checks if the player died to an enemy or spike
	*/

	bool diedToEnemy()
	{
		std::vector<gameObject> allDeathObjects = gameObjects;
		allDeathObjects.insert(allDeathObjects.end(), roombas.begin(), roombas.end());
		allDeathObjects.insert(allDeathObjects.end(), spikes.begin(), spikes.end());

		std::sort(allDeathObjects.begin(), allDeathObjects.end(), [](gameObject a, gameObject b) { return a.x < b.x; });
		
		for (auto& deathObject : allDeathObjects)
		{
			deathObject.x = round(deathObject.x);
			deathObject.y = round(deathObject.y);
		}

		for (auto& player : players)
		{
			if (!validMove(round(player.x), round(player.y), allDeathObjects))
			{
				return true;
			}
		}

		return false;
	}

public:
	Game()
	{
		std::cout << "Input the level name. Available levels:\n";
		std::vector<std::string> levels = getDirectories("levels");

		for (auto& level : levels)
		{
			std::cout << level;
		}

		std::cout << "\nChoose your level\n> ";
		std::string levelname;
		std::cin >> levelname;

		auto contents = read_file("levels/" + levelname + "/level.txt");
		auto formatted = format_contents(contents);

		// Decide if the gameObject goes in the players or gameObjects vector
		for (gameObject& gameObj : formatted)
		{
			checkIfNewSize(gameObj);

			if (gameObj.symbol == 'O')
			{
				players.push_back(gameObj);
			}

			else if (gameObj.symbol == 'G')
			{
				goals.push_back(gameObj);
			}

			else if (gameObj.symbol == '@')
			{
				gameObj.xVel = -0.25;
				roombas.push_back(gameObj);
			}

			else if (gameObj.symbol == '*')
			{
				spikes.push_back(gameObj);
			}

			else
			{
				gameObjects.push_back(gameObj);
			}
		}

		// Get the color map
		colorData = parse_config("levels/" + levelname + "/colors.txt");
	}

	void run()
	{
		system("cls");
		
		while (true)
		{
			applyGravity();

			moveObjects(players);
			moveObjects(roombas);
			turnRoombas();

			ShowConsoleCursor(false);
			getInput();
			draw();

			if (won())
			{
				std::cout << "\nYou won!" << std::endl;
				Sleep(4000);
				return;
			}

			if (diedToEnemy())
			{
				std::cout << "\nYou died to an enemy!" << std::endl;
				Sleep(4000);
				return;
			}

			if (died())
			{
				std::cout << "\nYou died." << std::endl;
				Sleep(4000);
				return;
			}

			Sleep(50);
		}
	}
};


int main()
{
	Game g;
	g.run();
	return 0;
}