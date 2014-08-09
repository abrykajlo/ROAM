#include <iostream>

#include "RTIN.h"

using namespace std;

int main() {
	RTIN terrain(10);
	cout << terrain.Neighbor(L, 12) << endl;

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