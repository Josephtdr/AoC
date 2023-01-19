#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoclib.hpp"

bool isContained(std::string assignments)
{
    std::vector<std::string> elves, elf1Bounds, elf2Bounds;
    splitString(elves, assignments, ',');
    
    splitString(elf1Bounds, elves[0], '-');
    splitString(elf2Bounds, elves[1], '-');

    std::vector<int> elf1( (std::stoi(elf1Bounds[1]) - std::stoi(elf1Bounds[0])) + 1 );
    std::iota(elf1.begin(), elf1.end(), std::stoi(elf1Bounds[0]));

    std::vector<int> elf2( (std::stoi(elf2Bounds[1]) - std::stoi(elf2Bounds[0])) + 1 );
    std::iota(elf2.begin(), elf2.end(), std::stoi(elf2Bounds[0]));
    
    auto it = std::search(elf1.begin(), elf1.end(), elf2.begin(), elf2.end());
    if (it != elf1.end())
        return true;
    it = std::search(elf2.begin(), elf2.end(), elf1.begin(), elf1.end());
    if (it != elf2.end())
        return true;

    return false;
}

int main()
{
    std::ifstream inf {"input.txt"};

    int num {};
    std::string assignments;

    while(inf && std::getline(inf, assignments))
    {
        if (isContained(assignments)) ++num;
    }

    std::cout << "The num contaied fully is " << num << "!\n";
}