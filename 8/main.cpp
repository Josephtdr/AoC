#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> setupGrid()
{
    std::ifstream inf {"input.txt"};
    std::istream_iterator<std::string> input(inf);

    std::vector<std::string> strGrid {};
    
    std::copy(input, std::istream_iterator<std::string>(),    
          std::back_inserter(strGrid));

    std::vector<std::vector<int>> grid {};
    for (auto& l : strGrid)
    {
        std::vector<int> v{};
        for (int j{}; j<l.length(); ++j)
            v.push_back(l[j]-'0');
        
        grid.push_back(v);
    }
    
    return grid;
}

bool treeVisible(const std::vector<std::vector<int>>& grid, int col, int row)
{
    auto maxCol {grid[0].size()};
    auto maxRow {grid.size()};
    auto treeHeight {grid[row][col]};

    //if on an edge, trivially true
    if (col==0 || col==maxCol-1 || row==0 || row==maxRow-1)
        return true;

    auto colLambda = [treeHeight] (int t) {return treeHeight > t;};

    if (std::all_of(grid[row].cbegin()+col+1,grid[row].cend(), colLambda))
        return true;
    if (std::all_of(grid[row].crbegin()+(maxCol-col),grid[row].crend(), colLambda))
        return true;
    
    auto rowLambda = [treeHeight,col] (const auto& v) {return treeHeight > v[col];};
    
    if (std::all_of(grid.cbegin()+row+1,grid.cend(),rowLambda))
        return true;
    if (std::all_of(grid.crbegin()+(maxRow-row),grid.crend(),rowLambda))
        return true;

    return false;
}

template
<typename Iterator, typename Condition>
const int count_until(Iterator first, const Iterator last, Condition&& condition)
{
    int num{};
    for (; first != last; ++first)
    {
        ++num;
        if (condition(*first)) break;
    }
    return num;
}
int getScenicScore(const std::vector<std::vector<int>>& grid, int col, int row)
{
    auto maxCol {grid[0].size()};
    auto maxRow {grid.size()};
    auto treeHeight {grid[row][col]};

    //if on an edge, trivially 0
    if (col==0 || col==maxCol-1 || row==0 || row==maxRow-1)
        return 0;

    auto colLambda = [treeHeight] (int t) {return treeHeight <= t;};
    
    int right = count_until(grid[row].cbegin()+col+1,grid[row].cend(),colLambda);
    int left = count_until(grid[row].crbegin()+(maxCol-col),grid[row].crend(),colLambda);
    
    auto rowLambda = [treeHeight,col] (const auto& v) {return treeHeight <= v[col];};
    
    int up = count_until(grid.cbegin()+row+1,grid.cend(),rowLambda);
    int down = count_until(grid.crbegin()+(maxRow-row),grid.crend(),rowLambda);
    
    return right*left*up*down;
}

int main()
{
    auto grid {setupGrid()};
    int maxScore {};
    // int num{};

    for (int col{}; col<grid[0].size(); ++col)
    {
        for (int row{}; row<grid.size(); ++row)
        {
            maxScore = std::max(maxScore, getScenicScore(grid, col, row));
            // if (treeVisible(grid, col, row)) ++num; 
        }
    }
    std::cout << "The maximum sceneic score is " << maxScore << "\n";
    // std::cout << "The number of visible trees is " << num << "\n";
}