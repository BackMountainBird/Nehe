#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include "SOIL.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

struct Texture {
	GLuint id;
	int width;
	int height;
	int channel;
};

struct Vector4 {
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};

struct Face {
public:
	GLuint vxIndex;
	GLuint vyIndex;
	GLuint vzIndex;

	GLuint txIndex;
	GLuint tyIndex;
	GLuint tzIndex;

	GLuint nxIndex;
	GLuint nyIndex;
	GLuint nzIndex;
};

struct MeshData {
	vector<Vector4> pVertices;
	vector<Vector4> tVertices;
	vector<Vector4> nVertices;

	vector<Face> faces;
} meshData;

static Texture textureImage;

void initTexture() {
	unsigned char * image  = SOIL_load_image("texture.png", &textureImage.width, &textureImage.height, &textureImage.channel, SOIL_LOAD_AUTO);
	glGenTextures(1, &textureImage.id);

	printf("width:%d,height:%d,channel:%d",textureImage.width, textureImage.height, textureImage.channel);

	glBindTexture(GL_TEXTURE_2D, textureImage.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,textureImage.width,textureImage.height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
}

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
			meshData.pVertices.push_back(v);
		} else if (line.substr(0, 2) == "f ") {
			string buf;
			istringstream s(line.substr(2));
			vector<string> tokens; // Create vector to hold our 
			while (s >> buf){
				tokens.push_back(buf);
			}

			Face f;
			sscanf(tokens.at(0).c_str(), "%d/%d/%d", &f.vxIndex, &f.txIndex, &f.nxIndex);
			sscanf(tokens.at(1).c_str(), "%d/%d/%d", &f.vyIndex, &f.tyIndex, &f.nyIndex);
			sscanf(tokens.at(2).c_str(), "%d/%d/%d", &f.vzIndex, &f.tzIndex, &f.nzIndex);

			meshData.faces.push_back(f);

			printf("Face(v(%d,%d,%d),t(%d,%d,%d),n(%d,%d,%d))\n",
				f.vxIndex, f.vyIndex, f.vzIndex,
				f.vyIndex, f.tyIndex, f.tzIndex,
				f.vzIndex, f.nyIndex, f.nzIndex
			);
		} else if (line.substr(0, 3) == "vn ") {
			istringstream s(line.substr(2));
			Vector4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0f;
			meshData.nVertices.push_back(v);
		} else if (line.substr(0, 3) == "vt ") {
			istringstream s(line.substr(2));
			Vector4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0f;
			meshData.tVertices.push_back(v);
		} 
	}
	printf("vertex size: %d, triangle size: %d" , meshData.pVertices.size(), meshData.faces.size());
}

void initGL(void) {
	initTexture();

	glClearColor(0.3, 0.5, 0.7, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat lightPosition[] = {2,2,2};
	GLfloat whiteLight[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR, whiteLight);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.2);

	GLfloat spotDirection[] = {-2,-2,-2};

	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void drawVertex(Vector4 pVertex, Vector4 tVertex, Vector4 nVertex) {
	glTexCoord3f(tVertex.x, tVertex.y, tVertex.z);
	glNormal3f(nVertex.x, nVertex.y, nVertex.z);
	glVertex4f(pVertex.x, pVertex.y, pVertex.z, pVertex.w);
}

void drawTriangle(Face face, vector<Vector4> pVertices, vector<Vector4> tVertices, vector<Vector4> nVertices){
	drawVertex(pVertices.at(face.vxIndex - 1), tVertices.at(face.txIndex - 1), nVertices.at(face.nxIndex - 1));
	drawVertex(pVertices.at(face.vyIndex - 1), tVertices.at(face.tyIndex - 1), nVertices.at(face.nyIndex - 1));
	drawVertex(pVertices.at(face.vzIndex - 1), tVertices.at(face.tzIndex - 1), nVertices.at(face.nzIndex - 1));
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat specular[] = {1.0,0.0,0.0,1.0};
	GLfloat emission[] = {0.0,1.0,0.0,1.0};
	GLfloat ambient[] = {0.0,0.0,1.0,1.0};
	GLfloat shiness[] = {50.0};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureImage.id);
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, shiness);
		glBegin(GL_TRIANGLES);
		for(vector<Face>::iterator iterator = meshData.faces.begin();iterator!=meshData.faces.end();iterator++){
			drawTriangle(*iterator, meshData.pVertices, meshData.tVertices, meshData.nVertices);
		}
		glEnd();
	glPopMatrix();

	glFlush();
}

void reshape(int width, int height) {
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-1,1,-1,1);
	gluPerspective(70, width * 1.0f/ height, 1.0, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0, -5,0,0,0,0,1,0);
}

void main(int argc, char* argv[]) {
	loadObj("apple_bowl.data");

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