#include <fstream>
#include <iostream>

enum class States : uint8_t { Default, Slash, MultilineComment, SingleLineComment,
	Asterisk, Char, String, BackSlashInChar, BackSlashInString,
	Block, Number, Decimal, DotAtBeginNumber, BeginE, Sign, E, Long, Float};

bool isNumber(char tmp)
{
	return tmp >= '0' && tmp <= '9';
}

bool isAlphabet(char tmp)
{
	return (tmp >= 'A' && tmp <= 'Z') || (tmp >= 'a' && tmp <= 'z') || tmp == '_';
}

bool isNotNumberAndAlphabet(char tmp)
{
	return !isNumber(tmp) && !isAlphabet(tmp);
}

int main() {
	std::fstream fs;
	std::ofstream out;
	
	States state = States::Default;
	
	fs.open("test3.cpp");
	out.open("out_test3.cpp", std::ios::out);
	
	if (fs.is_open() && out.is_open()) {
		char tmp;
		while (fs.get(tmp)) {
			switch (state) {
			case States::Default:
				if(tmp == '/')			state = States::Slash;
				else if(tmp == '\'')	state = States::Char;
				else if(tmp == '\"')	state = States::String;
				else if(isNumber(tmp)){
					state = States::Number;
					out.put(tmp);
				}
				else if(tmp == '.')
				{
					state = States::DotAtBeginNumber;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))state = States::Block;
				break;
			case States::Slash:
				if (tmp == '\'')						state = States::Char;
				else if (tmp =='"')						state = States::String;
				else if (tmp == '/')					state = States::SingleLineComment;
				else if (tmp == '*')					state = States::MultilineComment;
				else if (isNotNumberAndAlphabet(tmp))	state = States::Block;
				else if (isNumber(tmp))
				{
					state = States::Number;	
					out.put(tmp);
				}
				else if (tmp == '.')
				{
					state = States::DotAtBeginNumber;
					out.put(tmp);
				}
				break;
			case States::MultilineComment:
				if (tmp == '*')			state = States::Asterisk;
				break;
			case States::Asterisk:
				if (tmp == '/')			state = States::Default;
				else if (tmp != '*')	state = States::MultilineComment;
				break;
			case States::SingleLineComment:
				if (tmp == '\n' || tmp == '\r') state = States::Default;
				break;
			case States::Char:
				if (tmp == '\\')		state = States::BackSlashInChar;
				else if (tmp == '\'')	state = States::Default;
				break;
			case States::BackSlashInChar:
				state = States::Char;
				break;
			case States::String:
				if (tmp == '\\')		state = States::BackSlashInString;
				else if (tmp == '\"')	state = States::Default;
				break;
			case States::BackSlashInString:
				state = States::String;
				break;
			case States::Block:
				if (tmp == '/') state = States::Slash;
				else if (isNotNumberAndAlphabet(tmp)) state = States::Default;
				break;
			case States::Number:
				if (isNumber(tmp)) out.put(tmp);
				else if (tmp == 'e')
				{
					state = States::BeginE;
					out.put(tmp);
				}
				else if (tmp == '.')
				{
					state = States::Decimal;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << tmp << " error\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << tmp << " error\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << tmp << " error\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << tmp << " error\n";
				}
				break;
			case States::DotAtBeginNumber:
				if (isNumber(tmp))
				{
					state = States::Decimal;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << tmp << " error\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << tmp << " error\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << tmp << " error\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << tmp << " error\n";
				}
				break;
			case States::Decimal:
				if(isNumber(tmp)) out.put(tmp);
				else if(tmp == 'e')
				{
					state = States::BeginE;
					out.put(tmp);
				}
				else if(tmp == 'F' || tmp == 'f')
				{
					state = States::Float;
					out.put(tmp);
				}
				else if(tmp == 'L' || tmp == 'l')
				{
					state = States::Long;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << " double\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << " double\n";
				}
				else if (tmp == '.')
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << " double\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << " double\n";
				}
				break;
			case States::BeginE:
				if(tmp == '+' || tmp == '-')
				{
					state = States::Sign;
					out.put(tmp);
				}
				else if (isNumber(tmp))
				{
					state = States::E;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << tmp << " error\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << tmp << " error\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << tmp << " error\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << tmp << " error\n";
				}
				break;
			case States::Sign:
				if (isNumber(tmp))
				{
					state = States::E;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << tmp << " error\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << tmp << " error\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << tmp << " error\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << tmp << " error\n";
				}
				break;
			case States::E:
				if(isNumber(tmp)) out.put(tmp);
				else if(tmp == 'L' || tmp == 'l')
				{
					state = States::Long;
					out.put(tmp);
				}
				else if (tmp == 'F' || tmp == 'f')
				{
					state = States::Float;
					out.put(tmp);
				}
				else if(isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << " double\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << " double\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << " double\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << " double\n";
				}
				break;
			case States::Long:
				if (isNumber(tmp) || isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << " long double\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << " long double\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << " long double\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << " long double\n";
				}
				break;
			case States::Float:
				if (isNumber(tmp) || isAlphabet(tmp))
				{
					state = States::Block;
					out << tmp << " error\n";
				}
				else if (tmp == '"')
				{
					state = States::String;
					out << " float\n";
				}
				else if (tmp == '\'')
				{
					state = States::Char;
					out << " float\n";
				}
				else if (tmp == '/')
				{
					state = States::Slash;
					out << " float\n";
				}
				else if (isNotNumberAndAlphabet(tmp))
				{
					state = States::Default;
					out << " float\n";
				}
				break;
			}
		}
	}

	fs.close();
	out.close();

	return 0;
}