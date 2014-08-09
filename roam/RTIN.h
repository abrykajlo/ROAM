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
	int ForceSplit(int);
	int * flags;
	float * e_T;
	int size;
	GLuint VBO;
	GLuint IBO;
};