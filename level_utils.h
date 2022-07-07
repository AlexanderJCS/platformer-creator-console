#pragma once
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

std::vector<std::string> read_file(std::string filename);
std::vector<gameObject> format_contents(std::vector<std::string> contents);
