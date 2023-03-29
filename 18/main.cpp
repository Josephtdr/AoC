#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <optional>
#include <variant>
#include <cassert> 
#include <numeric>

#include "../aoclib.hpp"


struct Pos
{
    int x;
    int y;
    int z;

    Pos(int _x, int _y, int _z) : x{_x},y{_y},z{_z} {}
    Pos(std::vector<std::string> input)
        : x {stoi(input[0])}, y {stoi(input[1])}, z {stoi(input[2])}
    {}

    bool operator==(const Pos& other) const {return x==other.x && y==other.y && z==other.z; }
    void operator+=(const Pos& other) { x+=other.x; y+=other.y; z+=other.z; }
    Pos operator+(const Pos& other) const { return {x+other.x, y+other.y, z+other.z}; }
    Pos operator+(const Pos& other) { return {x+other.x, y+other.y, z+other.z}; }
};
std::ostream& operator<< (std::ostream& os, const Pos& pos)
{
    os << "(" << pos.x << "," << pos.y << "," << pos.z << ")";
    return os;
}


class Grid
{
    std::vector<bool> m_Grid{};
    std::vector<Pos> m_Positions{};

    int m_Width {}; // x size
    int m_xMin {};
    int m_Height {}; // y size
    int m_yMin {};
    int m_Depth {}; // z size?
    int m_zMin {};

    void setup()
    {
        std::ifstream inf {"input.txt"};
        std::string line {};

        while(inf && std::getline(inf, line))
            m_Positions.push_back({splitString(line, ',')});

        auto max_x = std::max_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.x < b.x; }
                                     );
        m_Width = (*max_x).x + 1;
        auto min_x = std::min_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.x < b.x; }
                                     );
        m_xMin = (*min_x).x - 1;

        auto max_y = std::max_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.y < b.y; }
                                     );
        m_Height = (*max_y).y + 1;
        auto min_y = std::min_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.y < b.y; }
                                     );
        m_yMin = (*min_y).y - 1;

        auto max_z = std::max_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.z < b.z; }
                                     );
        m_Depth = (*max_z).z + 1;
        auto min_z = std::min_element( m_Positions.begin(), m_Positions.end(),
                                     [] (const Pos& a, const Pos& b) { return a.z < b.z; }
                                     );
        m_zMin = (*min_z).z - 1;

        m_Grid.resize((m_Depth)*(m_Height)*(m_Width), false);

        for (const auto& pos : m_Positions)
            setGrid(pos, true);
    }

    void setGrid(Pos pos, bool value)
    {
        m_Grid[pos.z*m_Width*m_Height + pos.y*m_Width + pos.x] = value;
    }

    void findOuterEdges(Pos pos, std::vector<Pos>& visited, int& edges)
    {
        visited.push_back(pos);

        static const std::vector<Pos> posMoves {
            {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1} 
        }; 

        //too far from lava
        if (pos.x < m_xMin || pos.y < m_yMin || pos.z < m_zMin ||
                pos.x > m_Width || pos.y > m_Height || pos.z > m_Depth)
            return;

        for (const auto& move : posMoves)
        {
            Pos newPos {pos+move};

            if ((*this)(newPos)) // we encouter lava
            {
                ++edges;
                continue;
            }
            if (std::find(visited.begin(), visited.end(), newPos) != visited.end())
                continue;

            findOuterEdges(newPos, visited, edges);
        }
    }

public:
    Grid()
    { setup(); }

    int analyise() //part1
    {
        std::vector<Pos> moves {
            {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1} 
        };

        int sides{};
        for (const auto& pos : m_Positions)
        {
            for (const auto& move : moves) //deals with each 'edge'
            {
                auto conPos {pos+move};
                if ((*this)(conPos)) //if its a block ignore
                    continue;

                ++sides;
            }
        }
        return sides;
    }

    int outerAnalyise() //part 2
    {
        Pos pos {m_xMin, m_yMin, m_zMin };
        std::vector<Pos> visited {};
        int sides {};

        findOuterEdges(pos, visited, sides);

        return sides;
    }
    
    bool operator() (Pos pos) const { 
        if (pos.x <= m_xMin || pos.y <= m_yMin || pos.z <= m_zMin || pos.x>=m_Width || pos.y>=m_Height || pos.z>=m_Depth)
            return  false;

        return m_Grid[pos.z*m_Width*m_Height + pos.y*m_Width + pos.x]; 
    }
};

int main()
{
    Grid grid {};

    std::cout << grid.outerAnalyise() << "\n";
}