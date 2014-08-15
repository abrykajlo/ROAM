#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

union conv {
	unsigned int i;
	float f;
};

int main() {
	union conv c;
	c.i = 0x7F000000;
	cout << c.f;
	return 0;
}