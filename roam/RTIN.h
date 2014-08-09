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
	RTIN &operator=(RTIN);
	void Draw();
	void Init();
//temporary for unit testing private:
	int Parent(int);
	int Child(child, int);
	int Neighbor(neighbor, int);
	void ForceSplit(int);
	void Split(int);
	void Merge(int);
	void Triangulate();
	void DrawTriangle(int);
	int * flags;
	float * e_T;
	unsigned int * splitQueue;
	unsigned int * mergeQueue;
	int size;
	vec3 * normalBuffer;
	point4 * vertexBuffer;
	GLuint * indexBuffer;
	GLuint VBO;
	GLuint IBO;
};