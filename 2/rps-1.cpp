#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


enum class RPS
{
    rock,
    paper,
    scisors,
};
int calculateResult(RPS p1, RPS p2) //p1 is us, p2 is opponent
{
    int score {static_cast<int>(p1) + 1};

    if (p1 == p2)
        return score + 3;

    switch(p1)
    {
        case RPS::rock: 
            score += p2==RPS::scisors ? 6 : 0; break;
        case RPS::paper: 
            score += p2==RPS::rock ? 6 : 0; break;
        case RPS::scisors: 
            score += p2==RPS::paper ? 6 : 0; break;
    }
    return score;
}

std::unordered_map<char,RPS> const conversion 
{
    {'A',RPS::rock}, {'B',RPS::paper}, {'C',RPS::scisors},
    {'X',RPS::rock}, {'Y',RPS::paper}, {'Z',RPS::scisors}
};

int main()
{
    std::ifstream inf {"input.txt"};

    int score {};
    std::string input {};

    while(inf && std::getline(inf, input))
    {
        auto player1 = input[input.length() - 1];
        auto player2 = input[0];

        score += calculateResult(conversion.at(player1), conversion.at(player2));
    }

    std::cout << "Your total score is " << score << "!\n";
}