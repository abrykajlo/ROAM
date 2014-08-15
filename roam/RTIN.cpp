#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "RTIN.h"
#include "HeightMap.h"

#define TRIANGLE_COUNT 1000

using namespace std;



//isnt exactly safe but is the fastest hack
bool MaxPriority(priority p1, priority p2) {
	return *p1.p < *p2.p;
}

bool MinPriority(priority p1, priority p2) {
	return *p2.p < *p1.p;
}

priority::priority(int _t, float * _p) {
	triangle = _t;
	p = _p;
	//cout << "Triangle " << triangle << " with priority " << *p << endl;
}

RTIN::RTIN() {
	size = 0;
	e_T = 0;
	flags = 0;
	vertexNormalBuffer = 0;
	faceNormalBuffer = 0;
	vertexBuffer = 0;
	indexBuffer = 0;
	priorities = 0;
	frame = 0;
}

RTIN::RTIN(vec4* ep, vec4* ed) {
	eye_dir = ed;
	eye_pos = ep;
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

void RTIN::Draw() {
	DrawTriangle(0);
	//cout << "After root recursed" << endl;
}

void RTIN::Update() {
	if (frame == 0) {
		priorities = new float[size - 1];
		SetPriority(1);
		SetPriority(2);
		triangles = 2;
		flags[1] = 1;
		flags[2] = 1;
		splitQueue.push_back(priority(1, priorities));
		splitQueue.push_back(priority(2, priorities+1));
		make_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
	} else {
		for (int i = 0; i < splitQueue.size(); i++) {
			SetPriority(splitQueue[i].triangle);
		}
		make_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
		for (int i = 0; i < mergeQueue.size(); i++) {
			int triangle = mergeQueue[i].triangle;
			int T_B = Neighbor(B, triangle);
			SetPriority(triangle);
			SetPriority(T_B);
			if (priorities[T_B - 1] > priorities[triangle - 1]) {
				mergeQueue[i] = priority(T_B, &priorities[T_B - 1]);
			}
		}
		make_heap(splitQueue.begin(), splitQueue.end(), MinPriority);
	}
	float max = 0, min = 0;
	if (!splitQueue.empty()) max = *splitQueue[0].p;
	if (!mergeQueue.empty()) min = *mergeQueue[0].p;
	//cout << max << " " << min << endl;
	//cout << "Triangle count " << triangles << endl;
	int counter = 0;
	while (max > min || triangles != target) {
		//cout << max << " " << min << endl;
		// cout << triangles << endl;
		if (triangles > target) {
			if (mergeQueue.empty()) break;
			cout << "here" << endl;
			Merge(mergeQueue[0].triangle);
			pop_heap(mergeQueue.begin(), mergeQueue.end(), MinPriority);
			cout << "after" << endl;
			mergeQueue.pop_back();
			// cout << "Merge" << endl;
			if (mergeQueue.empty()) 
				break;
			else 
				min = *mergeQueue[0].p;
		} else {
			int triangle = splitQueue[0].triangle;
			ForceSplit(triangle);
			int baseNeighbor = Neighbor(B, triangle);
			if (baseNeighbor != -1) {
				SetPriority(triangle);
				SetPriority(baseNeighbor);
				if (priorities[baseNeighbor - 1] < priorities[triangle - 1]) {
					mergeQueue.push_back(priority(baseNeighbor, &priorities[baseNeighbor - 1]));
				} else {
					mergeQueue.push_back(priority(triangle, &priorities[triangle - 1]));
				}
				make_heap(mergeQueue.begin(), mergeQueue.end(), MinPriority);
				min = *mergeQueue[0].p;
			}
			make_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
			max = *splitQueue[0].p;
			//cout << "Split " << counter << endl;
			if (splitQueue.empty()) 
				break;
			else if (!splitQueue.empty())
			 	max = *splitQueue[0].p;
		}
		//cout << "before break" << endl;
		//cout << counter;
		if (counter == 1000) break;
		counter++;
	}
	// for (int i = 0; i < 10; i++) {
	// 	if (splitQueue.empty()) 
	// 			break;
	// 	int triangle = splitQueue[0].triangle;
	// 		ForceSplit(triangle);
	// 		int baseNeighbor = Neighbor(B, triangle);
	// 		if (baseNeighbor != -1) {
	// 			SetPriority(triangle);
	// 			SetPriority(baseNeighbor);
	// 			if (priorities[baseNeighbor - 1] < priorities[triangle - 1]) {
	// 				mergeQueue.push_back(priority(baseNeighbor, &priorities[baseNeighbor - 1]));
	// 			} else {
	// 				mergeQueue.push_back(priority(triangle, &priorities[triangle - 1]));
	// 			}
	// 			make_heap(mergeQueue.begin(), mergeQueue.end(), MinPriority);
	// 			min = *mergeQueue[0].p;
	// 		}
	// 		make_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
	// 		max = *splitQueue[0].p;
	// }
	//cout << "end while" << endl;
	frame++;
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
	if (Child(LEFT, triangle) != -1 && flags[triangle] == 1) {
		flags[triangle] = 0;
		int left = Child(LEFT, triangle);
		int right = Child(RIGHT, triangle);
		flags[left] = 1;
		flags[right] = 1;
		for (int i = 0; i < splitQueue.size(); i++) {
			if (splitQueue[i].triangle == triangle) {
				//cout << "triangle " << triangle << " removed" << endl;
				splitQueue.erase(splitQueue.begin() + i);
				break;
			}
		}
		SetPriority(left);
		SetPriority(right);
		splitQueue.push_back(priority(left, &priorities[left - 1]));
		splitQueue.push_back(priority(right, &priorities[right - 1]));
		triangles++;
	}
}

void RTIN::Merge(int triangle) {
	int T_B = Neighbor(B, triangle);
	if (!flags[Child(LEFT, triangle)] || !flags[Child(RIGHT, triangle)] ||
		!flags[Child(LEFT, T_B)] || !flags[Child(LEFT, T_B)]) return;
	flags[triangle] = 1;
	flags[T_B] = 1;
	SetPriority(triangle);
	SetPriority(T_B);
	splitQueue.push_back(priority(triangle, &priorities[triangle-1]));
	push_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
	splitQueue.push_back(priority(T_B, &priorities[T_B-1]));
	push_heap(splitQueue.begin(), splitQueue.end(), MaxPriority);
	int left = Child(LEFT, triangle);
	int right = Child(RIGHT, triangle);
	flags[left] = 0;
	flags[right] = 0;
	left = Child(LEFT, T_B);
	right = Child(RIGHT, T_B);
	flags[left] = 0;
	flags[right] = 0;
	for (int i = 0; i < mergeQueue.size(); i++) {
		if (mergeQueue[i].triangle == triangle) {
			mergeQueue.erase(mergeQueue.begin() + i);
			break;
		}
	}
	int parent = Parent(triangle);
	int baseNeighbor = Neighbor(B, parent);
	if (flags[Child(LEFT, parent)] && flags[Child(RIGHT, parent)] &&
		flags[Child(LEFT, baseNeighbor)] && flags[Child(RIGHT, baseNeighbor)]) {
		SetPriority(parent);
		SetPriority(baseNeighbor);
		float * p = (priorities[baseNeighbor - 1] < priorities[parent - 1]) ? 
					&priorities[parent - 1] : &priorities[baseNeighbor - 1]; 
		mergeQueue.push_back(priority(triangle, p));
	}
	triangle = Neighbor(B, triangle);
	parent = Parent(triangle);
	baseNeighbor = Neighbor(B, parent);
	if (flags[Child(LEFT, parent)] && flags[Child(RIGHT, parent)] &&
		flags[Child(LEFT, baseNeighbor)] && flags[Child(RIGHT, baseNeighbor)]) {
		SetPriority(parent);
		SetPriority(baseNeighbor);
		float * p = (priorities[baseNeighbor - 1] < priorities[parent - 1]) ? 
					&priorities[parent - 1] : &priorities[baseNeighbor - 1]; 
		mergeQueue.push_back(priority(triangle, p));
	}
	triangles -= 2;

}

void RTIN::DrawWire() {
	glColor4f(0.0, 0.0, 0.0, 1.0);
	DrawWireTriangle(0);
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



void RTIN::Triangulate(const char * filename, int levels, vec4 *ep, vec4 *ed) {
	size = (2 << levels) - 1;
	target = size - (2 << (levels - 1));
	target /= 2;
	if (target > 1000) target = 1000;
	if (levels <= 4) target = size - 3;
	flags = new int[size];
	e_T = new float[size];
	faceNormalBuffer = new vec3[size - 1];
	vertexNormalBuffer = new vec3[4+(size-3)];
	vertexBuffer = new vec4[4+(size-3)];
	indexBuffer = new GLuint[3 * (size - 1)];
	HeightMap z(filename, 0.5);
	
	vertexBuffer[0] = vec4(-1.0, 1.0, 0.0, 1.0);
	vertexBuffer[1] = vec4(1.0, 1.0, 0.0, 1.0);
	vertexBuffer[2] = vec4(-1.0, -1.0, 0.0, 1.0);
	vertexBuffer[3] = vec4(1.0, -1.0, 0.0, 1.0);

	vertexBuffer[0].z = z(vertexBuffer[0].x, vertexBuffer[0].y);
	vertexBuffer[1].z = z(vertexBuffer[1].x, vertexBuffer[1].y);
	vertexBuffer[2].z = z(vertexBuffer[2].x, vertexBuffer[2].y);
	vertexBuffer[3].z = z(vertexBuffer[3].x, vertexBuffer[3].y);

	indexBuffer[0] = 3;
	indexBuffer[1] = 0;
	indexBuffer[2] = 2;

	//cout << indexBuffer[0] << " " << indexBuffer[1] << " " << indexBuffer[2] << endl;

	indexBuffer[3] = 0;
	indexBuffer[4] = 3;
	indexBuffer[5] = 1;

	//cout << indexBuffer[3] << " " << indexBuffer[4] << " " << indexBuffer[5] << endl;

	int v = 4;
	for (int t = 1; t < size; t++) {
		if (Child(LEFT, t) == -1) break;
		int i = 3 * (t - 1);
		vertexBuffer[v] = vertexBuffer[indexBuffer[i]] + vertexBuffer[indexBuffer[i + 1]];
		vertexBuffer[v] = vertexBuffer[v] / 2;
		vertexBuffer[v].z = z(vertexBuffer[v].x, vertexBuffer[v].y);
		
		int left = Child(LEFT, t);
		int right = Child(RIGHT, t);

		int j = 3 * (left - 1);

		indexBuffer[j] = indexBuffer[i + 2];
		indexBuffer[j + 1] = indexBuffer[i];
		indexBuffer[j + 2] = v;

		//cout << indexBuffer[j] << " " << indexBuffer[j+1] << " " << indexBuffer[j+2] << endl;

		j = 3 * (right - 1);

		indexBuffer[j] = indexBuffer[i + 1];
		indexBuffer[j + 1] = indexBuffer[i + 2];
		indexBuffer[j + 2] = v; 
		//cout << indexBuffer[j] << " " << indexBuffer[j+1] << " " << indexBuffer[j+2] << endl;
		v++;
	}

	for (int i = 0; i < size - 1; i++) {
		vec4 a = vertexBuffer[indexBuffer[3 * i]];
		vec4 b = vertexBuffer[indexBuffer[3 * i + 1]];
		vec4 c = vertexBuffer[indexBuffer[3 * i + 2]];
		faceNormalBuffer[i] = normalize( cross( (b - a), (c - a) ) );
	}

	for (int t = 1; t < size; t++) {
		if (Child(LEFT, t) != -1) continue;
		int index = 3 * (t - 1);
		vertexNormalBuffer[indexBuffer[index]] += faceNormalBuffer[t - 1];
		vertexNormalBuffer[indexBuffer[index + 1]] += faceNormalBuffer[t - 1];
		vertexNormalBuffer[indexBuffer[index + 2]] += faceNormalBuffer[t - 1];
	}
	priorities = new float[size - 1];
	BuildWedgies();
	eye_pos = ep;
	eye_dir = ed;

	// for (int i = 0; i < size; i++) {
	// 	if (Child(LEFT, i) != -1) continue;
	// 	flags[i] = 1;
	// }
}

void RTIN::DrawTriangle(int triangle) {
	//if (triangle >= 0) cout << "Draw triangle entered for " << triangle << endl;
	//if (triangle >= 0) cout << "flags[" << triangle << "]: " << flags[triangle] << endl;
	if (flags[triangle] == 1) {
		int i = 3 * (triangle - 1);
		int index;
		vec4 vect;
		vec3 norm;
		glBegin(GL_TRIANGLES);
			glColor4f(0.5,0.5,0.5,1.0);
			index = indexBuffer[i];
			vect = vertexBuffer[index]; i++;
			norm = faceNormalBuffer[triangle - 1];
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
			index = indexBuffer[i];
			vect = vertexBuffer[index]; i++;
			norm = faceNormalBuffer[triangle - 1];
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
			index = indexBuffer[i];
			vect = vertexBuffer[index];
			norm = faceNormalBuffer[triangle - 1];
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
		glEnd();
	} else {
		int left = Child(LEFT, triangle);
		int right = Child(RIGHT, triangle);
		if (left >= 0 && right >= 0) {
			//cout << "Consider children: " << left << " & " << right << endl;
			DrawTriangle(left);
			DrawTriangle(right);
		}
	}
}

void RTIN::DrawWireTriangle(int triangle) {
	if (flags[triangle] == 1) {
		int i = 3 * (triangle - 1);
		int index;
		vec4 vect;
		glBegin(GL_LINE_LOOP);
			index = indexBuffer[i];
			vect = vertexBuffer[index]; i++;
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
			index = indexBuffer[i];
			vect = vertexBuffer[index]; i++;
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
			index = indexBuffer[i];
			vect = vertexBuffer[index];
			glVertex4f(vect.x, vect.y, vect.z, 1.0);
		glEnd();
	} else {
		int left = Child(LEFT, triangle);
		int right = Child(RIGHT, triangle);
		if (left >= 0 && right >= 0) {
			//cout << "Consider children: " << left << " & " << right << endl;
			DrawWireTriangle(left);
			DrawWireTriangle(right);
		}
	}
}

void RTIN::BuildWedgies() {
	for (int t = size-1; 0 < t; t--)
	{
		if (Child(LEFT, t) == -1) {
			e_T[t] = 0.0;
		} else {
			int left = Child(LEFT, t);
			int right = Child(RIGHT, t);
			float vc = vertexBuffer[indexBuffer[3*(left - 1) + 2]].z; //z(vc)
			float vc_t = (vertexBuffer[indexBuffer[3*(t - 1)]].z+vertexBuffer[indexBuffer[3*(t - 1)+1]].z)/2; //z_T(vc)
			e_T[t] = max(e_T[left], e_T[right]) + abs(vc - vc_t); //i+1 for empty root
		}
		
	}
}

void RTIN::DrawEye() {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex4f(eye_pos->x, eye_pos->y, eye_pos->z, 1.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex4f(eye_pos->x, eye_pos->y, eye_pos->z, 1.0);
		glVertex4f(eye_pos->x + eye_dir->x, eye_pos->y + eye_dir->y, eye_pos->z + eye_dir->z, 1.0);
	glEnd();
}

void RTIN::SetEye(vec4 *ep, vec4 *ed) {
	eye_pos = ep;
	eye_dir = ed;
}

void RTIN::SetPriority(int triangle) {
	mat4 cameraSpace = LookAt(*eye_pos, (*eye_pos)+(*eye_dir), vec4(0.0, 0.0, 1.0, 1.0));
	vec4 abc = cameraSpace * vec4(0.0, 0.0, e_T[triangle - 1], 1.0);

	int i = 3 * (triangle - 1);

	vec4 pqr;
	
	float max, min, maxTemp, minTemp;

	for (int n = 0; n < 3; n++) {
		pqr = cameraSpace * vertexBuffer[indexBuffer[i + n]];

		maxTemp = pow(abc.x * pqr.z - abc.z * pqr.x, 2.0) + pow(abc.y * pqr.z - abc.z * pqr.y, 2.0);
		minTemp = pow(pqr.z, 2.0) - pow(abc.z, 2.0);
		if (n == 0) {
			max = maxTemp;
			min = minTemp;
		} else {
			if (max < maxTemp) max = maxTemp;
			if (minTemp < min) min = minTemp;
		}
	}
	if (min <= 0) 
		priorities[triangle - 1] = 1.7e+38 * sqrt(max);
	else
		priorities[triangle - 1] = 2 / min * sqrt(max);
	//cout << "Priority " << triangle << " = " << priorities[triangle - 1] << endl;
}