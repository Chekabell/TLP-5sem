#include <iostream>
#include <fstream>

enum States { Default, Slash, Comment, Star };

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
				if (tmp == '/') state = States::Slash;
				else out << tmp;
				break;
			case States::Slash:
				if (tmp == '/') out << '/';
				else if (tmp == '*') state = States::Comment;
				else {
					out << '/';
					out << tmp;
					state = States::Default;
				}
				break;
			case States::Comment:
				if (tmp == '*') state = States::Star;
				break;
			case States::Star:
				if (tmp == '/') state = States::Default;
				else if (tmp == '*') state = state;
				else state = States::Comment;
				break;
			}
		}
	}

	fs.close();
	out.close();

	return 0;
}