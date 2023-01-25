#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <set>
#include <chrono>
#include <cmath>

#include "../aoclib.hpp"

struct Pos
{
    int x {}; //col
    int y {}; //row

    bool operator==(const Pos& other) const { return x == other.x && y == other.y; };
    bool operator==(const Pos& other) { return x == other.x && y == other.y; };
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

struct Node
{
    Pos m_Pos {};
    int huristic{};

    bool operator==(const Node& other) { return m_Pos == other.m_Pos; };
    bool operator<(const Node& other) { return huristic < other.huristic; };
    bool operator<(const Node& other) const { return huristic < other.huristic; };
    bool operator>(const Node& other) { return huristic > other.huristic; };
    bool operator>(const Node& other) const { return huristic > other.huristic; };
};

struct Grid
{
    std::vector<std::string> m_Grid {};
    int m_Width {};
    int m_Height {};

    Grid(std::vector<std::string> grid)
        : m_Grid {grid}, m_Height{static_cast<int>(grid.size())}, m_Width {static_cast<int>(grid[0].size())}
    {}

    char& operator[](const Pos& pos) { return m_Grid[pos.y][pos.x]; };
};
std::vector<std::string> setupGrid()
{
    std::ifstream inf {"input.txt"};
    std::istream_iterator<std::string> input(inf);

    std::vector<std::string> strGrid {};
    std::copy(input, std::istream_iterator<std::string>(),    
          std::back_inserter(strGrid));

    return strGrid;
}



class AStarSearch
{
private:
    Grid m_Grid;
    Pos m_Head {};
    Pos m_Goal {};

    void setup()
    {
        //find initial state
        for (int row{}; row<m_Grid.m_Grid.size(); ++row)
        {
            auto col {m_Grid.m_Grid[row].find('S')};
            if (col!=std::string::npos)
            {
                m_Head = {static_cast<int>(col),row};
                m_Grid[m_Head] = 'a';
            }
            col = m_Grid.m_Grid[row].find('E');
            if (col!=std::string::npos)
            {
                m_Goal = {static_cast<int>(col),row};
            }
        }
    }

    int getHuristic(Pos node)
    {
        return std::abs(node.x-m_Goal.x) + std::abs(node.y-m_Goal.y);
    }

    int encodeNode(Pos node) { return node.x * m_Grid.m_Height + node.y; }

    std::vector<Pos> getNeighbours(Pos node)
    {
        static const std::vector<Pos> possibleActions = {
            {0,1},{0,-1},{1,0},{-1,0}
        };

        std::vector<Pos> neighbours {};
        for (const auto& action : possibleActions)
        {
            Pos newPos {node + action};

            if (newPos.x < 0 || newPos.x >= m_Grid.m_Width ||
                newPos.y < 0 || newPos.y >= m_Grid.m_Height)
                continue;

            if (m_Grid[newPos]=='E')
            {
                if (m_Grid[node]=='z')
                    neighbours.push_back(newPos);
                else
                    continue;
            }
                
            if (m_Grid[newPos] <= m_Grid[node] + 1)
                neighbours.push_back(newPos);
            
        }
        return neighbours;
    }

public:
    AStarSearch()
        : m_Grid {setupGrid()}
    {
        setup();
    }

    int search(Pos start)
    {
        std::priority_queue<Node,std::vector<Node>,std::greater<Node>> queue {};

        std::set<int> queueList {};

        queue.push({start,1});
        queueList.insert(encodeNode(start));


        std::unordered_map<int, int> rawDistance {};
        std::unordered_map<int, int> score {};
        rawDistance[encodeNode(start)] = 0;
        score[encodeNode(start)] = getHuristic(start);


        while(!queue.empty())
        {
            auto [current, h] = queue.top();
            auto curEnc = encodeNode(current);

            if (current == m_Goal)
                return rawDistance.at(curEnc);

            queue.pop();
            queueList.erase(curEnc);

            for (auto& n : getNeighbours(current))
            {
                int tentDist = rawDistance.at(curEnc) + 1;
                auto neighbourEnc = encodeNode(n);

                if (rawDistance.find(neighbourEnc)==rawDistance.end() || tentDist < rawDistance.at(neighbourEnc))
                {
                    rawDistance[neighbourEnc] = tentDist;
                    score[neighbourEnc] = tentDist + getHuristic(n);

                    if (queueList.find(neighbourEnc)==queueList.end())
                    {
                        queueList.insert(neighbourEnc);
                        queue.push({n, score.at(neighbourEnc)});
                    }
                }
            }
        }
        return INT32_MAX;
    }

    int getBestA()
    {
        int leastDistFromA {INT32_MAX};

        for (int row{}; row<m_Grid.m_Grid.size(); ++row)
        {
           for (int col{}; col<m_Grid.m_Grid[0].size(); ++col)
            {
                Pos pos {col,row};

                if (m_Grid[pos] == 'a')
                    leastDistFromA = std::min(
                        leastDistFromA, search(pos)
                    );
            }
        }
        return leastDistFromA;
    }
};

int main()
{
    AStarSearch astar {};

    auto start = std::chrono::high_resolution_clock::now();
    int result = astar.getBestA();
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "maybe: "<< result << " in "<< duration.count() << " milliseconds\n";
    
}