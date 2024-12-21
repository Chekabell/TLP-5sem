#include "Translator.h"

#include <tuple>

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
    outputFile = std::ofstream("output.txt");
    InitializeMatrix();
    InitializeGrammar();
}

Translator::~Translator()
{
    
}

void Translator::Start()
{
    tokenStack.push(Token("~", TokenType::Sep));
    Token currentToken = GetToken();

    while(true)
    {
        if(tokenStack.top().getType() == TokenType::Sep && currentToken.getType() == TokenType::Sep)
            break;

        while(CheckShiftReduce(currentToken))
            continue;

        currentToken = GetToken();

        if(tokenStack.top().GetOperation() !=IsNotBinary)
            currentToken.SetOperation(tokenStack.top().GetOperation());
        
        if(currentToken.GetSym() == '-')
            currentToken.SetOperation(OpeartionType::Minus);
        else if(currentToken.GetSym() == '+')
            currentToken.SetOperation(OpeartionType::Plus);
        else if(currentToken.GetSym() == '*')
            currentToken.SetOperation(OpeartionType::Multiply);
        
        if(tokenStack.top().getType() == TokenType::I && currentToken.getValue() != "=")
        {
            bool found = false;
            for(auto var : varList)
            {
                if(tokenStack.top().getValue() == var)
                {
                    found = true;
                    break;
                }
            }

            if(!found)
                Error("Variable with name " + tokenStack.top().getValue() + " is not initialized");
        }
    }
    std::cout << "Fine\n";
    outputTriads();
}

void Translator::outputTriads() {
    int i = 1;
    for (auto x : triadsList) {
        std::cout << i << ":\t" << *x <<"\n";
        i++;
    }
}

void Translator::InitializeMatrix()
{
    equalMatrix ={
        {'L','S'}, {'L','~'},{'E',']'},
        {'T',')'}, {'T',','},
        {'-','('}, {'+','('},{'*','('},
        {',','E'}, {'[','I'},{'=','E'},
        {'I','='}
    };
    lessMatrix = {
        {'L','['},{'(','S'},{'(','-'},
        {'(','+'},{'(','*'},{'(','['},
        {'(','I'},{'(','C'},{',','S'},
        {',','-'},{',','+'},{',','*'},
        {',','['},{',','I'},{',','C'},
        {'=','S'},{'=','-'},{'=','+'},
        {'=','*'},{'=','['},{'=','I'},
        {'=','C'},{'~','S'},{'~','['},
        {'(','E'}
    };
    lessOrEqualMatrix = {
        {'~','L'},{'(','T'}
    };
    moreMatrix = {
        {'S','~'},{'S',')'},{'S',','},{'S',','},
        {'S','['},{'S',']'},{'E',')'},
        {'E',','},{')',')'},{')',','},
        {')',']'},{']',')'},{']',','},
        {']','['},{']',']'},{']','~'},
        {'I',')'},{'I',','},{'I',']'},
        {'C',')'},{'C',','},{'C',']'}
    };
}

void Translator::InitializeGrammar()
{
    grammar = {
        {'L',{"LS","S"}},
        {'S',{"[I=E]"}},
        {'E',{"-(T)","+(T)","*(T)","S","I","C"}},
        {'T',{"T,E","E"}}
    };
}

bool Translator::HasInMatrix(Token leftToken, Token rightToken, std::vector<std::pair<char, char>> matrix)
{
    for(auto pairInMatrix:matrix)
        if(pairInMatrix.first == leftToken.GetSym() && pairInMatrix.second == rightToken.GetSym())
            return true;
    return false;
}

bool Translator::CheckShiftReduce(Token token)
{
    if(HasInMatrix(tokenStack.top(), token, lessOrEqualMatrix))
    {
        token.setPrecedingType(PrecedingType::LessOrEqual);
        tokenStack.push(token);
        logStack("shift (less or equal)");
        std::cout << token.getType();
    }
    else if(HasInMatrix(tokenStack.top(), token, equalMatrix))
    {
        token.setPrecedingType(PrecedingType::Equal);
        if(tokenStack.top().GetOperation() != OpeartionType::IsNotBinary)
            token.SetOperation(tokenStack.top().GetOperation());
        tokenStack.push(token);
        logStack("shift (equal)");
        std::cout << token.getType();
    }
    else if(HasInMatrix(tokenStack.top(), token, lessMatrix))
    {
        token.setPrecedingType(PrecedingType::Less);
        tokenStack.push(token);
        logStack("shift (less)");
        std::cout << token.getType();
    }
    else if(HasInMatrix(tokenStack.top(), token, moreMatrix))
    {
        Reduce();
        logStack("reduce");
        std::cout << token.getType();
        return true;
    }
    else
    {
        Error("No such sequence of characters");
    }
    return false;
}

