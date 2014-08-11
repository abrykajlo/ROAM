#include <iostream>
#include <vector>
#include <cmath>
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
	//cout << "Before bind" << endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//cout << "After bind" << endl;
	DrawTriangle(0);
	//cout << "After root recursed" << endl;
}

void RTIN::BuildWedgies() {
	for (int i = 0; i < size; ++i)
	{
		e_T[i] = 0.0; 
		
	}

	int curr_level = MaxLevel-1;

	while(curr_level >= 0) {
		for (int i = pow(2,curr_level)-2; i < pow(2,curr_level+1)-2; ++i)
		{
			float vc = vertexBuffer[indexBuffer[3*(2*i+2)+2]].z; //z(vc)
			float vc_t = (vertexBuffer[indexBuffer[3*i+0]].z+vertexBuffer[indexBuffer[3*i+1]].z)/2; //z_T(vc)
			e_T[i+1] = max(e_T[(2*i)+2], e_T[(2*i)+3]) + abs(vc - vc_t); //i+1 for empty root
		}
		curr_level--;
	}
}

void RTIN::WedgieTreePrint() {
	cout << "----------" << endl;
	for (int i = 0; i <= MaxLevel; ++i)
	{
		for (int j = 0; j < pow(2,MaxLevel-i); ++j)
		{
			cout << " ";
		}
		for (int j = pow(2,i)-1; j < pow(2,i+1)-1; ++j)
		{
			cout << e_T[j];
			for (int k = 0; k < pow(2,MaxLevel-i+2)-pow(2,MaxLevel-i+1)-1; ++k)	cout << " ";
		}
		cout << endl << endl;
	}
	cout << "----------" << endl;
}

void RTIN::Init() {
	//cout << "Enter RTIN init" << endl;

	MaxLevel = 7; //7 for 9x9, 19 for 513x513
	//RITN(9)
	size = 0;
	for (int i = 0; i < MaxLevel; ++i) {
		size += pow(2, i)*2;
	}
	size++; //For empty root
	e_T = new float[size];
	flags = new int[size];

	//cout << "After RTIN constructor" << endl;
	Triangulate();
	//cout << "After Triangulate" << endl;

	BuildWedgies(); // Currently seg faults for too high of levels
	//WedgieTreePrint();	

	glGenBuffers(1, &IBO);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);
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

    //int MaxLevel = 3; //Level 9 results in one vertex per pixel for 513x513

    int vertex_w = pow(2,floor(MaxLevel/2))+1;
    int vertex_h = pow(2,floor(MaxLevel/2))+1;
    int levels = MaxLevel;

    //Calculation of vertices
    for (int i = 0; i < vertex_h; ++i) {
        for (int j = 0; j < vertex_w; ++j) {
            point4 new_vertex;
            new_vertex.x = -1.0+((j/((vertex_w-1)*1.0))*2.0);
            new_vertex.y = -1.0+((i/((vertex_h-1)*1.0))*2.0);
            new_vertex.z = height_vals(new_vertex.x, new_vertex.y); //Height value
            new_vertex.w = 1;
            vertex_list.push_back(new_vertex);
        }
    }
    //cout << "Vertex list populated" << endl;
    //cout << sqrt(vertex_list.size()) << "x" << sqrt(vertex_list.size()) << endl;

    //Root square with two initial triangles:
    face new_face1;
    face new_face2;

    new_face1.x = 0; //top left
    new_face1.y = (vertex_w*vertex_h)-1; //bottom right
    new_face1.z = vertex_w-1; //top right
    
    new_face2.x = (vertex_w*vertex_h)-1; //bottom right
    new_face2.y = 0; //top left
    new_face2.z = (vertex_h-1)*vertex_w; //bottom left

    face_list.push_back(new_face1);
    face_list.push_back(new_face2);

    levels--;

    //Build all sub triangles for all triangles in face list (goes by levels)
    int i = 0;
    for (int currlevel = 1; currlevel <= levels; currlevel++) {
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
    //cout << "Face list populated" << endl;
    //cout << face_list.size() << endl;

    vertexBuffer = new point4[vertex_list.size()];
    for (unsigned int i = 0; i < vertex_list.size(); i++) {
    	vertexBuffer[i] = vertex_list[i];
    }
    //cout << "Vertex buffer created" << endl;

    indexBuffer = new GLuint[face_list.size() * 3];
    for (unsigned int i = 0; i < face_list.size(); i++) {
    	indexBuffer[3*i] = face_list[i].x;
    	indexBuffer[3*i+1] = face_list[i].y;
    	indexBuffer[3*i+2] = face_list[i].z;
    }
    //cout << "Index buffer created" << endl;

    normalBuffer = new vec3[vertex_list.size()];
    for (unsigned int i = 0; i < vertex_list.size(); i++) {
    	normalBuffer[i] = vec3(0.0,0.0,0.0);
    }
    for (unsigned int i = 0; i < face_list.size(); i++) {
    	point4 c = vertex_list[face_list[i].x];
        point4 b = vertex_list[face_list[i].y];
        point4 a = vertex_list[face_list[i].z];
        vec3 normal = normalize( cross(b - a, c - b) );
        normalBuffer[face_list[i].x] += normal;
        normalBuffer[face_list[i].y] += normal;
        normalBuffer[face_list[i].z] += normal;
    }
    for (unsigned int i = 0; i < vertex_list.size(); i++) {
    	normalBuffer[i] = normalize( normalBuffer[i] );
    }
    //cout << "Normal buffer created" << endl;
}

void RTIN::DrawTriangle(int triangle) {
	//if (triangle >= 0) cout << "Draw triangle entered for " << triangle << endl;
	//if (triangle >= 0) cout << "flags[" << triangle << "]: " << flags[triangle] << endl;
	if (flags[triangle] == 1) {
		cout << "Flagged to draw triangle " << triangle << endl;
		int index = ((triangle*3)-3);
		for (int i = 0; i < 3; ++i) cout << "Vertex " << i+1 << ": " << indexBuffer[index+i] << endl;
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,(void*)(sizeof(GLuint)*index));
		cout << "Triangle " << triangle << " drawing complete" << endl;
	} else {
		int left = Child(LEFT, triangle);
		int right = Child(RIGHT, triangle);
		if (left >= 0 && right >= 0) {
			cout << "Considering children " << left << " & " << right << endl;
			DrawTriangle(left);
			DrawTriangle(right);
		}
	}
}