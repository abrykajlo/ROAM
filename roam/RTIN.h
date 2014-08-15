#include <vector>
#include <inttypes.h>
#include "include/Angel.h"

using namespace std;

typedef struct priority {
	int triangle;
	float * p;
	priority(int, float*);
} priority;

enum child {LEFT = 1, RIGHT = 2};

enum neighbor {L, R, B};

//must pass in eye pointer and direction to know error for queues
class RTIN {
public:
	RTIN();
	RTIN(vec4*, vec4*);
	~RTIN();
	void Draw();
	void DrawWire();
	void DrawEye();
	void SetEye(vec4*, vec4*);
	//for Triangulating the mesh
	void Triangulate(const char *, int, vec4*, vec4*);
	void Update();
private:
	int Parent(int);
	int Child(child, int);
	int Neighbor(neighbor, int);
	
	void ForceSplit(int);
	void Split(int);
	void Merge(int);
	
	void SetPriority(int);
	float * priorities;
	vector<priority> splitQueue; //stores triangles to split
	vector<priority> mergeQueue; //stores mergable diamonds

	//private draw functions
	void DrawTriangle(int);
	void DrawWireTriangle(int);
	
	void BuildWedgies();

	void MergeQueueRemove(int);
	void SplitQueueRemove(int);

	unsigned int frame; //what frame are we on should increment every draw

	vec4 * eye_pos;
	vec4 * eye_dir;

	int * flags;
	float * e_T;
	int size;
	int triangles;
	int target;
	vec3 * faceNormalBuffer;
	vec3 * vertexNormalBuffer;
	vec4 * vertexBuffer;
	GLuint * indexBuffer;
};