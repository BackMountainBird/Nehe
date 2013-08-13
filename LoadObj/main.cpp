#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Vector4 {
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};

struct Face {
public:
	GLuint aIndex;
	GLuint bIndex;
	GLuint cIndex;
};

struct MeshData {
	vector<Vector4> vectors;
	vector<Face> faces;
} meshData;

void loadObj(const char * fileName) {
	ifstream in(fileName, ios::in);
	if(!in) {
		cout << "fail" << endl;
		return ;
	}

	string line;
	while(getline(in, line)) {
		if(line.substr(0, 2) == "v ") {
			istringstream s(line.substr(2));
			Vector4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0f;
			meshData.vectors.push_back(v);
		} else if (line.substr(0, 2) == "f ") {
			istringstream s(line.substr(2));
			Face f;
			s >> f.aIndex;
			s >> f.bIndex;
			s >> f.cIndex;
			meshData.faces.push_back(f);
		}
	}
}

void initGL(void) {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.5, 0.7, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display(void) {

}

void reshape(int width, int height) {
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, width * 1.0f/ height, 1.0, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(200,200);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Nehe");
	initGL();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return;
}