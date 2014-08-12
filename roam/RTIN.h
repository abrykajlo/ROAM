#include <inttypes.h>
#include "include/Angel.h"

typedef Angel::vec4 point4;

enum child {LEFT = 1, RIGHT = 2};

enum neighbor {L, R, B};

//must pass in eye pointer and direction to know error for queues
class RTIN {
public:
	RTIN();
	RTIN(int);
	~RTIN();
	void Draw();
	void DrawWire();
	void DrawEye();
	void WedgieTreePrint();
	void BuildWedgies();
	void SetEye(vec4*, vec4*);
//temporary for unit testing private:
	int Parent(int);
	int Child(child, int);
	int Neighbor(neighbor, int);
	void ForceSplit(int);
	void Split(int);
	void Merge(int);
	void Triangulate(const char *, int);
	void DrawTriangle(int);
	void DrawWireTriangle(int);
	//void MakeErrors(int);
	vec4 * eye_pos;
	vec4 * eye_dir;
	int * flags;
	float * e_T;
	unsigned int * splitQueue;
	unsigned int * mergeQueue;
	int size;
	vec3 * faceNormalBuffer;
	vec3 * vertexNormalBuffer;
	point4 * vertexBuffer;
	GLuint * indexBuffer;
};