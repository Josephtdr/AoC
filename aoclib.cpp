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