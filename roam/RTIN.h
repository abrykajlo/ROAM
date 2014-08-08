#include "include/Angel.h"

enum child {LEFT = 1, RIGHT = 2};

enum neighbor {L, R, B};

class RTIN {
public:
	RTIN();
	RTIN(int);
	~RTIN();
	void Draw();
	void Test();
private:
	int Parent(int);
	int Child(int, int);
	int Neighbor(int, int);
	int * flags;
	float * e_T;
	int size;
	GLuint VBO;
	GLuint IBO;
};