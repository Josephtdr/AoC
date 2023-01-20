#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <deque>
#include <bits/stdc++.h>

bool allUnique(std::deque<char>& dque, int size)
{
    std::unordered_set<char> s (dque.begin(), dque.end());
    return s.size()==size;
}

int main()
{
    std::ifstream inf {"input.txt"};

    std::string buffer {};
    std::getline(inf, buffer);
    std::stringstream ss {buffer};
    
    std::deque<char> dque {};

    int dryStart {14};
    for (int i{}; i < dryStart; ++i)
        dque.push_back(ss.get());
    
    int charsRead {dryStart};
    while(!allUnique(dque, dryStart))
    {
        dque.pop_front();
        dque.push_back(ss.get());
        ++charsRead;
    }
    
    std::cout << "Starts after " << charsRead <<" characters have been read\n";
}