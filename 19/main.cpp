#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cassert> 
#include <numeric>

#include "../aoclib.hpp"


enum Resource
{
    Ore, Clay, Obsidian, Geode, Num_Resouces
};
Resource strToResource(std::string_view str)
{
    if (str=="ore")
        return Ore;
    else if (str=="clay")
        return Clay;
    else if (str=="obsidian")
        return Obsidian;
    else if (str=="geode")
        return Geode; 
}

//an array in which each of the 'Num_Resouces' indexes indicate their respective resource
template<typename T>
struct RobotArray
{
    std::array<T, Num_Resouces> m_Data{};

    T& operator[](int i) { return m_Data[i]; }
    //true when all elements in left are less than respective in right
    bool operator<(const RobotArray& other) { 
        for (int i{}; i<Num_Resouces;  ++i)
        { if (m_Data[i] > other.m_Data[i]) return false; }
        return true;
    }
    bool operator==(const RobotArray& other) const { 
        return m_Data==other.m_Data;
    }
    void operator+=(const RobotArray& other) {
        for (int i{}; i<Num_Resouces;  ++i)
            m_Data[i] += other.m_Data[i];
    }
    void operator-=(const RobotArray& other) {
        for (int i{}; i<Num_Resouces;  ++i)
            m_Data[i] -= other.m_Data[i];
    }
};
template<typename T>
std::ostream& operator<< (std::ostream& os, RobotArray<T>& rA)
{
    os << '[';
    for (int i{}; i<Num_Resouces;  ++i)
        os << i << ":" << rA.m_Data[i] << ",";
    os << ']';
    return os;
}

struct GameState
{
    RobotArray<int> m_Robots {};
    RobotArray<int> m_Resources {};
    int m_TimeRemaining {};
    
    void reset()
    {
        m_Robots = {{1,0,0,0}};
        m_TimeRemaining = 32;
    }
    void tick()
    {
        m_Resources += m_Robots;
        --m_TimeRemaining;
    }
};
struct Action
{
    int waitingTime {};
    Resource botToBuild {};
};

class Operation
{
private:
    RobotArray<RobotArray<int>> m_Blueprint {};
    RobotArray<int> m_MaxRequired {};
    int m_ID {};
    GameState m_State {};
    
    void setup(std::string str)
    {
        auto getid = splitString(str, ':');
        m_ID = stoi(splitString(getid[0], ' ')[1]);

        auto robotStrs = splitString(getid[1], '.');
        for (int i{}; i<Num_Resouces;  ++i)
        {
            auto words = splitString(robotStrs[i],' ');
            for (int j{5}; j<words.size(); j+=3 )
            {
                m_Blueprint[i][strToResource(words[j+1])] = stoi(words[j]); 
            }
        }

        for (int i{}; i<Num_Resouces;  ++i)
        {
            auto maxElem = std::max_element(m_Blueprint.m_Data.begin(), m_Blueprint.m_Data.end(), 
                [i] (auto& a, auto& b) { return a[i] < b[i]; }
                );
            m_MaxRequired[i] = (*maxElem)[i];
        }

        m_State.reset();
    }

    std::vector<Action> getPossibleActions(GameState state)
    {
        std::vector<Action> actions {};

        for (int i{}; i<Num_Resouces;  ++i)
        {
            //check if we need more of given robot
            if (i!=Geode && ((state.m_Robots[i]*state.m_TimeRemaining)+state.m_Resources[i] >=
                m_MaxRequired[i]*state.m_TimeRemaining))
                continue;

            auto requiredRes = m_Blueprint[i];
            //check robot is creatable
            bool creatable {true};
            for (int j{}; j<Num_Resouces;  ++j)
            {
                if (requiredRes[j]>0 && state.m_Robots[j]<=0)
                    creatable = false;
            }
            if (!creatable) continue; 

            //see how long will be needed to wait to aquire necessary resources
            int waitTime {1};
            while(!(requiredRes < state.m_Resources) && (state.m_TimeRemaining-waitTime)>=0)
            {
                waitTime++; requiredRes -= state.m_Robots;
            }
            actions.push_back({waitTime, static_cast<Resource>(i)});
        }

        return actions;
    }
    
    int exhaustiveSearch(GameState state)
    {
        if (state.m_TimeRemaining<=0)
            return state.m_Resources[Geode];

        int maxGeodes {};

        for (auto action : getPossibleActions(state))
        {
            auto newState = state;
            while(action.waitingTime > 0 && newState.m_TimeRemaining > 0)
            {
                --action.waitingTime;
                newState.tick();
            }

            if (action.waitingTime == 0 && newState.m_TimeRemaining > 0)
            {
                newState.m_Resources -= m_Blueprint[action.botToBuild];
                newState.m_Robots[action.botToBuild]++;
            }
            maxGeodes = std::max(maxGeodes, exhaustiveSearch(newState));
        }
        return maxGeodes;     
    }

public:
    Operation(std::string blueprintStr)
    {
        setup(blueprintStr);
    }

    int getMaxGeodes() { return exhaustiveSearch(m_State); }
    int getID () { return m_ID; }
};

int main()
{
    std::ifstream inf {"input.txt"};
    std::string line {};

    int quality {1};
    int num {};
    while(inf && std::getline(inf, line) && num < 3)
    {
        Operation op {line};
        quality *= op.getMaxGeodes();
        num++;
    }

    std::cout << "\n" << quality << "\n";
}