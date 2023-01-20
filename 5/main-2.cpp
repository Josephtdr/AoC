#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoclib.hpp"

using ship_t = std::vector<std::vector<char>>;

ship_t setupShip(std::ifstream& inf)
{
    ship_t ship(9);
    std::string row{};

    while(inf && std::getline(inf, row))
    {
        if (row.find_first_of('[') == std::string::npos) break;

        auto crates {splitString(row, 4)};
        
        for (int i {}; i<crates.size(); ++i)
        {
            if (std::all_of(crates[i].begin(), crates[i].end(), isspace))
                continue;

            ship[i].push_back( crates[i][1] );
        }   
    }

    for (auto& col : ship)
        std::reverse(col.begin(), col.end());

    return ship;
}

int main()
{
    std::ifstream inf {"input.txt"};
    auto ship {setupShip(inf)};

    std::string instruction {};
  
    while(inf && std::getline(inf, instruction))
    {
        if (instruction.empty()) continue;

        auto row {splitString(instruction, ' ')};

        int num {std::stoi(row[1])};
        int from {std::stoi(row[3]) - 1};
        int to {std::stoi(row[5]) - 1};

        for (int i{}; i < num; ++i)
        {
            ship[to].insert(ship[to].end()-i, ship[from].back());
            ship[from].pop_back();
        }
    }

    std::cout << "The top crate in each stack is: ";
    for (const auto& col : ship)
        std::cout << col.back();
    std::cout << "\n";
}