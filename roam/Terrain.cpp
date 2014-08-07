#include <iostream>
#include "Terrain.h"

using namespace std;

Terrain::Terrain() {
	size = 0;
	e_T = 0;
	flags = 0;
	VBO = 0;
	IBO = 0;
}

void Terrain::Test() {
//Test for class
}

Terrain::Terrain(int levels) {
	size = ( 2 << (levels + 1) ) - 1;
	e_T = new float[size];
	flags = new int[size];
}

Terrain::~Terrain() {
	if (e_T != 0) {
		delete [] e_T;
	}
	if (flags != 0) {
		delete [] flags;
	}
}

int Terrain::Parent(int node) {
	return (node - 1) >> 1;
}