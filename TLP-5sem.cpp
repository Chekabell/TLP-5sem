#include <fstream>

enum class States : uint8_t { Default, BeginComment, MultilineComment, SingleLineComment, Asterisk, Char, String, BackSlashInChar, BackSlashInString };

int main() {
	std::fstream fs;
	std::ofstream out;

	States state = States::Default;

	fs.open("test.cpp");
	out.open("outtest.cpp", std::ios::out);

	if (fs.is_open() && out.is_open()) {
		char tmp;
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
				if (tmp == '/')			state = States::SingleLineComment;
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
			case States::SingleLineComment:
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