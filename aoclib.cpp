#include "aoclib.hpp"

void splitString(std::vector<std::string>& result, std::string str, char delimiter)
{
    std::stringstream ss(str);

    while (ss.good()) {
        std::string substr;
        getline(ss, substr, delimiter);
        result.push_back(substr);
    }
}

std::vector<std::string> splitString(std::string str, char delimiter)
{
    std::vector<std::string> result {};

    std::stringstream ss(str);

    while (ss.good()) {
        std::string substr;
        getline(ss, substr, delimiter);
        result.push_back(substr);
    }

    return result;
}

void splitString(std::vector<std::string>& result, std::string str, int len)
{
    for (int i {}; i < str.length(); i+= len)
    {
        result.push_back(str.substr(i, len));
    }
}

std::vector<std::string> splitString(std::string str, int len)
{
    std::vector<std::string> result {};

    for (int i {}; i < str.length(); i+= len)
    {
        result.push_back(str.substr(i, len));
    }
    return result;
}