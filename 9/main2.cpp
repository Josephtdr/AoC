#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <vector>

#include "../aoclib.hpp"

struct Coord
{
    int x;
    int y;

    bool operator==(const Coord& other) const { return x == other.x && y == other.y; };
    bool operator==(const Coord& other) { return x == other.x && y == other.y; };
    bool operator<(const Coord& other) { return x < other.x || (x == other.x && y < other.y); };
    void operator+=(const Coord& other) { x += other.x; y += other.y; };
};

void updateTail(const Coord& head, Coord& tail)
{
    int xdist {head.x-tail.x};
    int ydist {head.y-tail.y};
 
    if (std::abs(xdist) <= 1 && std::abs(ydist) <= 1)
        return;

    if (std::abs(ydist)==2)
        ydist/=2;
    if (std::abs(xdist)==2)
        xdist/=2;
    
    tail += {xdist, ydist};
}

void updateRope(std::vector<Coord>& rope)
{
    for (auto h {rope.begin()}; h<rope.end()-1; ++h)
        updateTail(*h, *(h+1));
}

int main()
{
    std::ifstream inf {"input.txt"};

    std::vector<Coord> rope(10);

    
    const std::unordered_map<char, Coord> directions{
        {'U', {0,1}}, {'D', {0,-1}}, {'L', {-1,0}}, {'R', {1,0}},
    };
    
    auto hash = [](const Coord& p) { return std::hash<int>()(p.x) * 31 + std::hash<int>()(p.y); };
    auto equal = [](const Coord& p1, const Coord& p2) { return p1==p2; };
    std::unordered_set<Coord,decltype(hash),decltype(equal)> visitedSet (8,hash,equal);

    std::string instruction {};
    while(inf && std::getline(inf, instruction))
    {
        auto cmds = splitString(instruction, ' ');

        for (int i{}; i<std::stoi(cmds[1]); ++i)
        {
            rope.front() += directions.at(cmds[0][0]);
            updateRope(rope);
            visitedSet.insert(rope.back());
        }
    }
    
    std::cout << "The number of tiles visited at least once is " << visitedSet.size() << "\n";
}