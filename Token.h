#pragma once
#include <string>

enum TokenType : uint8_t
{
    L = 'L', T = 'T', E = 'E', S = 'S', I = 'I', C = 'C', Sep = '~', Terminal = '@'
};

enum PrecedingType : uint8_t
{
    None = ' ', Less = '<', LessOrEqual = '&', Equal = '=', More = '>'
};

enum OpeartionType : uint8_t
{
    IsNotBinary = ' ', Minus = '-', Plus ='+', Multiply = '*'
};

class Token
{
    OpeartionType operation;
    std::string value;
    TokenType type;
    PrecedingType precedingType;
    int triadNumber;
public:
    Token(std::string val, TokenType typ, PrecedingType precedingTyp = None, OpeartionType opTyp = IsNotBinary)
    {
        value = val;
        type = typ;
        precedingType = precedingTyp;
        operation = opTyp;
    }

    void SetOperation(OpeartionType op)
    {
        operation = op;
    }
    OpeartionType GetOperation()
    {
        return operation;
    }
    std::string getValue()
    {
        return value;
    }
    void setValue(std::string val)
    {
        value = val;
    }
    TokenType getType()
    {
        return type;
    }
    PrecedingType getPrecedingType()
    {
        return precedingType;
    }
    int getNumberTriad()
    {
        return triadNumber;
    }

    void setPrecedingType(PrecedingType preceding)
    {
        precedingType = preceding;
    }
    void setTriadNumber(int number)
    {
        triadNumber = number;
    }
    
    char GetSym() {
        if (type != TokenType::Terminal)
            return type;
        return value[0];
    }
};
