#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Eigen/Eigen"
#include "include/Angel.h"
#include "Mesh.h"

using namespace std;

Mesh::Mesh() {
	num_verts = 0;
	num_faces = 0;
	//pointers to zero
	verts = 0;
	faces = 0;
	vert_normals = 0;
	face_normals = 0;
	one_rings = 0;
}

Mesh::~Mesh() {

	if (verts != 0)
		delete [] verts;

	if (faces != 0)
		delete [] faces;

	if (face_normals != 0)
		delete [] face_normals;

	if (vert_normals != 0)
		delete [] vert_normals;

	if (one_rings != 0)
		delete [] one_rings;

	if (vert_flags != 0)
		delete [] vert_flags;

	if (face_flags != 0)
		delete [] face_flags;
}

void Mesh::loadMesh(const char* filename) {
	if (verts != 0) {
		delete [] verts;
	}

	if (vert_normals != 0) {
		delete [] vert_normals;
	}

	if (faces != 0) {
		delete [] faces;
	}

	if (face_normals != 0) {
		delete [] face_normals;
	}

	if (vert_flags != 0) {
		delete [] vert_flags;
	}

	if (face_flags != 0) {
		delete [] face_flags;
	}

	if (one_rings != 0) {
		delete [] one_rings;
	}

	fstream file(filename, ios_base::in);
	//store chars
	char c;
	file >> c;
	file >> num_verts;
	file >> num_faces;

	max_faces = num_faces;
	max_verts = num_verts;
	//allocate space for verts and faces
	verts = new vec4[num_verts];
	faces = new face[num_faces];

	vert_normals = new vec3[num_verts];
	face_normals = new vec3[num_faces];

	vert_flags = new bool[num_verts];
	face_flags = new bool[num_faces];

	//create space for one rings
	one_rings = new vector<int>[num_verts];


	int curr_vert = 0;
	int curr_face = 0;

	file >> c;
	while (!file.eof()) {
		if (c == '#') {
			while (c != '\n') file.get(c);
		} else if (c == 'v') {
			file >> verts[curr_vert].x;
			file >> verts[curr_vert].y;
			file >> verts[curr_vert].z;
			verts[curr_vert].w = 1.0;

			vert_normals[curr_vert].x = 0;
			vert_normals[curr_vert].y = 0;
			vert_normals[curr_vert].z = 0;

			vert_flags[curr_vert] = true;

			curr_vert++;
		} else if (c == 'f') {
			file >> faces[curr_face].i;
			file >> faces[curr_face].j;
			file >> faces[curr_face].k;

			faces[curr_face].i--;
			faces[curr_face].j--;
			faces[curr_face].k--;

			one_rings[faces[curr_face].i].push_back(curr_face);
			one_rings[faces[curr_face].j].push_back(curr_face);
			one_rings[faces[curr_face].k].push_back(curr_face);

			face_normals[curr_face] = cross(verts[faces[curr_face].j] - verts[faces[curr_face].i], 
											verts[faces[curr_face].k] - verts[faces[curr_face].i]);

			vert_normals[faces[curr_face].i] += face_normals[curr_face];
			vert_normals[faces[curr_face].j] += face_normals[curr_face];
			vert_normals[faces[curr_face].k] += face_normals[curr_face];

			face_flags[curr_face] = true;

			curr_face++;
		}
		file >> c;
	}
	file.close();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void Mesh::saveMesh(const char* filename) {
	fstream file( filename, ios_base::out );
	file << "# " << num_verts << " " << num_faces << '\n';
	int j = 0;
	int *corrections = new int[max_verts];
	for (int i = 0; i < max_verts; i++) {
		if (vert_flags[i]) {
			corrections[i] = j;
		} else {
			j++;
		}
	}

	for (int i = 0; i < num_verts; i++) {
		while (!vert_flags[j]) {
			j++;
		}

		file << "v " << verts[j].x << " " << verts[j].y << " " << verts[j].z << '\n';
		j++;
	}

	j = 0;
	for (int i = 0; i < num_faces; i++) {
		while (!face_flags[j]) j++;
		file << "f " << faces[j].i + 1 - corrections[faces[j].i] 
			 << " " << faces[j].j + 1 - corrections[faces[j].j]
			 << " " << faces[j].k + 1 - corrections[faces[j].k] << '\n';
		j++;
	}
	delete [] corrections;
	file.close();
}

void Mesh::drawMesh(int options) {
	vec4 vect;
	if (num_verts != 0) {
		int j = 0; //actually drawn face
		for (int i = 0; i < num_faces; i++) {
			while (!face_flags[j]) j++;
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			if ((options & FLAT) == FLAT) {
				glBegin(GL_TRIANGLES);
					vect = verts[faces[j].i];
					glNormal3f(face_normals[j].x, face_normals[j].y, face_normals[j].z);
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].j];
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].k];
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
				glEnd();
			} else if((options & SMOOTH) == SMOOTH) {
				glBegin(GL_TRIANGLES);
					vect = verts[faces[j].i];
					glNormal3f(vert_normals[faces[j].i].x, vert_normals[faces[j].i].y, vert_normals[faces[j].i].z);
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].j];
					glNormal3f(vert_normals[faces[j].j].x, vert_normals[faces[j].j].y, vert_normals[faces[j].j].z);
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].k];
					glNormal3f(vert_normals[faces[j].k].x, vert_normals[faces[j].k].y, vert_normals[faces[j].k].z);
					glVertex4f(vect.x, vect.y, vect.z, vect.w);			
				glEnd();
			}


			if ((options & WIREFRAME) == WIREFRAME) {
				glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
				glDisable(GL_LIGHTING);
				glBegin(GL_LINE_LOOP);
					vect = verts[faces[j].i];
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].j];
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
					vect = verts[faces[j].k];
					glVertex4f(vect.x, vect.y, vect.z, vect.w);
				glEnd();
				glEnable(GL_LIGHTING);
			}
			j++;
		}
	}
}

