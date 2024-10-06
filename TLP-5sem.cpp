#include <fstream>

enum class States : std::uint8_t { Default, Slash, Comment, Star };

int main() {
	std::fstream fs;
	std::ofstream out;
	States state = States::Default;

	fs.open("test.cpp");
	out.open("out_test.cpp", std::ios::out);

	if (fs.is_open() && out.is_open()) {
		char tmp;
		while (!fs.eof()) {
			fs.get(tmp);
			switch (state) {
			case States::Default:
				if (tmp == '/') state = States::Slash;
				else out.put(tmp);
				break;
			case States::Slash:
				if (tmp == '/') out.put('/');
				else if (tmp == '*') state = States::Comment;
				else {
					out.put('/');
					out.put(tmp);
					state = States::Default;
				}
				break;
			case States::Comment:
				if (tmp == '*') state = States::Star;
				break;
			case States::Star:
				if (tmp == '/') state = States::Default;
				else if (tmp == '*') state = States::Star;
				else state = States::Comment;
				break;
			}
		}
	}

	fs.close();
	out.close();

	return 0;
}