#include "include/Angel.h"

#include "HeightMap.h"

#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <vector>

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec3 point3;
typedef Angel::vec4 color4;

vector<point4> vertex_list;
vector<point3> face_list;

int main(int argc, char const *argv[])
{

    int w = 2;
    int h = 2;

        if (w == 2 || h == 2) {
            for (int i = 0; i < (h-1); ++i) {
                for (int j = 0; j < (w-1); ++j) {
                    point3 new_face1;
                    point3 new_face2;

                    new_face1.x = (i*w)+(j+0); //0
                    new_face1.y = ((i+1)*w)+(j+1); //3
                    new_face1.z = (i*w)+(j+1); //1
                    
                    new_face2.x = ((i+1)*w)+(j+1); //3
                    new_face2.y = (i*w)+(j+0); //0
                    new_face2.z = ((i+1)*w)+(j+0); //2

                    face_list.push_back(new_face1);
                    face_list.push_back(new_face2);
                }
            }
        }

        else if (w%2 == 1 || h%2 == 1){
            for (int i = 0; i < (h-1); i+=2) {
                for (int j = 0; j < (w-1); j+=2) {
                    point3 new_face1;
                    point3 new_face2;
                    point3 new_face3;
                    point3 new_face4;
                    point3 new_face5;
                    point3 new_face6;
                    point3 new_face7;
                    point3 new_face8;


                    new_face1.x = (i*w)+(j+0); //0
                    new_face1.y = ((i+1)*w)+(j+1); //4
                    new_face1.z = (i*w)+(j+1); //1
                    
                    new_face2.x = ((i+1)*w)+(j+1); //4
                    new_face2.y = (i*w)+(j+0); //0
                    new_face2.z = ((i+1)*w)+(j+0); //3


                    new_face3.x = ((i+1)*w)+(j+1); //4
                    new_face3.y = (i*w)+(j+2); //2
                    new_face3.z = (i*w)+(j+1); //1

                    new_face4.x = (i*w)+(j+2); //2
                    new_face4.y = ((i+1)*w)+(j+1); //4
                    new_face4.z = ((i+1)*w)+(j+2); //5


                    new_face5.x = ((i+2)*w)+(j+0); //6
                    new_face5.y = ((i+1)*w)+(j+1); //4
                    new_face5.z = ((i+1)*w)+(j+0); //3

                    new_face6.x = ((i+1)*w)+(j+1); //4
                    new_face6.y = ((i+2)*w)+(j+0); //6
                    new_face6.z = ((i+2)*w)+(j+1); //7


                    new_face7.x = ((i+1)*w)+(j+1); //4
                    new_face7.y = ((i+2)*w)+(j+2); //8
                    new_face7.z = ((i+1)*w)+(j+2); //5

                    new_face8.x = ((i+2)*w)+(j+2); //8
                    new_face8.y = ((i+1)*w)+(j+1); //4
                    new_face8.z = ((i+2)*w)+(j+1); //7

                    face_list.push_back(new_face1);
                    face_list.push_back(new_face2);
                    face_list.push_back(new_face3);
                    face_list.push_back(new_face4);
                    face_list.push_back(new_face5);
                    face_list.push_back(new_face6);
                    face_list.push_back(new_face7);
                    face_list.push_back(new_face8);
                }
            }
        }

    for (int i = 0; i < face_list.size(); ++i)
    {
        cout << face_list[i].x << " " << face_list[i].y << " " << face_list[i].z << endl; 
    }

    return 0;
}