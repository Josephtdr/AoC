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

std::vector<std::string> splitString(std::string str, std::string delimiter)
{
    size_t pos = 0;
    std::vector<std::string> result {};

    while ((pos = str.find(delimiter)) != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);
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