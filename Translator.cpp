#include "Translator.h"


Translator::Translator()
{
    _inputFile.open("input.txt", std::ios::in);
    curr = '\0';
    _numberOfOperands = 1;
}

Translator::~Translator()
{
    _inputFile.close();
}


void Translator::StartParse()
{
    std::cout << "Begin parsing.\n";
    GetChar();
    while(curr != EOF)
        ProcS();

    std::cout << "End parsing.\n";
    std::cout << symTable.size() << " variables defined:\n";
    
    for(auto it = symTable.begin(); it != symTable.end(); it++)
        std::cout << it->first << " = " << it->second << '\n';
}

void Translator::GetChar()
{
    curr = _inputFile.get();
    while(IsSpace(curr))
    {
        curr = _inputFile.get();
    }
    //std::cout << curr;
}
bool Translator::IsDigit(char value)
{
    return value >= '0' && value <= '9';
}

bool Translator::IsSpace(char value)
{
    return value == ' ' || value == '\t' || value == '\n' || value == '\r';
}

bool Translator::IsAlpha(char value)
{
    return value >= 'a' && value <= 'z';
}

void Translator::Error(std::string msg)
{
    _inputFile.close();
    std::cout << "\nError: " << msg << "\n";
    throw "Abort parsing.";
}

void Translator::AddOrReplace(const std::pair<std::string, int>& value)
{
    for (auto &x :  symTable)
    {
        if(x.first == value.first)
        {
            x.second = value.second;
            return;
        }
    }
    symTable.push_back(value);
}

int Translator::ProcS()
{
    if(curr != '(') Error("Expected '('");
    GetChar();
    
    std::string id;
    while(IsAlpha(curr))
    {
        id += curr;
        GetChar();
    }

    if(curr != ',') Error("Expected ','");
    GetChar(); 
    std::pair<std::string, int> pair = std::make_pair(id, ProcE());
    AddOrReplace(pair);
    if(curr != ')') Error("Expected ')'");
    GetChar();
    
    std::cout << "Operator defined " << _numberOfOperands << ":\t";
    std::cout << pair.first << " = " << pair.second << '\n';
    _numberOfOperands++;
    return pair.second;
}


int Translator::ProcE()
{
    if(IsDigit(curr))
        Error("Expected a '#' or operand");
    if (curr == '-')
    {
        GetChar();
        return ProcE()*(-1);
    }
    if (curr == '+')
        return ProcT();
    if (curr == '*')
        return ProcT();
    if (curr == '(')
        return ProcS();
    if (curr == '#')
        return ProcR();
    if (IsAlpha(curr))
        return ProcI();
    Error("Error");
}

int Translator::ProcT()
{
    int res;
    if(curr == '+')
    {
        GetChar();
        if(curr != '(') Error("Expected '('");
        GetChar();
        res = ProcE();
        if(curr != ',') Error("Expected ','");
        GetChar();
        while(curr != EOF)
        {
            res += ProcE();
            if (curr == ')')
                break;
            if(curr == ',')
                GetChar();
            else
                Error("Expected ',' or ')'");
        }
    }
    if(curr == '*')
    {
        GetChar();
        if(curr != '(') Error("Expected '('");
        GetChar();
        res = ProcE();
        if(curr != ',') Error("Expected ','");
        GetChar();
        while(curr != EOF)
        {
            res *= ProcE();
            if (curr == ')')
                break;
            if(curr == ',')
                GetChar();
            else
                Error("Expected ',' or ')'");
        }
    }
    GetChar();
    return res;
}

int Translator::ProcI()
{
    std::string id;
    while(IsAlpha(curr))
    {
        id += curr;
        GetChar();
    }
    
    for (auto x: symTable)
    {
        if(x.first == id)
        {
            return x.second;
        }
    }
    Error("Operator '" + id + "' not defined");
}

int Translator::ProcR()
{
    std::string result;
    GetChar();
    while (IsDigit(curr)) {
        result += curr;
        GetChar();
    }
    return stoi(result);
}


