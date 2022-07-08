#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "level_utils.h"


const short SCREEN_WIDTH = 30;
const short SCREEN_HEIGHT = 10;
const float GRAVITY = 0.35;
const float JUMP_VEL = 1.9;


void swapVars(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}


class Game
{
	// Allow for multiple player objects
	// All player objects have the same controls, this is not multiplayer
	// Managing two players can make for some interesting levels, though
	std::vector<gameObject> players;
	std::vector<gameObject> gameObjects;

	int width = 0;
	int height = 0;

	// Finds the char of the object at x, y
	// Mainly used for the draw method
	char findCharAtCoords(int x, int y)
	{
		if (x < 0 || x >= width || y < 0 || y > height)
		{
			return ' ';
		}
		
		// Find the index of the x value
		int xIndex = findItemIndex(x, 0, gameObjects.size());
		
		if (xIndex == -1)
		{
			return ' ';
		}

		// Go to the first index with the x value
		for (int i = xIndex; gameObjects[i].x == x && x >= 0; i--)
		{
			xIndex = i;
		}

		// Var index is now at the first value
		// Now, iterate through all x values to see if the y matches
		for (int i = xIndex; gameObjects[i].x == x; i++)
		{
			if (gameObjects[i].y == y)
			{
				return gameObjects[i].symbol;
			}
		}

		return ' ';
	}

	// Finds the index of an item with a certain x value
	int findItemIndex(int target, int start, int end)
	{
		if (start + 1 == end)
		{
			return -1;
		}
		
		int middle = (start + end) / 2;

		if (gameObjects[middle].x == target)
		{
			return middle;
		}

		if (gameObjects[middle].x > target)
		{
			return findItemIndex(target, start, middle);
		}

		if (gameObjects[middle].x < target)
		{
			return findItemIndex(target, middle, end);
		}
		
		// Not needed but prevents a compiler warning
		return -1;
	}

	bool validMove(int x, int y)
	{
		if (x <= 0 || x >= width)
		{
			return false;
		}
		
		int index = findItemIndex(x, 0, gameObjects.size());

		if (index == -1)
		{
			return true;
		}

		// Go to the first index with the x value
		for (int i = index; gameObjects[i].x == x; i--)
		{
			index = i;
		}

		// Var index is now at the first value
		// Now, iterate through all x values to see if the y matches
		for (int i = index; gameObjects[i].x == x; i++)
		{
			if (gameObjects[i].y == y)
			{
				return false;
			}
		}

		return true;
	}

	void draw()
	{
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
				// Iterate through all player objects and see if the x and y match
				// This is less efficient than it could be, but it does not matter
				// Considering most of the time, there is only 1 player object
				bool foundPlayer = false;

				for (auto& player : players)
				{
					if (round(player.x) == x && round(player.y) == y)
					{
						std::cout << "O ";
						foundPlayer = true;
						break;
					}
				}
				
				if (!foundPlayer)
				{ 
					std::cout << findCharAtCoords(x, y) << " ";
				}
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
			int originalX = round(player.x);
			int originalY = round(player.y);
			int newX = round(player.x + player.xVel);
			int newY = round(player.y + player.yVel);

			if (originalX > newX)
			{
				swapVars(originalX, newX);
			}

			if (originalY > newY)
			{
				swapVars(originalY, newY);
			}

			for (int i = originalX; i <= newX; i++)
			{
				if (!validMove(i, round(player.y)))
				{
					player.xVel = 0;
					break;
				}
			}
			
			player.x = player.x + player.xVel;

			for (int i = originalY; i <= newY; i++)
			{
				if (!validMove(round(player.x), i))
				{
					player.yVel = 0;
					break;
				}
			}

			player.y = player.y + player.yVel;
		}
	}

	void getInput()
	{
		// Jump
		if (GetAsyncKeyState('W'))
		{
			for (auto& player : players)
			{
				if (!validMove(round(player.x), std::floor(player.y + 1)))
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

public:
	Game()
	{
		std::cout << "Input the level filename:\n> ";
		std::string filename;
		std::cin >> filename;
		
		auto contents = read_file(filename);
		auto formatted = format_contents(contents);

		// Decide if the gameObject goes in the players or gameObjects vector
		for (gameObject& gameObj : formatted)
		{
			if (gameObj.symbol == 'P')
			{
				players.push_back(gameObj);
			}

			else
			{
				gameObjects.push_back(gameObj);

				if (gameObj.x > width)
				{
					width = gameObj.x;
				}

				if (gameObj.y > height)
				{
					height = gameObj.y;
				}
			}
		}
	}

	void run()
	{
		while (true)
		{
			draw();
			applyGravity();
			movePlayers();
			getInput();
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