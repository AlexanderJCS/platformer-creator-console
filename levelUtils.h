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

std::vector<std::string> readFile(std::string filename);
std::vector<gameObject> formatContents(std::vector<std::string> contents);
std::map<std::string, std::string> readConfig(std::string filepath);
void writeConfig(std::map<std::string, std::string> map, std::string filename);
std::vector<std::string> getDirectories(std::string filepath);
