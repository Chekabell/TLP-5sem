#pragma once
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class Triad
{
public:
    enum Operations : uint8_t { Minus, Plus, Multiply, Assign, Variable, Const, Deleted};
    enum Operands : uint8_t { IsLink, IsConst, IsVariable, IsEmpty};
    Triad(Operations op, Operands leftOp, Operands rightOp, std::string leftValue, std::string rightValue = "");
    Operands leftOperandType, rightOperandType;
    Operations operation;
    std::string leftOperandValue, rightOperandValue;
    friend std::ostream& operator<<(std::ostream& stream, const Triad& counter);
};

class Translator
{
    friend class Triad;
public:
    Translator();
    ~Translator();
    
    void StartParse();
    void StartOptimize();
    
private:
    std::vector<Triad> triadTable;
    std::vector<std::pair<std::string, int>> symTable;
    std::ifstream _inputFile;
    char curr;
    long _numberOfTriads;
    
    void GetChar();
    bool IsDigit(char value);
    bool IsSpace(char value);
    bool IsAlpha(char value);
    void Error(std::string msg);
    void AddOrReplace(const std::pair<std::string, int>& value);
    
    int ProcS();
    int ProcE();
    int ProcT();
    int ProcI();
    int ProcR();

    void CheckFirstRule(Triad*);
    void CheckSecondRule(Triad*);
    void CheckThirdRule(Triad*);
    void CheckFourthRule(Triad*);
};

