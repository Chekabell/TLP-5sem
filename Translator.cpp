#include "Translator.h"

Triad::Triad(Operations op, Operands leftOp, Operands rightOp, std::string leftValue, std::string rightValue)
{
    operation = op;
    leftOperandType = leftOp;
    rightOperandType = rightOp;
    leftOperandValue = leftValue;
    rightOperandValue = rightValue;
}

std::ostream& operator<<(std::ostream& stream, const Triad& triad)
{
    switch (triad.operation)
    {
    case Triad::Deleted:
        stream << "DELETED TRIAD";
        return stream;
    case Triad::Minus:
        std::cout << "-";
        break;
    case Triad::Plus:
        std::cout << "+";
        break;
    case Triad::Multiply:
        std::cout << "*";
        break;
    case Triad::Assign:
        std::cout << "=";
        break;
    case Triad::Variable:
        std::cout << "V";
        break;
    case Triad::Const:
        std::cout << "C";
        break;
    }
        
    std::cout << "(";
        
    if (triad.leftOperandType == Triad::IsLink)
        std::cout << "^" + triad.leftOperandValue;
    else if (triad.leftOperandType == Triad::IsEmpty)
        std::cout << "@";
    else
        std::cout << triad.leftOperandValue;

    std::cout << ", ";
        
    if (triad.rightOperandType == Triad::IsLink)
        std::cout << "^" + triad.rightOperandValue;
    else if (triad.rightOperandType == Triad::IsEmpty)
        std::cout << "@";
    else
        std::cout << triad.rightOperandValue;
        
    std::cout << ")";
    return stream;
}

Translator::Translator()
{
    _inputFile.open("input.txt", std::ios::in);
    curr = '\0';
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

void Translator::StartOptimize()
{
    for (auto& tr : triadTable)
    {
        CheckFirstRule(&tr);
        CheckSecondRule(&tr);
        CheckThirdRule(&tr);
        CheckFourthRule(&tr);
    }
    for(int i = 0; i < triadTable.size(); i++)
    {
        std::cout << i+1 << ":\t" << triadTable[i] << '\n';
    }
}

void Translator::CheckFirstRule(Triad* tr)
{
    if(tr->operation == Triad::Operations::Minus
        && tr->leftOperandType == Triad::Operands::IsLink){
            Triad* linkTriad = &triadTable[stoi(tr->leftOperandValue)-1];
            if(linkTriad->operation == Triad::Operations::Const)
            {
                tr->leftOperandType = linkTriad->leftOperandType;
                tr->leftOperandValue = linkTriad->leftOperandValue;
                linkTriad->operation = Triad::Operations::Deleted;
            }
        }
    else if(tr->operation != Triad::Operations::Assign && tr->rightOperandType == Triad::Operands::IsLink)
    {
        Triad* linkTriad = &triadTable[stoi(tr->rightOperandValue)-1];
        if(linkTriad->operation == Triad::Operations::Const)
        {
            tr->rightOperandType = linkTriad->leftOperandType;
            tr->rightOperandValue = linkTriad->leftOperandValue;
            linkTriad->operation = Triad::Operations::Deleted;
        }
    }
}

void Translator::CheckSecondRule(Triad* tr)
{
    if(tr->operation == Triad::Operations::Minus
        && tr->leftOperandType == Triad::Operands::IsConst)
    {
        tr->operation = Triad::Operations::Const;
        tr->leftOperandType = Triad::Operands::IsConst;
        tr->leftOperandValue = std::to_string(stoi(tr->leftOperandValue)*-1);
    }
}

void Translator::CheckThirdRule(Triad* tr)
{
    if(tr->operation != Triad::Operations::Assign
        && tr->rightOperandType == Triad::Operands::IsConst
        && tr->leftOperandType == Triad::Operands::IsLink)
        {
            Triad* linkTriad = &triadTable[stoi(tr->leftOperandValue)-1];
            if(linkTriad->operation == Triad::Operations::Const)
            {
                tr->leftOperandType = Triad::Operands::IsConst;
                tr->rightOperandType = Triad::Operands::IsEmpty;
                if(tr->operation == Triad::Operations::Plus)
                {
                    tr->leftOperandValue = std::to_string(stoi(linkTriad->leftOperandValue)
                        +stoi(tr->rightOperandValue));
                }
                else
                {
                    tr->leftOperandValue = std::to_string(stoi(linkTriad->leftOperandValue)
                        *stoi(tr->rightOperandValue));
                }
                tr->operation = Triad::Operations::Const;
                tr->rightOperandValue = "";
                linkTriad->operation = Triad::Operations::Deleted;
            }
        }
}

void Translator::CheckFourthRule(Triad* tr)
{
    if(tr->operation == Triad::Operations::Assign
        && tr->leftOperandType == Triad::Operands::IsLink)
    {
        Triad* linkTriad = &triadTable[stoi(tr->leftOperandValue)-1];
        if(linkTriad->operation == Triad::Operations::Variable)
        {
            tr->leftOperandType = Triad::Operands::IsVariable;
            tr->leftOperandValue = linkTriad->leftOperandValue;
            linkTriad->operation = Triad::Operations::Deleted;
        }
    }
}



void Translator::GetChar()
{
    curr = _inputFile.get();
    while(IsSpace(curr))
        curr = _inputFile.get();
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
    triadTable.push_back(*new Triad(Triad::Variable, Triad::IsVariable, Triad::IsEmpty, id));
    
    int leftOp = _numberOfTriads;
    if(curr != ',') Error("Expected ','");
    
    GetChar();
    int rightOp = ProcE();
    symTable.push_back({id, rightOp});
    
    std::cout << ++_numberOfTriads << ":\t" << "=(^" << leftOp << ", ^" << rightOp <<")\n";
    triadTable.push_back(*new Triad(Triad::Assign, Triad::IsLink, Triad::IsLink, std::to_string(leftOp), std::to_string(rightOp)));
    
    if(curr != ')') Error("Expected ')'");
    GetChar();
    return _numberOfTriads;
}

int Translator::ProcE()
{
    if(IsDigit(curr))
        Error("Expected a '#' or operand");
    if (curr == '-')
    {
        GetChar();
        int triad = ProcE();
        std::cout << ++_numberOfTriads << ":\t" << "-(^" << triad << ", @)\n";
        triadTable.push_back(*new Triad(Triad::Minus, Triad::IsLink, Triad::IsEmpty, std::to_string(triad)));
        return _numberOfTriads;
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
            triadTable.push_back(*new Triad(Triad::Plus, Triad::IsLink, Triad::IsLink, std::to_string(leftOp), std::to_string(rightOp)));
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
            triadTable.push_back(*new Triad(Triad::Multiply, Triad::IsLink, Triad::IsLink, std::to_string(leftOp), std::to_string(rightOp)));
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
    return _numberOfTriads;
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
            triadTable.push_back(*new Triad(Triad::Variable, Triad::IsVariable, Triad::IsEmpty, id));
            return _numberOfTriads;
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
    std::cout << ++_numberOfTriads << ":\t" << "C(" << result << ", @)\n";
    triadTable.push_back(*new Triad(Triad::Const, Triad::IsConst, Triad::IsEmpty, result));
    return _numberOfTriads;
}


