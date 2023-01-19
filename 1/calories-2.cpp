#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

void updateCalories(std::vector<int>& topElves, int curCalories)
{
    //iterator pointing to first element greater than curCalories
    auto upper {std::upper_bound(topElves.begin(), topElves.end(), curCalories)};

    //if curCalories is less than the 3rd elf's amount, return.
    if (upper == topElves.begin()) return;

    topElves.insert(upper, curCalories);
    topElves.erase(topElves.begin());
}

int main()
{
    std::ifstream inf {"data.txt"};
    if (!inf)
    {
        std::cerr << "data.txt could not be opened for reading!\n";
        return 1;
    }

    std::vector topElves {0,0,0}; 
    std::string input {};

    while(inf)
    {
        int curCalories {};

        while (std::getline(inf, input))
        {
            if (input.empty()) break; //move onto next elf
            curCalories += std::stoi(input);
        }
        updateCalories(topElves, curCalories);
    }

    int total {std::accumulate(begin(topElves), end(topElves), 0, std::plus<int>())};

    std::cout << "The 3 elves with the most calories are carrying " << total << " calories!\n";
}