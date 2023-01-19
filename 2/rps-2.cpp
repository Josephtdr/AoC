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
enum class Outcome
{
    lose,
    draw,
    win,
};
int calculateResult(RPS p2, Outcome outcome) //p1 is us, p2 is opponent
{
    int score {static_cast<int>(outcome) * 3}; //win draw lose score

    if (outcome == Outcome::draw) //peice is same as opponent
        return score + (static_cast<int>(p2) + 1);

    switch(p2)
    {
        case RPS::rock: 
            score += outcome==Outcome::win ? 2 : 3; break;
        case RPS::paper: 
            score += outcome==Outcome::win ? 3 : 1; break;
        case RPS::scisors: 
            score += outcome==Outcome::win ? 1 : 2; break;
    }
    return score;
}

std::unordered_map<char,RPS> const conversion1 
{
    {'A',RPS::rock}, {'B',RPS::paper}, {'C',RPS::scisors}
};
std::unordered_map<char,Outcome> const conversion2
{
    {'X',Outcome::lose}, {'Y',Outcome::draw}, {'Z',Outcome::win}
};

int main()
{
    std::ifstream inf {"input.txt"};

    int score {};
    std::string input {};

    while(inf && std::getline(inf, input))
    {
        auto player1 = input[input.length() - 1]; //opponent
        auto player2 = input[0]; //me

        score += calculateResult(conversion1.at(player2), conversion2.at(player1));
    }

    std::cout << "Your total score is " << score << "!\n";
}