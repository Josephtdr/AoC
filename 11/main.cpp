#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <functional>
#include <vector>
#include <numeric>

#include "../aoclib.hpp"

template
<typename T>
struct Monkey
{
    int m_ID{};
    int m_NumInspections{};
    std::vector<T> m_HeldItems{};
    std::function<T(T,T)> m_Operation {};
    int m_OpTerm1 {};
    int m_OpTerm2 {};
    int m_DivisionTest {};
    int m_MonkeyTrue {};
    int m_MonkeyFalse {};

    T inspectItem(T& item)
    {
        ++m_NumInspections;

        item = m_Operation(item, m_OpTerm2 ? m_OpTerm2 : item);
        // item /= 3;

        if ((item%m_DivisionTest)==0)
            return m_MonkeyTrue;
        else 
            return m_MonkeyFalse;
    }
};

template
<typename T>
std::vector<Monkey<T>> generateMonkeys()
{
    std::ifstream inf {"input.txt"};
    std::vector<Monkey<T>> monkeys {};

    while(inf)
    {   
        monkeys.push_back({});
        auto& monkey = monkeys.back();

        std::string line{};

        inf.ignore(1000, 'y');
        inf >> monkey.m_ID;
        inf.ignore(1);
        
        inf.ignore(1000, ':');
        std::getline(inf, line);
        auto stritems {splitString(line, ',')};

        for (auto& s : stritems)
        {
            if (s.empty()) continue;
            monkey.m_HeldItems.push_back(std::stoi(s.substr(1,2)));
        }

        inf.ignore(1000, 'd');
        char chOperation {};
        inf >> chOperation; 
        std::function<T(T,T)> add = std::plus<T>();
        std::function<T(T,T)> mult = std::multiplies<T>();
        monkey.m_Operation = (chOperation=='+') ? add : mult;

        inf.ignore(1);
        std::getline(inf, line);
        if (line!="old")
            monkey.m_OpTerm2 = std::stoi(line);

        inf.ignore(1000,'y');
        inf >> monkey.m_DivisionTest;

        inf.ignore(1000,'y');
        inf >> monkey.m_MonkeyTrue;

        inf.ignore(1000,'y');
        inf >> monkey.m_MonkeyFalse;

        std::getline(inf, line);
        std::getline(inf, line);
    }
    return monkeys;
}

int LcmOfArray(std::vector<int> arr, int idx){
    if (idx == arr.size()-1)
        return arr[idx];

    int a {arr[idx]};
    int b {LcmOfArray(arr, idx+1)};
    return (a*b/std::gcd(a,b));
}


int main()
{
    using m_t = long;
    auto monkeys {generateMonkeys<m_t>()};
    
    std::vector<int> divisors{};
    for (const auto& m : monkeys)
        divisors.push_back(m.m_DivisionTest);
    m_t lcm = LcmOfArray(divisors,0);

    for (int round{}; round < 10000; ++round)
    {
        for (auto& m : monkeys)
        {
            for (auto& i : m.m_HeldItems)
            {
                auto newMonkey = m.inspectItem(i);
                monkeys[newMonkey].m_HeldItems.push_back(i % lcm);
            }
            m.m_HeldItems.clear();
        }
    }

    std::vector<m_t> scores {};
    for (const auto& m : monkeys)
        scores.push_back(m.m_NumInspections);
    std::sort(scores.begin(), scores.end());

    m_t monkeyBuisness { static_cast<m_t>(scores.back()) * *(scores.end()-2) };
    std::cout << "The calculated monkey buisness is " << monkeyBuisness << "!\n";
}