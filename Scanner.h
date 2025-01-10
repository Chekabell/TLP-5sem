#pragma once
#include <fstream>
#include <iostream>
#include "Token.h"

class Scanner
{
    char currentChar;
    std::fstream _inputFile;
public:
    Scanner(const std::string& fileName = "input.txt");
    ~Scanner();
    Token GetToken();
    void Error (std::string msg);
private:
    void NextChar();
    bool IsDigit(char value);
    bool IsSpace(char value);
    bool IsAlpha(char value);
    

    std::string ProcI();
    std::string ProcC();
    
};
