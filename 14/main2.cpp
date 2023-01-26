#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <optional>
#include <variant>

#include "../aoclib.hpp"


struct Pos
{
    int x {}; //col
    int y {}; //row

    Pos() = default;
    Pos(int X, int Y)
        : x {X}, y {Y}
    {}
    Pos(std::string str)
    {
        auto coords = splitString(str, ',');
        x = std::stoi(coords[0]);
        y = std::stoi(coords[1]);
    }

    bool operator==(const Pos& other) const { return x == other.x && y == other.y; };
    bool operator==(const Pos& other) { return x == other.x && y == other.y; };
    bool operator!=(const Pos& other) { return !(*this==other); };
    bool operator!=(const Pos& other) const { return !(*this==other); };
    bool operator<(const Pos& other) { return x < other.x || (x == other.x && y < other.y); };
    void operator+=(const Pos& other) { x += other.x; y += other.y; };
    Pos operator+(const Pos& other) { return {x + other.x, y + other.y}; };
    Pos operator+(const Pos& other) const { return {x + other.x, y + other.y}; };
};
std::ostream& operator<< (std::ostream& os, Pos& pos)
{
    os << "(" << pos.x << "," << pos.y <<")";
    return os;
}

enum class Block
{
    Air, Blocked,
};
struct Grid
{
    std::vector<std::vector<Block>> m_Grid {}; //y 0 is top of grid
    int m_Width {};
    int m_Height {};

    void addPath(Pos start, Pos end)
    {
        int maxX = std::max(start.x, end.x);
        int maxY = std::max(start.y, end.y);
        
        if (maxY >= m_Height)
        {
            m_Height = maxY+1;
            m_Grid.resize(m_Height, std::vector<Block>(m_Width));
        }
        if (maxX >= m_Width)
        {
            m_Width = maxX+1;
            for (auto& v :  m_Grid)
                v.resize(m_Width);
        }

        Pos change;
        if (start.x==end.x)
            change = start.y > end.y ? Pos{0,-1} : Pos{0,1};
        else
            change = start.x > end.x ? Pos{-1,0} : Pos{1,0};

        for (auto pos {start}; pos!=end; pos+=change)
            (*this)[pos] = Block::Blocked;
        (*this)[end] = Block::Blocked;
    }

    void resizeX(const Pos& pos)
    {
        if (pos.x >= m_Width)
        {
            m_Width = pos.x+1;
            for (auto v=m_Grid.begin();v<m_Grid.end()-1;++v)
                (*v).resize(m_Width);
            m_Grid.back().resize(m_Width, Block::Blocked);
        }
    }

    Block& operator[](const Pos& pos) { return m_Grid[pos.y][pos.x]; };  
};

class Env
{
private: 
    Grid m_Grid {};
    Pos sandGenerator;

    void setupGrid()
    {
        std::ifstream inf {"input.txt"};
        std::string line{};
        
        while(inf, std::getline(inf, line))
        {
            auto positions = splitString(line, " -> ");

            for (auto pos=positions.begin()+1; pos<positions.end(); ++pos)
                m_Grid.addPath(*(pos-1), *(pos));
        }
        m_Grid.m_Grid.push_back(std::vector<Block>(m_Grid.m_Width));
        m_Grid.m_Grid.push_back(std::vector<Block>(m_Grid.m_Width, Block::Blocked));
    }

    bool simulateSand()
    {
        const static std::vector<Pos> moves {{0,1},{-1,1},{1,1}};
        Pos sand {sandGenerator};
        bool atRest {};
        while(!atRest)
        {
            atRest = true;
            for (const auto& move : moves)
            {
                auto newSand = sand+move;

                m_Grid.resizeX(newSand);

                if (m_Grid[newSand] == Block::Air)
                {
                    sand = newSand;
                    atRest = false;
                    break;
                }
            }
        }
        if (sand == sandGenerator)
            return false;

        m_Grid[sand] = Block::Blocked;
        return true;
    }

public:
    Env()
        : sandGenerator {500,0}
    {
        setupGrid();
    }


    int simulate()
    {
        int accumulator {};

        while(simulateSand()) 
            ++accumulator;
        return ++accumulator;
    }
};

int main()
{
    Env env{};
    auto num {env.simulate()};
    std::cout << "The number of sand that came to rest is: " << num << "\n";
}