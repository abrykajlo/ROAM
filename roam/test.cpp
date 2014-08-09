#include <iostream>

#include "RTIN.h"

using namespace std;

int main() {
	RTIN terrain(0);
	cout << sizeof(int) << endl;
	cout << "test 1 ";
	if (terrain.size == 1) {
		cout << "passed";
	} else {
		cout << "failed";
	}
	cout << endl;

	terrain(1);
	cout << "test 2 ";
	if (terrain.size == 3) {
		cout << "passed";
	} else {
		cout << "failed";
	}
	cout << endl;

	terrain = RTIN(2);
	cout << "test 3 ";
	if (terrain.size == 7) {
		cout << "passed";
	} else {
		cout << "failed";
	}
	cout << endl;

	terrain = RTIN(10);
	cout << "test 3 ";
	if (terrain.size == 1023) {
		cout << "passed";
	} else {
		cout << "failed";
	}
	cout << endl;
	return 0;
}