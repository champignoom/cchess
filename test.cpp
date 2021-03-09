#include <array>
#include <memory>
#include <regex>
using namespace std;


int main() {
	static const regex e{R"((?:[jmpxswbz][1-9]|[qh2-5][jmpxswbz1-9])[jtp][1-9])"};
}
