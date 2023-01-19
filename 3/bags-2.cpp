#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

int calculatePriority(std::string elf1, std::string elf2, std::string elf3)
{
    std::sort(elf1.begin(), elf1.end());
    std::sort(elf2.begin(), elf2.end());
    std::sort(elf3.begin(), elf3.end());

    std::string intersection {};
    std::string intersection2 {};
    std::set_intersection(elf1.begin(), elf1.end(), elf2.begin(), elf2.end(), std::back_inserter(intersection));
    std::set_intersection(intersection.begin(), intersection.end(), elf3.begin(), elf3.end(), std::back_inserter(intersection2));

    int priority {intersection2[0]};
    priority -= (priority < 91) ? 38 : 96;  

    return priority;
}

int main()
{
    std::ifstream inf {"input.txt"};

    int priorities {};
    std::string elf1, elf2, elf3;

    while(inf && std::getline(inf, elf1) && std::getline(inf, elf2) && std::getline(inf, elf3))
    {
        priorities += calculatePriority(elf1, elf2, elf3);
    }

    std::cout << "The sum of all priorities is " << priorities << "!\n";
}