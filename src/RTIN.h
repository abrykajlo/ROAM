#include <boost/qvm/vec.hpp>
#include <GL/glew.h>

#include <vector>

using namespace std;
using namespace boost::qvm;

typedef struct priority {
	int triangle;
	float * p;
	priority(int, float*);
} priority;

//vector definitions
typedef vec<GLfloat, 3> vec3;
typedef vec<GLfloat, 4> vec4;
typedef vec<GLuint, 3> triangle;

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

	vector<int> flags;
	vector<float> e_T;
	int size;
	int triangles;
	int target;
	vector<vec3> faceNormalBuffer;
	vector<vec3> vertexNormalBuffer;
	vector<vec4> vertexBuffer;
	vector<triangle> triangleBuffer;
};