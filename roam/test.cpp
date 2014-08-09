#include <iostream>

#include "RTIN.h"

using namespace std;

int main() {
	RTIN terrain(4);
	terrain.flags[1] = 1;
	terrain.flags[2] = 1;
	terrain.ForceSplit(2);
	terrain.ForceSplit(4);
	terrain.ForceSplit(9);
	for (int i = 0; i < terrain.size; i++) {
		cout << terrain.flags[i] << endl;
	}

	// terrain = RTIN(1);
	// cout << "test 2 ";
	// if (terrain.size == 3) {
	// 	cout << "passed";
	// } else {
	// 	cout << "failed";
	// }
	// cout << endl;

	// terrain = RTIN(2);
	// cout << "test 3 ";
	// if (terrain.size == 7) {
	// 	cout << "passed";
	// } else {
	// 	cout << "failed";
	// }
	// cout << endl;

	// terrain = RTIN(10);
	// cout << "test 3 ";
	// if (terrain.size == 1023) {
	// 	cout << "passed";
	// } else {
	// 	cout << "failed";
	// }
	// cout << endl;
	return 0;
}