void Mesh::decimate(int k, int d) {
	srand(time(NULL));
	vector<edge> *edges; //to use as heap
	//collapse d edges
	edges = new vector<edge>();
	//build heap of random edges
	for (int i = 0; i < k; i++) {
		edge e;
		makeEdge(e);
		edges->push_back(e);
	}

	make_heap( edges->begin(), edges->end(), edgeCompare);

	while (d != 0) {
		//pick k edges and put into heap
		collapse(edges->front());

		pop_heap(edges->begin(), edges->end(), edgeCompare);
		edges->pop_back();

		edge e;
		makeEdge(e);

		edges->push_back(e);
		push_heap(edges->begin(), edges->end(), edgeCompare);


		d--;
	}
	delete edges;
	return;
}

mat4 Mesh::quadric(int v) {
	mat4 result(0.0);
	for (int i = 0; i < one_rings[v].size(); i++) {
		int f = one_rings[v][i];
		
		float plane[4];
		vec3 norm = normalize(face_normals[f]);
		vec4 vert = verts[v];

		plane[0] = norm.x;
		plane[1] = norm.y;
		plane[2] = norm.z;

		float x, y, z;

		x = vert.x;
		y = vert.y;
		z = vert.z;

		plane[3] = -plane[0] * x - plane[1] * y - plane[2] * z;

		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++)
				result[j][k] += plane[j] * plane[k];
		}
	}
	
	return result;
}

