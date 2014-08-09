#include <array>
#include <inttypes.h>
#include "include/Angel.h"

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
//temporary for unit testing private:
	int Parent(int);
	int Child(child, int);
	int Neighbor(neighbor, int);
	void ForceSplit(int);
	void Split(int);
	void Merge(int);
	void BuildTriangulation();
	int * flags;
	float * e_T;
	unsigned int * splitQueue;
	unsigned int * mergeQueue;
	int size;
	//point4 * vertexBuffer;
	//point3 * indexBuffer;
	GLuint VBO;
	GLuint IBO;
};