#include <iostream>
#include <fstream>

enum States { Default, BeginComment, MultilineComment, MonolineComment, Asterisk, Char, String, BackSlashInChar, BackSlashInString };

int main() {
	std::fstream fs;
	std::ofstream out;
	char tmp;
	States state = States::Default;

	fs.open("test.cpp");
	out.open("outtest.cpp", std::ios::out);

	if (fs.is_open() && out.is_open()) {
		while (!fs.eof()) {
			fs.get(tmp);
			switch (state) {
			case States::Default:
				if (tmp == '/')			state = States::BeginComment;
				else if (tmp == '\'') {
					out.put(tmp);
					state = States::Char;
				}
				else if (tmp == '\"') {
					state = States::String;
					out.put(tmp);
				}
				else out.put(tmp);
				break;
			case States::BeginComment:
				if (tmp == '/')			state = States::MonolineComment;
				else if (tmp == '*')	state = States::MultilineComment;
				else {
					out.put('/');
					out.put(tmp);
					state = States::Default;
				}
				break;
			case States::MultilineComment:
				if (tmp == '*') state = States::Asterisk;
				break;
			case States::Asterisk:
				if (tmp == '/') {
					state = States::Default;
					out.put(' ');
				}
				else if (tmp != '*') state = States::MultilineComment;
				break;
			case States::MonolineComment:
				if (tmp == '\n' || tmp == '\r') {
					out.put(tmp);
					state = States::Default;
				}
				break;
			case States::Char:
				if (tmp == '\\') {
					state = States::BackSlashInChar;
				}
				else if (tmp == '\'') {
					state = States::Default;
				}
				out.put(tmp);
				break;
			case States::BackSlashInChar:
				out.put(tmp);
				state = States::Char;
				break;
			case States::String:
				if (tmp == '\\') {
					state = States::BackSlashInString;
				}
				else if (tmp == '\"') {
					state = States::Default;
				}
				out.put(tmp);
				break;
			case States::BackSlashInString:
				out.put(tmp);
				state = States::String;
				break;
			}
		}
	}

	fs.close();
	out.close();

	return 0;
}