#pragma once

#include <vector>
#include <sstream>
#include <string>

void splitString(std::vector<std::string>& result, std::string str, char delimiter);
std::vector<std::string> splitString(std::string str, char delimiter);
std::vector<std::string> splitString(std::string str, std::string delimiter);

void splitString(std::vector<std::string>& result, std::string str, int len);
std::vector<std::string> splitString(std::string str, int len);






