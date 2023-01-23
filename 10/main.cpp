#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../aoclib.hpp"


struct CPU
{
    int cycle{};
    int X{1};

    int sum{};

    std::vector<std::string> screen {};
    int scrnPtr {-1};

    void increment() 
    {
        if (cycle%40==0)
            screen.push_back({});

        if (X-1<= (cycle%40) && (cycle%40)<=X+1)
            screen.back().append("#");
        else
            screen.back().append(".");

        if ((++cycle-20)%40==0)
            sum += X*cycle;  
    }
};

int main()
{
    std::ifstream inf {"input.txt"};

    CPU cpu{};

    std::string instruction {};
    while(inf && std::getline(inf, instruction))
    {
        if (instruction=="noop")
        {
            cpu.increment();
            continue;
        }
            
        auto cmds = splitString(instruction, ' ');

        cpu.increment();
        cpu.increment();
        cpu.X += std::stoi(cmds[1]);
    }

    for (auto& s : cpu.screen)
        std::cout << s << "\n";

    // std::cout << "The sum of the interesting signal strengths is " << cpu.sum << "\n";
}