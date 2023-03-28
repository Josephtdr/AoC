#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <numeric>

struct Pos
{
    int x {}; //col
    long y {}; //row
};
std::ostream& operator<< (std::ostream& os, Pos& pos)
{
    os << "(" << pos.x << "," << pos.y <<")";
    return os;
}

struct Rock
{
    Pos m_Pos{}; //position of the bottom left of the 'rectangle' encompassing the rock 
    int m_Width{};
    int m_Height{};

    //0,0 is m_BottomLeft position!
    //bit on if rock present in shape.
    std::vector<uint8_t> m_Grid{};

    uint8_t getAsChamberRow(int row, int width=7)
    {
        if (row<0 || row>=m_Height)
            std::cout <<"uh oh!!!!!!!!!!!!\n";

        return (m_Grid[row] << (width-(m_Pos.x+m_Width)));
    }
};

class Chamber
{
private:
    int m_Width{};
    long m_TowerHeight{};
    long m_SlicedRow{}; //tracks the row which is the 0th element of the chamber

    std::deque<uint8_t> m_Chamber{};
    std::string m_jetStream{};
    int m_jetStreamPtr{};
    const std::array<Rock, 5> m_Rocks{};
    int m_RockPtr{}; //tracks the next rock to be processed
    long m_RockCtr{}; //counts number of stationary rocks
    long m_MaxRocks{}; //continue until this number of rocks have been placed

    Rock m_CurrentRock {};

    bool tick()
    {
        char move = m_jetStream.at(m_jetStreamPtr++);
        m_jetStreamPtr %= m_jetStream.size();

        if (move=='<')
            moveHorizontal(-1);
        else if (move=='>')
            moveHorizontal(1);

        if (moveVerticle())
        {
            burnInRock();
            setNextRock();
            return true;
        }
        return false;
    }

    void burnInRock()
    {
        m_TowerHeight = std::max(m_TowerHeight, m_CurrentRock.m_Pos.y + m_CurrentRock.m_Height);

        for (int row{}; row<m_CurrentRock.m_Height; ++row)
        {
            getChamberRow(m_CurrentRock.m_Pos.y+row) |= m_CurrentRock.getAsChamberRow(row);

            if (getChamberRow(m_CurrentRock.m_Pos.y+row)==127)
            {
                auto old_sr = m_SlicedRow;
                m_SlicedRow = m_CurrentRock.m_Pos.y+row+1;
                m_Chamber.erase(m_Chamber.begin(), m_Chamber.begin()+(m_SlicedRow-old_sr));
            }
        }
        ++m_RockCtr;
    }

    void setNextRock()
    {
        m_CurrentRock = m_Rocks[m_RockPtr++];
        m_CurrentRock.m_Pos = {2, m_TowerHeight+3};
        
        m_RockPtr %= m_Rocks.size();
        int newSize = (m_TowerHeight-m_SlicedRow) + 3 + m_CurrentRock.m_Height;
        if (newSize > m_Chamber.size())
            m_Chamber.resize(newSize);
    }

    void moveHorizontal(int dir)
    {
        m_CurrentRock.m_Pos.x += dir;
        if (m_CurrentRock.m_Pos.x < 0 || m_CurrentRock.m_Pos.x + m_CurrentRock.m_Width > m_Width)
        {
            m_CurrentRock.m_Pos.x -= dir;
            return;
        }
        
        for (int row{}; row<m_CurrentRock.m_Height; ++row)
        {
            if ((getChamberRow(m_CurrentRock.m_Pos.y + row) & m_CurrentRock.getAsChamberRow(row)) > 0)
            {
                m_CurrentRock.m_Pos.x -= dir;
                return;
            }
        }
    }

    bool moveVerticle() //returns true if rock becomes stationary
    {
        long newBLy = m_CurrentRock.m_Pos.y - 1;
        if (newBLy < m_SlicedRow)
            return true;

        for (int row{}; row<m_CurrentRock.m_Height; ++row)
        {
            if ((getChamberRow(newBLy + row) & m_CurrentRock.getAsChamberRow(row)) > 0)
                return true;
        }
        
        m_CurrentRock.m_Pos.y -= 1;
        return false;
    }

    uint8_t& getChamberRow(long realRow)
    {
        return m_Chamber[realRow - m_SlicedRow];
    }

    std::vector<uint8_t> getTop(int size)
    {
        int top{0};
        while( *(m_Chamber.end()-top-1) == 0 )
            ++top;
        
        return {m_Chamber.end()-top-size, m_Chamber.end()-top};
    }

public:
    Chamber(long maxRocks)
        : m_Width{ 7 }, 
          m_Rocks{{
            {{}, 4,1, {15}},
            {{}, 3,3, {2,7,2}},
            {{}, 3,3, {7,1,1}},
            {{}, 1,4, {1,1,1,1}},
            {{}, 2,2, {3,3}}
          }},
          m_MaxRocks{ maxRocks }
    {
        std::ifstream inf {"input.txt"};
        std::getline(inf, m_jetStream);
        setNextRock();
    }

    void print()
    {
        int rowctr {m_Chamber.size()-1};
        for (auto row {m_Chamber.end()-1}; row >= m_Chamber.begin(); --row, --rowctr)
        {   
            for (int i{6}; i>=0; --i)
            {
                bool curStone{};

                if (m_CurrentRock.m_Pos.y <= rowctr && rowctr < m_CurrentRock.m_Pos.y+m_CurrentRock.m_Height)
                    curStone = (((m_CurrentRock.getAsChamberRow(rowctr-m_CurrentRock.m_Pos.y)) & (1<<i)) > 0);
                
                if (curStone)
                    std::cout << "@";
                else if (((*row) & (1<<i)) > 0)
                    std::cout << "#";
                else
                    std::cout << ".";
            }
            std::cout << +(*row) << "\n";
        }
        std::cout << "\n";
    }

    bool placeRock()
    {
        while(!tick());
        return m_RockCtr>=m_MaxRocks;
    }

    long simulateWCycle(int offset=200, int topSize=20)
    {
        for (int i{}; i<offset; ++i)
            placeRock();

        auto top {getTop(topSize)};
        int heightBefore {m_TowerHeight};
        int rocksBefore {m_RockCtr};
        std::vector<uint8_t> nextTop{};

        do{
            placeRock();
            nextTop = getTop(topSize);
        }while(top!=nextTop);

        auto rocksPlaced {m_RockCtr};
        auto height {m_TowerHeight};
        
        int heightDelta = height - heightBefore;
        heightBefore = height;
        int rockDelta = rocksPlaced - rocksBefore;

        while(rocksPlaced+rockDelta < m_MaxRocks)
        {
            rocksPlaced+=rockDelta;
            height+=heightDelta;
        }

        m_RockCtr = rocksPlaced;
        while(!placeRock());
        
        return height+(m_TowerHeight - heightBefore);
    }
};


int main()
{
    Chamber chamber{1000000000000};

    std::cout << "Height is " << chamber.simulateWCycle() << "\n";
}