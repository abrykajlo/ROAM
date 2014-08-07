
enum {LEFT = 1, RIGHT = 2};

enum {L, R, B};

class Terrain {
public:
	Terrain();
	Terrain(int);
	~Terrain();
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