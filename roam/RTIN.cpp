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

RTIN::RTIN(int levels) {
	size = ( 2 << levels ) - 1;
	e_T = new float[size];
	flags = new int[size];
}

RTIN::~RTIN() {
	if (e_T != 0) {
		delete [] e_T;
		e_T = 0;
	}
	if (flags != 0) {
		delete [] flags;
		flags = 0;
	}
}

// void &RTIN::operator=(RTIN r) {
// 	//check allocated memory
// 	if (e_T != 0) {
// 		delete [] e_T;
// 		e_T = 0;
// 	}

// 	if (flags != 0) {
// 		delete [] flags;
// 		flags = 0;
// 	}

// 	size = r.size;
// 	flags = r.flags;
// 	e_T = r.e_T;
// }

void RTIN::Draw() {

}

int RTIN::Parent(int triangle) {
	return (triangle - 1) >> 1;
}

int RTIN::Child(child c, int triangle) {
		return (triangle << 1) + c;
}

//implement this
int RTIN::Neighbor(neighbor n, int triangle) {
	return 0;
}