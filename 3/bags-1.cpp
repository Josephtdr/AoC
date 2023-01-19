#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

int calculatePriority(std::string input)
{
    auto len {input.size()};

    auto first {input.substr(0,len/2)};
    auto second {input.substr((len/2), len/2)};
    
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());

    std::string intersection {};
    std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(intersection));
    
    int priority {intersection[0]};
    priority -= (priority < 91) ? 38 : 96;  

    return priority;
}

int main()
{
    std::ifstream inf {"input.txt"};

    int priorities {};
    std::string input {};

    while(inf && std::getline(inf, input))
    {
        priorities += calculatePriority(input);
    }

    std::cout << "The sum of all priorities is " << priorities << "!\n";
}