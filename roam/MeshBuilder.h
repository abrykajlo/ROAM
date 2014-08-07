#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "include/Angel.h"
typedef Angel::vec4 point4;
typedef Angel::vec3 point3;

#include <vector>
using namespace std;

#include "HeightMap.h"

class MeshBuilder {
  public:
  	MeshBuilder();
	int	BuildMesh( const char *, int, float, vector<point4> *, vector<point3> *);
};

#endif