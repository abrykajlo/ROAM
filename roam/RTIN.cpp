#include <iostream>
#include <vector>
#include "RTIN.h"
#include "HeightMap.h"

using namespace std;

RTIN::RTIN() {
	// size = 0;
	// e_T = 0;
	// flags = 0;
	// VBO = 0;
	// IBO = 0;
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

	size = r.size;
	flags = r.flags;
	e_T = r.e_T;

	return *this;
}

void RTIN::Draw() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	DrawTriangle(0);
}

void RTIN::Init() {
	RTIN(9);
	cout << "hello";
	Triangulate();
	glGenBuffers(1, &IBO);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);
}

int RTIN::Parent(int triangle) {
	if (triangle == 0) {
		return -1;
	}
	return (triangle - 1) >> 1;
}

int RTIN::Child(child c, int triangle) {
	int result = (triangle << 1) + c;
	if (result < size) 
		return result;
	else
		return -1;
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

void RTIN::Merge(int triangle) {
	int T_B = Neighbor(B, triangle);
	flags[triangle] = 1;
	flags[T_B] = 1;
	int left = Child(LEFT, triangle);
	int right = Child(RIGHT, triangle);
	flags[left] = 0;
	flags[right] = 0;
	left = Child(LEFT, T_B);
	right = Child(RIGHT, T_B);
	flags[left] = 0;
	flags[right] = 0;
}

typedef struct _face {
		GLuint x, y, z;
	} face;

void RTIN::Triangulate() {
	HeightMap height_vals("heightmap513px.bmp", 0.5);

	vector<point4> vertex_list;
	vector<face> face_list;

    int MaxLevel = 9; //Level 9 results in one vertex per pixel for 513x513

    int granularity = pow(2,MaxLevel);
    int w = granularity+1;
    int h = granularity+1;
    int levels = 1+(2*MaxLevel);

    //Calculation of vertices
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            point4 new_vertex;
            new_vertex.x = -1.0+((j/((w-1)*1.0))*2.0);
            new_vertex.y = -1.0+((i/((h-1)*1.0))*2.0);
            new_vertex.z = height_vals(new_vertex.x, new_vertex.y); //Height value
            new_vertex.w = 1;
            vertex_list.push_back(new_vertex);
        }
    }

    //Root square with two initial triangles:
    face new_face1;
    face new_face2;

    new_face1.x = 0; //top left
    new_face1.y = (w*h)-1; //bottom right
    new_face1.z = w-1; //top right
    
    new_face2.x = (w*h)-1; //bottom right
    new_face2.y = 0; //top left
    new_face2.z = (h-1)*w; //bottom left

    face_list.push_back(new_face1);
    face_list.push_back(new_face2);

    levels--;

    //Build all sub triangles for all triangles in face list (goes by levels)
    int i = 0;
    for (int currlevel = 0; currlevel < levels; currlevel++) {
        int limit = face_list.size();
        while (i < limit) {
            face new_face3;
            face new_face4;

            new_face3.x = face_list[i].z;
            new_face3.y = face_list[i].x;
            new_face3.z = (face_list[i].x + face_list[i].y)/2;
            
            new_face4.x = face_list[i].y;
            new_face4.y = face_list[i].z;
            new_face4.z = (face_list[i].x + face_list[i].y)/2;

            face_list.push_back(new_face3);
            face_list.push_back(new_face4);

            i++;
        }
    }
    vertexBuffer = new point4[vertex_list.size()];
    for (int i = 0; i < vertex_list.size(); i++) {
    	vertexBuffer[i] = vertex_list[i];
    }
    indexBuffer = new GLuint[face_list.size() * 3];
    normalBuffer = new vec3[vertex_list.size()];

    for (int i = 0; i < vertex_list.size(); i++) {
    	normalBuffer[i] = vec3(0.0,0.0,0.0);
    }

    for (int i = 0; i < face_list.size(); i++) {
    	indexBuffer[3*i] = face_list[i].x;
    	indexBuffer[3*i+1] = face_list[i].y;
    	indexBuffer[3*i+2] = face_list[i].z;
    	point4 c = vertex_list[face_list[i].x];
        point4 b = vertex_list[face_list[i].y];
        point4 a = vertex_list[face_list[i].z];
        vec3 normal = normalize( cross(b - a, c - b) );
        normalBuffer[(i*3)] += normal;
        normalBuffer[(i*3)+1] += normal;
        normalBuffer[(i*3)+2] += normal;
    }

    for (int i = 0; i < vertex_list.size(); i++) {
    	normalBuffer[i] = normalize( normalBuffer[i] );
    }
}

void RTIN::DrawTriangle(int triangle) {
	if (flags[triangle] == 1) {
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,(void*)(sizeof(GLuint)*(triangle-1)));
	} else {
		int left = Child(LEFT, triangle);
		int right = Child(RIGHT, triangle);
		DrawTriangle(left);
		DrawTriangle(right);
	}
}