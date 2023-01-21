#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoclib.hpp"


struct File
{
    std::string m_Name {};
    int m_Size {};
};

struct Directory
{
    std::string m_Name {};
    std::vector<File> m_FileList {};
    std::vector<Directory> m_DirectoryList {};
    Directory* m_ParentDirectory;
    
    Directory(Directory* parent, std::string_view name)
        : m_ParentDirectory {parent}, m_Name {name}
    {}

    Directory* getSubDir(std::string_view name)
    {
        for (auto& subdir : m_DirectoryList)
        {
            if (subdir.m_Name == name)
                return &subdir;
        }
        return nullptr;
    }

    int getTotalSize()
    {
        int size {};
        for (auto& f : m_FileList)
            size += f.m_Size;
        
        for (auto& d : m_DirectoryList)
            size += d.getTotalSize();

        return size;
    }
};

int main()
{
    std::ifstream inf {"input.txt"};

    Directory headDir {nullptr, "/"};
    Directory* dirPtr {&headDir};
    std::vector<std::reference_wrapper<Directory>> dirList {};

    std::string line {};
    while(inf && std::getline(inf, line))
    {
        auto args {splitString(line, ' ')};

        if (args[1]=="cd")
        {
            if (args[2] == "/")
                dirPtr = &headDir;
            else if (args[2] == "..")
            {
                if (dirPtr->m_ParentDirectory)
                    dirPtr = dirPtr->m_ParentDirectory;
            }
            else
                dirPtr = dirPtr->getSubDir(args[2]);
        }
        else if (args[1]=="ls")
        {
            while(inf && inf.peek()!='$' && std::getline(inf, line))
            {
                args = splitString(line, ' ');

                if (args[0]=="dir")
                    dirPtr->m_DirectoryList.push_back({dirPtr, args[1]});    
                else
                    dirPtr->m_FileList.push_back({args[1],std::stoi(args[0])});
            }
            for (auto& dir : dirPtr->m_DirectoryList)
                dirList.push_back(dir);
        }
    }

    const int maxSize {70000000};
    int currentSize {headDir.getTotalSize()};
    int idealDirSize {currentSize};

    for (auto& dir : dirList)
    {
        if (int size {dir.get().getTotalSize()}; currentSize - size <= 40000000)
        {
            idealDirSize = std::min(idealDirSize, size);
        }
    }
    std::cout << "smallest such dir is of size " << idealDirSize << "\n";

    int sum {};
    for (auto& dir : dirList)
        if (int size {dir.get().getTotalSize()}; size <= 100000 ) sum += size;
    std::cout << "total size of small dirs is " << sum << "\n";
}