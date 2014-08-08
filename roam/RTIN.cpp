#include <iostream>
#include "RTIN.h"

using namespace std;

RTIN::RTIN() {
	size = 0;
	e_T = 0;
	flags = 0;
	VBO = 0;
	IBO = 0;
}

void RTIN::Test() {
//Test for class
}

RTIN::RTIN(int levels) {
	size = ( 2 << (levels + 1) ) - 1;
	e_T = new float[size];
	flags = new int[size];
}

RTIN::~RTIN() {
	if (e_T != 0) {
		delete [] e_T;
	}
	if (flags != 0) {
		delete [] flags;
	}
}

int RTIN::Parent(int node) {
	return (node - 1) >> 1;
}

int RTIN::Child(int child, int node) {
	if (child == LEFT || child == RIGHT)
		return (node << 1) + child;
	return -1;
}