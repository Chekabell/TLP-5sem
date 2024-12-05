#include "Translator.h"


Translator::Translator()
{
    _inputFile.open("input.txt", std::ios::in);
    curr = '\0';
    _numberOfOperations = 1;
    _numberOfTriads = 0;
}

Translator::~Translator()
{
    _inputFile.close();
}

void Translator::StartParse()
{
    GetChar();
    while(curr != EOF)
        ProcS();
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

    
    std::cout << ++_numberOfTriads << ":\t" << "V(" << id << ", @)\n";
    int leftOp = _numberOfTriads;
    if(curr != ',') Error("Expected ','");
    GetChar();
    int rightOp = ProcE();
    symTable.push_back({id, rightOp});
    std::cout << ++_numberOfTriads << ":\t" << "=(^" << leftOp << ", ^" << rightOp <<")\n";
    if(curr != ')') Error("Expected ')'");
    GetChar();
    
    return _numberOfTriads;
}


int Translator::ProcE(int minus)
{
    if(IsDigit(curr))
        Error("Expected a '#' or operand");
    if (curr == '-')
    {
        int i = 0;
        while(curr == '-')
        {
            GetChar();
            i++;
        }
        return ProcE(i);
    }
    if (curr == '+')
        return ProcT();
    if (curr == '*')
        return ProcT();
    if (curr == '(')
        return ProcS();
    if (curr == '#')
        return ProcR(minus);
    if (IsAlpha(curr))
        return ProcI();
    Error("Error");
}

int Translator::ProcT()
{
    int leftOp, rightOp;
    if(curr == '+')
    {
        GetChar();
        if(curr != '(') Error("Expected '('");
        GetChar();
        leftOp = ProcE();
        if(curr != ',') Error("Expected ','");
        GetChar();
        while(curr != EOF)
        {
            rightOp = ProcE();
            std::cout << ++_numberOfTriads << ":\t" << "+(^" << leftOp << ", ^" << rightOp << ")\n";
            if (curr == ')')
                break;
            if(curr == ',')
                GetChar();
            else
                Error("Expected ',' or ')'");
            leftOp = _numberOfTriads;
        }
    }
    if(curr == '*')
    {
        GetChar();
        if(curr != '(') Error("Expected '('");
        GetChar();
        leftOp = ProcE();
        if(curr != ',') Error("Expected ','");
        GetChar();
        while(curr != EOF)
        {
            rightOp = ProcE();
            std::cout << ++_numberOfTriads << ":\t" << "*(^" << leftOp << ", ^" << rightOp << ")\n";
            if (curr == ')')
                break;
            if(curr == ',')
                GetChar();
            else
                Error("Expected ',' or ')'");
            leftOp = _numberOfTriads;
        }
    }
    GetChar();
    return ++rightOp;
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
            std::cout << ++_numberOfTriads << ":\t" << "V(" << id << ", @)\n";
            return _numberOfTriads;
        }
    }
    Error("Operator '" + id + "' not defined");
}

int Translator::ProcR(int minus)
{
    std::string result;
    GetChar();
    while (IsDigit(curr)) {
        result += curr;
        GetChar();
    }
    std::cout << ++_numberOfTriads << ":\t" << "C(" << result << ", @)\n";
    while(minus)
    {
        std::cout << ++_numberOfTriads << ":\t" << "-(^" << _numberOfTriads-1 << ", @)\n";
        minus--;
    }
    return _numberOfTriads;
}