void Translator::Reduce()
{
    std::deque<Token> dequeRules = tokenStack._Get_container();

    bool reduced = false;
    char rule = NULL;
    std::string ruleString;
    while(!dequeRules.empty())
    {
        while(!dequeRules.empty() && dequeRules.front().getPrecedingType() != PrecedingType::Less
            && dequeRules.front().getPrecedingType() != PrecedingType::LessOrEqual)
                dequeRules.pop_front();

        ruleString = DequeToString(dequeRules);
        rule = FindRule(ruleString);
        if(rule!=NULL)
        {
            reduced = true;
            break;
        }

        dequeRules.pop_front();
    }

    if(!reduced)
        Error("Invalid syntax");
    
    Token tmp = tokenStack.top();
    tokenStack.pop();
    if(rule == 'L' && (tokenStack.top().GetSym() == '='
        || tokenStack.top().GetSym() == ','
        || tokenStack.top().GetSym() == '(')){
        rule = 'E';
    }
    else
    {
        tokenStack.push(tmp);
        for(int i = 0; i < dequeRules.size(); i++)
        {
            if(rule == 'S' && tokenStack.top().getType() == TokenType::I)
                varList.push_back(tokenStack.top().getValue());

            tokenStack.pop();
        }
    }

    Token deletedToken = Token("",static_cast<TokenType>(rule));
    ToTriad(deletedToken,dequeRules,ruleString);
    
    if (HasInMatrix(tokenStack.top(), deletedToken, lessOrEqualMatrix)) {
        deletedToken.setPrecedingType(PrecedingType::LessOrEqual);
    }
    else if (HasInMatrix(tokenStack.top(), deletedToken, equalMatrix)) {
        deletedToken.setPrecedingType(PrecedingType::Equal);
    }
    else if (HasInMatrix(tokenStack.top(), deletedToken, lessMatrix)) {
        deletedToken.setPrecedingType(PrecedingType::Less);
    }
    else {
        Error("invalid syntax (no such sequence of characters to reduce)");
    }

    tokenStack.push(deletedToken);
}

void Translator::ToTriad(Token& deletedToken, std::deque<Token> dequeToken, std::string rule)
{
    if(rule == "C")
    {
        triadsList.push_back(new Triad(Triad::Operations::Const,Triad::IsConst,Triad::IsEmpty,dequeToken[0].getValue(),"@"));
        deletedToken.setTriadNumber(triadsList.size());
        deletedToken.SetOperation(tokenStack.top().GetOperation());
    }
    else if(rule == "I")
    {
        triadsList.push_back(new Triad(Triad::Variable,Triad::IsVariable,Triad::IsEmpty,dequeToken[0].getValue(),"@"));
        deletedToken.setTriadNumber(triadsList.size());
        deletedToken.SetOperation(dequeToken[0].GetOperation());
    }
    else if(rule == "E")
    {
        deletedToken.setTriadNumber(dequeToken[0].getNumberTriad());
        deletedToken.SetOperation(dequeToken[0].GetOperation());
    }
    else if(rule == "T,E")
    {
        deletedToken.setTriadNumber(dequeToken[2].getNumberTriad()+1);
        deletedToken.SetOperation(dequeToken[2].GetOperation());
        if(deletedToken.GetOperation()== OpeartionType::Plus)
            triadsList.push_back(new Triad(Triad::Plus,Triad::IsLink,Triad::IsLink,std::to_string(dequeToken[0].getNumberTriad()),std::to_string(dequeToken[2].getNumberTriad())));
        else if(deletedToken.GetOperation()== OpeartionType::Minus)
            triadsList.push_back(new Triad(Triad::Minus,Triad::IsLink,Triad::IsLink,std::to_string(dequeToken[0].getNumberTriad()),std::to_string(dequeToken[2].getNumberTriad())));
        else if(deletedToken.GetOperation()== OpeartionType::Multiply)
            triadsList.push_back(new Triad(Triad::Multiply,Triad::IsLink,Triad::IsLink,std::to_string(dequeToken[0].getNumberTriad()),std::to_string(dequeToken[2].getNumberTriad())));
    }
    else if(rule == "+(T)")
    {
        deletedToken.setTriadNumber(triadsList.size());
        deletedToken.SetOperation(tokenStack.top().GetOperation());
    }
    else if(rule == "*(T)")
    {
        deletedToken.setTriadNumber(triadsList.size());
        deletedToken.SetOperation(tokenStack.top().GetOperation());
    }
    else if(rule == "-(T)")
    {
        deletedToken.setTriadNumber(triadsList.size());
        deletedToken.SetOperation(tokenStack.top().GetOperation());
    }
    else if(rule == "S")
    {
        deletedToken.SetOperation(tokenStack.top().GetOperation());
        deletedToken.setTriadNumber(dequeToken[0].getNumberTriad());
    }
    else if(rule == "[I=E]")
    {
        deletedToken.SetOperation(tokenStack.top().GetOperation());
        deletedToken.setTriadNumber(triadsList.size()+2);
        triadsList.push_back(new Triad(Triad::Variable,Triad::IsVariable,Triad::IsEmpty,dequeToken[1].getValue(),"@"));
        triadsList.push_back(new Triad(Triad::Assign,Triad::IsLink,Triad::IsLink,std::to_string(triadsList.size()),std::to_string(dequeToken[3].getNumberTriad())));
    }
}

std::string Translator::DequeToString(std::deque<Token> deque)  
{
    std::string result;

    while(!deque.empty())
    {
        result+=deque.front().GetSym();
        deque.pop_front();
    }
    
    return result;
}

char Translator::FindRule(std::string stringRule)
{
    for(auto& rules : grammar)
        for(auto& rule : rules.second)
            if(rule == stringRule)
                return rules.first;
    return NULL;
}

void Translator::logStack(std::string msg)
{
    std::cout << "Action:\t" << msg << "\n";
    std::cout << "Stack:\t\b" << getStackString() << "\n\n";
}

std::string Translator::getStackString() {
    std::stack<Token> tmp = tokenStack;
    std::vector<char> stack_chars;
    while (!tmp.empty()) {
        stack_chars.push_back(tmp.top().GetSym());
        stack_chars.push_back(tmp.top().getPrecedingType());
        tmp.pop();
    }

    std::reverse(stack_chars.begin(), stack_chars.end());
    return std::string(stack_chars.begin(), stack_chars.end());
}
