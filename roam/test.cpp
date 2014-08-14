#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

bool compare(int i, int j) {
	return j < i;
}

int main() {
	std::vector<int> ints;
	ints.push_back(5);
	ints.push_back(3);
	ints.push_back(7);
	std::cout << ints.front() << endl;
	pop_heap(ints.begin(), ints.end()); ints.pop_back();
	cout << ints.front() << endl;
	make_heap(ints.begin(), ints.end(), compare);
	ints.push_back(1); push_heap(ints.begin(), ints.end(), compare);
	cout << ints.front();
	return 0;
}