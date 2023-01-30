#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>

#include "../aoclib.hpp"

struct Pos
{
    int x {}; //col
    int y {}; //row
};

struct Sensor
{
    Pos m_Pos{};
    Pos m_Beacon{};
    int m_ManhattenDist {};

    Sensor() = default;
    Sensor(Pos p1, Pos p2)
        : m_Pos {p1}, m_Beacon{p2}
    {
        m_ManhattenDist = std::abs(p1.x-p2.x)+std::abs(p1.y-p2.y);
    }
};

enum class Block
{
    Possible, Used, Impossible,
};
struct Blocks
{
    Block m_Type{};
    int start{};
    int end{};
};

class Grid
{
private:
    std::vector<std::vector<Blocks>> m_Grid {};
    std::vector<Sensor> m_Sensors{};

    int m_PositiveWidth {};
    int m_PositiveHeight {};
    int m_NegativeWidth {};
    int m_NegativeHeight {};

    void updateRow(std::vector<Blocks>& row, int start, int end, bool output=false)
    {
        int idxStart = std::find_if(row.begin(),row.end(),[start](const auto& b) {return b.end >= start;}) - row.begin();
        int idxEnd = std::find_if(row.begin(),row.end(),[end](const auto& b) {return b.end >= end;}) - row.begin();

        if (idxStart==idxEnd)
        {
            if (row[idxStart].m_Type==Block::Impossible) return;

            row.insert(row.begin()+idxStart+1, {Block::Impossible, start, end});
            row.insert(row.begin()+idxStart+2, {Block::Possible, end+1, row[idxStart].end});
            row[idxStart].end = start-1;
            return;
        }

        //erase middle blocks, will be overwritten
        if (idxEnd-1>idxStart)
        {
            row.erase(row.begin()+idxStart+1,row.begin()+idxEnd);
            idxEnd = idxStart+1;
        }

        if (row[idxStart].m_Type==Block::Possible)
        {
            row[idxStart].end = start-1;

            if (row[idxEnd].m_Type==Block::Impossible)
                row[idxEnd].start = start;
            else
            {
                row[idxEnd].start = end+1;
                row.insert(row.begin()+idxStart+1, {Block::Impossible,start,end});
            }
        }
        else
        {
            row[idxStart].end = end;

            if (row[idxEnd].m_Type==Block::Possible)
                row[idxEnd].start = end+1;
            else
            {
                row[idxStart].end = row[idxEnd].end;
                row.erase(row.begin() + idxEnd);
            }
        }
    }

    void insertBlock(std::vector<Blocks>& row, int pos, Block type)
    {
        int idxPos = std::find_if(row.begin(),row.end(),[pos](const auto& b) {return b.end >= pos;}) - row.begin();
        
        if (row[idxPos].m_Type==type) return;

        row.insert(row.begin()+idxPos+1, {type, pos, pos});
        row.insert(row.begin()+idxPos+2, {row[idxPos].m_Type, pos+1, row[idxPos].end});
        row[idxPos].end = pos-1;
    }

    void setup()
    {
        std::ifstream inf {"input.txt"};
        std::string line{};
        
        while(inf, std::getline(inf, line))
        {
            std::stringstream ss {line};
            std::vector<int> ints (4);
            for (auto& i : ints)
            {
                ss.ignore(1000, '=');
                ss >> i;
            }
            m_Sensors.emplace_back(Pos{ints[0],ints[1]},Pos{ints[2],ints[3]});
        }

        auto leastX = *(std::min_element(m_Sensors.begin(), m_Sensors.end(), 
            [](const auto& s1, const auto& s2)
            { return (s1.m_Pos.x-s1.m_ManhattenDist) < (s2.m_Pos.x-s2.m_ManhattenDist); }));
        auto leastY = *(std::min_element(m_Sensors.begin(), m_Sensors.end(), 
            [](const auto& s1, const auto& s2)
            { return (s1.m_Pos.y-s1.m_ManhattenDist) < (s2.m_Pos.y-s2.m_ManhattenDist); }));
        auto mostX = *(std::max_element(m_Sensors.begin(), m_Sensors.end(), 
            [](const auto& s1, const auto& s2)
            { return (s1.m_Pos.x+s1.m_ManhattenDist) < (s2.m_Pos.x+s2.m_ManhattenDist); }));
        auto mostY = *(std::max_element(m_Sensors.begin(), m_Sensors.end(), 
            [](const auto& s1, const auto& s2)
            { return (s1.m_Pos.y+s1.m_ManhattenDist) < (s2.m_Pos.y+s2.m_ManhattenDist); }));
        
        m_NegativeWidth = std::abs(leastX.m_Pos.x-leastX.m_ManhattenDist);
        m_NegativeHeight = std::abs(leastY.m_Pos.y-leastY.m_ManhattenDist);
        m_PositiveWidth = mostX.m_Pos.x+mostX.m_ManhattenDist;
        m_PositiveHeight = mostY.m_Pos.y+mostY.m_ManhattenDist;

        m_Grid.resize(m_NegativeHeight + m_PositiveHeight + 1,
                      std::vector<Blocks>{{Block::Possible,-m_NegativeWidth,m_PositiveWidth}});

        for (auto& s : m_Sensors)
        {
            for (int rowidx{-s.m_ManhattenDist}; rowidx<=s.m_ManhattenDist; ++rowidx)
            {
                int colidx {s.m_ManhattenDist-std::abs(rowidx)};
                int row = m_NegativeHeight + (s.m_Pos.y+rowidx);
                int colStart = (s.m_Pos.x-colidx);
                int colEnd = (s.m_Pos.x+colidx);

                updateRow(m_Grid[row], colStart, colEnd, false);
            }
        }

        for (auto& s : m_Sensors)
        {
            insertBlock(m_Grid[m_NegativeHeight+s.m_Pos.y], s.m_Pos.x, Block::Used);
            insertBlock(m_Grid[m_NegativeHeight+s.m_Beacon.y], s.m_Beacon.x, Block::Used);
        }
    }

public:
    Grid()
    { 
        setup();
    }

    long getTuningFreq(int bound=4000000)
    {
        for (auto r=m_Grid.begin()+m_NegativeHeight;
                r<m_Grid.begin()+m_NegativeHeight+bound; ++r)
        {
            auto block = std::find_if((*r).begin(), (*r).end(), [bound](const auto& b)
            {
                return (b.end >= 0 && b.start <= bound && b.m_Type==Block::Possible);
            });
            if (block==std::end(*r)) continue;
            
            return ((*block).start * 4000000l) + ((r-m_Grid.begin())-m_NegativeHeight);
        }
        return -1;
    }

    int numBlocked(int row)
    {   
        int num{};
        for (auto& b : m_Grid[m_NegativeHeight+row])
        {
            if (b.m_Type == Block::Impossible)
                num += (b.end-b.start)+1;
        }
        return num;
    }
};

int main()
{
    Grid grid {};
    std::cout << "the number of blocked positions is " << grid.numBlocked(2000000) << "\n";
    std::cout << "tunfreq is " << grid.getTuningFreq() << "\n";
}