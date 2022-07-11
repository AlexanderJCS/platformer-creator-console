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
		for (int i = xIndex; x >= 0 && objects[i].x == x; i--)
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
		if (start + 1 == end)
		{
			return -1;
		}

		int middle = (start + end) / 2;

		if (objects[middle].x == target)
		{
			return middle;
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

	bool validMove(int x, int y)
	{
		if (x <= 0 || x > width)
		{
			return false;
		}

		if (findCharAtCoords(x, y, gameObjects) == ' ')
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

		std::sort(allObjects.begin(), allObjects.end(), [](gameObject a, gameObject b) { return a.x < b.x; });

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

	void movePlayers()
	{
		for (auto& player : players)
		{
			float originalX = player.x;
			float originalY = player.y;
			int x1 = round(player.x);
			int y1 = round(player.y);
			int x2 = round(player.x + player.xVel);
			int y2 = round(player.y + player.yVel);

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
				if (!validMove(i, round(player.y)))
				{
					player.xVel = 0;
					break;
				}
			}

			if (player.xVel != 0)
			{
				player.x += player.xVel;
			}

			for (int i = y1; i <= y2; i++)
			{
				if (!validMove(round(player.x), i))
				{
					player.yVel = 0;
					break;
				}

				player.y = i;
			}

			if (player.yVel != 0)  // If the loop was not broken out of
			{
				player.y = originalY + player.yVel;
			}
		}
	}

	void getInput()
	{
		// Jump
		if (GetAsyncKeyState('W'))
		{
			for (auto& player : players)
			{
				if (!validMove(round(player.x), std::floor(player.y + 1)) && player.yVel == 0)
				{
					player.yVel -= JUMP_VEL;
				}
			}
		}

		// Move right
		if (GetAsyncKeyState('D'))
		{
			for (auto& player : players)
			{
				player.xVel = 1;
			}
		}

		// Move left
		else if (GetAsyncKeyState('A'))
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

public:
	Game()
	{
		std::cout << "Input the level name:\n> ";
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
		while (true)
		{
			applyGravity();
			movePlayers();
			ShowConsoleCursor(false);
			draw();
			getInput();

			if (won())
			{
				std::cout << "\nYou won!" << std::endl;
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
	system("cls");
	Game g;
	g.run();
	return 0;
}