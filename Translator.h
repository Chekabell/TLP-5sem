#pragma once
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class Translator
{
public:
    Translator();
    ~Translator();
    
    void StartParse();
    
private:
    std::vector<std::pair<std::string, int>> symTable;
    std::ifstream _inputFile;
    char curr;
    long _numberOfTriads;
    
    void GetChar();
    bool IsDigit(char value);
    bool IsSpace(char value);
    bool IsAlpha(char value);
    void Error (std::string msg);
    void AddOrReplace(const std::pair<std::string, int>& value);
    
    int ProcS();
    int ProcE();
    int ProcT();
    int ProcI();
    int ProcR();
    
};
