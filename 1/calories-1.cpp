#include <fstream>
#include <iostream>
#include <string>


int main()
{
    std::ifstream inf {"data.txt"};
    if (!inf)
    {
        std::cerr << "data.txt could not be opened for reading!\n";
        return 1;
    }

    int maxCalories {};
    std::string input {};

    while(inf)
    {
        int curCalories {};

        while (std::getline(inf, input))
        {
            if (input.empty()) break; //move onto next elf
            curCalories += std::stoi(input);
        }
        maxCalories = std::max(maxCalories,curCalories);
    }

    std::cout << "The elf with the most calories is carrying " << maxCalories << " calories!\n";
}