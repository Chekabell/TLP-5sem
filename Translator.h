#pragma once
#include <iosfwd>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <string>

#include "Scanner.h"

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

class Translator : public Scanner
{
    std::ofstream outputFile;
    std::stack<Token> tokenStack;
    std::vector<std::string> varList;
    std::vector<Triad*> triadsList;

    std::vector<std::pair<char,char>> equalMatrix;
    std::vector<std::pair<char,char>> lessMatrix;
    std::vector<std::pair<char,char>> lessOrEqualMatrix;
    std::vector<std::pair<char,char>> moreMatrix;

    std::vector<std::pair<char, std::vector<std::string>>> grammar;
public:
    Translator();
    ~Translator();
    
    void Start();
    
private:
    void InitializeMatrix();
    void InitializeGrammar();
    bool CheckShiftReduce(Token);
    void Reduce();
    void ToTriad(Token&, std::deque<Token>, std::string);
    bool HasInMatrix(Token, Token, std::vector<std::pair<char,char>>);
    char FindRule(std::string);
    std::string DequeToString(std::deque<Token>);
    std::string getStackString();
    void logStack(std::string);
    void outputTriads();
    

    
};
