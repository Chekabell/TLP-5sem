#include "Scanner.h"

Scanner::Scanner(const std::string& fileName)
{
    _inputFile = std::fstream(fileName);
    NextChar();
}

Scanner::~Scanner()
{
    _inputFile.close();
}

Token Scanner::GetToken()
{
    if(IsAlpha(currentChar))
    {
        return *new Token(ProcI(),TokenType::I);
    }
    if(currentChar == '#')
    {
        NextChar();
        return *new Token(ProcC(),TokenType::C);
    }
    if(currentChar == '(' || currentChar == ')' || currentChar == '['
        || currentChar == ']' || currentChar == '=' || currentChar == '+'
        || currentChar == '-' || currentChar == '*' || currentChar == '/'
        || currentChar == ',')
    {
        Token* token = new Token(std::string(&currentChar),TokenType::Terminal);
        NextChar();
        return *token;
    }
    if(currentChar == EOF)
    {
        return *new Token("~",TokenType::Sep);
    }
    Error("Unacceptable character");
}

void Scanner::NextChar()
{
    currentChar = _inputFile.get();
    while(IsSpace(currentChar))
    {
        currentChar = _inputFile.get();
    }
}

bool Scanner::IsDigit(char value)
{
    return value >= '0' && value <= '9';
}

bool Scanner::IsSpace(char value)
{
    return value == ' ' || value == '\t' || value == '\n' || value == '\r';
}

bool Scanner::IsAlpha(char value)
{
    return value >= 'a' && value <= 'z';
}

void Scanner::Error(std::string msg)
{
    _inputFile.close();
    std::cout << "\nError: " << msg << "\n";
    throw "Abort parsing.";
}

std::string Scanner::ProcI()
{
    std::string id;
    while(IsAlpha(currentChar))
    {
        id += currentChar;
        NextChar();
    }
    return id;
}

std::string Scanner::ProcC()
{
    std::string id;
    while(IsDigit(currentChar))
    {
        id += currentChar;
        NextChar();
    }
    return id;
}
