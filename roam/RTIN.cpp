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

RTIN &RTIN::operator=(RTIN r) {
	//check allocated memory
	if (e_T != 0) {
		delete [] e_T;
		e_T = 0;
	}

	if (flags != 0) {
		delete [] flags;
		flags = 0;
	}

	this->size = r.size;
	this->flags = r.flags;
	this->e_T = r.e_T;

	return *this;
}

void RTIN::Draw() {

}

int RTIN::Parent(int triangle) {
	return (triangle - 1) >> 1;
}

int RTIN::Child(child c, int triangle) {
		return (triangle << 1) + c;
}

//a result of -1 means there is no neighbor
int RTIN::Neighbor(neighbor n, int triangle) {
	if (triangle == 0) {
		return -1;
	} else if (triangle == 1) {
		if (n == B)
			return 2;
		else
			return -1;
	} else if (triangle == 2) {
		if (n == B)
			return 1;
		else
			return -1;
	} else {
		int parent = Parent(triangle);
		int nParent; //parents neighbor
		if (n == B) {
			if (triangle % 2 == 0) {
				nParent = Neighbor(R, parent);
				if (nParent == -1) {
					return -1;
				}
				return Child(LEFT, nParent);
			} else {
				nParent = Neighbor(L, parent);
				if (nParent == -1) {
					return -1;
				}
				return Child(RIGHT, nParent);
			}
		} else if (n == L) {
			if (triangle % 2 == 0) {
				nParent = Neighbor(B, parent);
				if (nParent == -1) {
					return -1;
				}
				return Child(LEFT, nParent);
			} else {
				return Child(RIGHT, parent);
			}
		} else if (n == R) {
			if (triangle % 2 == 0) {
				return Child(LEFT, parent);
			} else {
				nParent = Neighbor(B, parent);
				if (nParent == -1) {
					return -1;
				}
				return Child(RIGHT, nParent);
			}
		}
	}
	return 0;
}

void RTIN::ForceSplit(int triangle) {
	int baseNeighbor = Neighbor(B, triangle);
	if (baseNeighbor == -1) { 
		Split(triangle);
	} else if (flags[baseNeighbor]) {
		Split(baseNeighbor);
		Split(triangle);
	} else {
		int baseNeighborParent = Parent(baseNeighbor);
		ForceSplit(baseNeighborParent);
		Split(baseNeighbor);
		Split(triangle);
	}
}

void RTIN::Split(int triangle) {
	flags[triangle] = 0;
	int left = Child(LEFT, triangle);
	int right = Child(RIGHT, triangle);
	flags[left] = 1;
	flags[right] = 1;
}