#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <optional>
#include <variant>

#include "../aoclib.hpp"

struct List_t;
std::ostream& operator<< (std::ostream& os, const List_t& list);

struct List_t
{
    std::vector<std::variant<List_t, int>> members {};

    List_t() = default;
    List_t(std::variant<List_t, int> variant)
    {
        if (const int* val = std::get_if<int>(&variant))
            members.push_back(*val);
        else if (const List_t* val = std::get_if<List_t>(&variant))
            members = val->members;
    }

    bool operator==(const List_t& other)
    {
        // std::cout << "Comparing " << *this << " == " << other <<"\n"; 
        if (members.size() != other.members.size())
           return false;

        for (int i{}; i<members.size(); ++i)
        {
            const auto& leftVariant = members[i];
            const auto& rightVariant = other.members[i]; 

            if (std::holds_alternative<int>(leftVariant) &&
                std::holds_alternative<int>(rightVariant))
            { // both sides ints
                if (leftVariant == rightVariant)
                    continue;
                return false;
            }

            List_t leftList {leftVariant};
            List_t rightList {rightVariant};

            if (leftList==rightList)
                continue;
            return false;
        }
        return true;
    }

    bool operator<(const List_t& other)
    {
        int thisSize = members.size();
        int otherSize = other.members.size();

        for (int i{}; i<std::max(thisSize,otherSize); ++i)
        {
            if (i >= thisSize && i < otherSize)
                return true; //left ran out first
            if (i >= otherSize && i < thisSize)
                return false; //right ran out first
            
            const auto& leftVariant = members[i];
            const auto& rightVariant = other.members[i]; 

            if (std::holds_alternative<int>(leftVariant) &&
                std::holds_alternative<int>(rightVariant))
            { // both sides ints
                // std::cout << "Comparing " << std::get<int>(leftVariant) << " and " << std::get<int>(rightVariant) <<"\n";
                if (leftVariant == rightVariant)
                    continue;
                return  leftVariant < rightVariant;
            }

            List_t leftList {leftVariant};
            List_t rightList {rightVariant};
            // std::cout << "Comparing " << leftList << " and " << rightList <<"\n"; 

            if (leftList==rightList)
                continue;
            return leftList < rightList;
        }
        return false;        
    };

    bool operator==(const List_t& other) const { return *this == other; }
    bool operator<(const List_t& other) const { return *this < other; }
};
std::ostream& operator<< (std::ostream& os, const List_t& list)
{
    os << '[';
    // for (const auto& member : list.members)
    for (int i{}; i<list.members.size(); ++i)
    {
        const auto& member = list.members[i]; 

        if (const int* val = std::get_if<int>(&member))
            os << *val;
        else if (const List_t* val = std::get_if<List_t>(&member))
            os << *val;

        if (i<list.members.size()-1)
            os << ",";
    }
    os << ']';

    return os;
}

List_t readInList(std::stringstream& ss)
{
    List_t packet {};
    ss.ignore(1);

    while(ss.good())
    {
        char nextChar = ss.peek();

        if (nextChar=='[')
            packet.members.push_back(readInList(ss));
        else if (nextChar == ',')
            ss.ignore(1);
        else if (nextChar == ']')
        {
            ss.ignore(1);
            return packet;
        }
        else
        {
            int value{};
            ss >> value;
            packet.members.push_back(value);
        }
    }
    return packet;
}

int main()
{
    std::ifstream inf {"input.txt"};
    std::string line {};

    std::vector<List_t> packets {};

    while(inf)
    {
        std::getline(inf, line);
        std::stringstream ssl(line);
        packets.push_back(readInList(ssl));
        std::getline(inf, line);
        std::stringstream ssl2(line);
        packets.push_back(readInList(ssl2));
        std::getline(inf, line);
    }

    std::string div1 = "[[2]]";
    std::stringstream ssdiv1(div1);
    List_t packetDiv1 = readInList(ssdiv1);
    packets.push_back(packetDiv1);
    std::string div2 = "[[6]]";
    std::stringstream ssdiv2(div2);
    List_t packetDiv2 = readInList(ssdiv2);
    packets.push_back(packetDiv2);

    std::sort(packets.begin(), packets.end());

    auto it1 = find(packets.begin(), packets.end(), packetDiv1);
    auto it2 = find(packets.begin(), packets.end(), packetDiv2);
    
    int indexSum = (it1-packets.begin() + 1) * (it2 - packets.begin() + 1) ;

    std::cout << indexSum << " is the idxs!\n";
}
