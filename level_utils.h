#pragma once
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

std::vector<std::string> read_file(std::string filename);
std::vector<gameObject> format_contents(std::vector<std::string> contents);
std::map<std::string, std::string> parse_config(std::string filepath);