void Mesh::collapse(edge& e) {
	int v1 = e.v1, v2 = e.v2;
	vec4 v = e.v;
	vert_flags[v2] = false; //dont need v2 anymore
	verts[v1] = v; //replace v1 with the new v
	
	for (int i = 0; i < one_rings[v2].size(); i++) {
		int f = one_rings[v2][i];
		//check if face belongs to both vertices thus no longer exists
		if (faces[f].i == v1 || faces[f].j == v1 || faces[f].k == v1) {
			face_flags[f] = false;
		} else if (faces[f].i == v2) {
			faces[f].i = v1;
			one_rings[v1].push_back(f);
		} else if (faces[f].j == v2) {
			faces[f].j = v1; // v1 is the new index location
			one_rings[v1].push_back(f);
		} else if (faces[f].k == v2) {
			faces[f].k = v1;
			one_rings[v1].push_back(f);
		}
	}
	// cout << 3 << endl;
	for (int i = 0; i < one_rings[v1].size(); i++) {
		int f = one_rings[v1][i];
		if (faces[f].i == v2 || faces[f].j == v2 || faces[f].k == v2) {
			one_rings[v1].erase(one_rings[v1].begin() + i);
			i--;		
		}
	}

	num_faces -= 2;
	num_verts--;
	return;
}

void Mesh::makeEdge(edge& e) {
	//randomize edges
	int f;
	do {
		f = rand() % max_faces;
	} while (!face_flags[f]);
	//choose one of the faces edges
	int r = rand() % 3;
	if (r == 0) {
		e.v1 = faces[f].i;
		e.v2 = faces[f].j;
	} else if (r == 1) {
		e.v1 = faces[f].j;
		e.v2 = faces[f].k;
	} else if (r == 2) {
		e.v1 = faces[f].k;
		e.v2 = faces[f].i;
	}

	mat4 Q = quadric(e.v1) + quadric(e.v2);
	Eigen::Matrix4d sumQ;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			sumQ(i, j) = Q[i][j];
		}
	}
	sumQ(3,0) = 0;
	sumQ(3,1) = 0;
	sumQ(3,2) = 0;
	sumQ(3,3) = 1;

	float d = sumQ.determinant();
	// cout << d << endl;
	if (d == 0) {
		vec4 mid = verts[e.v1] + verts[e.v2];
		mid /= 2;
		mid.w = 1;
		float error1, error2, error_mid;
		error1 = dot(verts[e.v1], Q * verts[e.v1]);
		error2 = dot(verts[e.v2], Q * verts[e.v2]);
		error_mid = dot(mid, Q * mid);
		
		e.v = verts[e.v1];
		e.error = error1;
		
		if (error2 < e.error) {
			e.v = verts[e.v2];
			e.error = error2;
		}

		if (error_mid < e.error) {
			e.v = mid;
			e.error = error_mid;
		}
	} else {

		Eigen::Vector4d v1(0,0,0,1);
		Eigen::Vector4d vec = sumQ.inverse() * v1;
		e.v.x = vec[0];
		e.v.y = vec[1];
		e.v.z = vec[2];
		e.v.w = vec[3];
		e.error = dot(e.v, Q * e.v);
	}
}

// void Mesh::updateMesh() {
// 	face* new_faces = new face[num_faces];
// 	vec4* new_verts = new vec4[num_verts];
// 	vec3* new_face_normals = new vec3[num_faces];
// 	vec3* new_vert_normals = new vec3[num_verts];
// 	int j = 0;
// 	for (int i = 0; i < max_verts; i++) {
// 		if (vert_flags[i]) {
// 			new_verts[j] = verts[i];
// 			new_vert_normals[j] = vert_normals[i];
// 			for (int k = 0; k < one_rings[i].size(); k++) {
// 				int f = one_rings[i][k];
// 				if (faces[f].i == i) {
// 					faces[f].i = j;
// 				} else if (faces[f].j == i) {
// 					faces[f].i = j;
// 				} else if (faces[f].k == i) {
// 					faces[f].i = j;
// 				}
// 			}
// 			j++;
// 		}
// 	}
// 	j = 0;
// 	for (int i = 0; i < max_faces; i++) {
// 		if (face_flags[i]) {
// 			new_faces[j] = faces[i];
// 			new_face_normals[j] = face_normals[i];
// 			j++;
// 		}
// 	}
// }

bool edgeCompare(edge e1, edge e2) {
	return e2.error < e1.error;
}

int Mesh::getNumVerts() {
	return num_verts;
}

int Mesh::getNumFaces() {
	return num_faces;
}