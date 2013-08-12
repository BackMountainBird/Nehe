#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SOIL.h"

#define PI 3.141592654

struct Texture {
	GLuint id;
	int width;
	int height;
	int channel;
};

static boolean lighEnabled = false;

static GLfloat quadXRotate = 180;
static GLfloat quadYRotate = 90;

static GLfloat quadVertices[8][3] = {
	{5,   5, 5},
	{5,  -5, 5},
	{5,  -5,-5},
	{5,   5,-5},
	{-5,  5, 5},
	{-5, -5, 5},
	{-5, -5,-5},
	{-5,  5,-5}
};

static GLubyte quadIndices[6][4] = {
	{0, 1, 2, 3},
	{3, 2, 6, 7},
	{7, 6, 5, 4},
	{4, 5, 1, 0},
	{7, 4, 0, 3},
	{2, 1, 5, 6}
};

static GLfloat cameraPosition[] = {0,0,40};
static GLfloat cameraAngles[] = {0,0};

static GLfloat moveVector[] = {0, 0, 0};

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

void initLight(void) {
	GLfloat lightPos[] = {0, 0, -40, 0.0};
	GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat modelAmbient[] = {0.5, 0.5, 0.5, 1.0}; 
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat matShiness[] = {50.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShiness);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmbient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void initGL(void) {
	initTexture();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.5, 0.7, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initLight();
}

void incAngle(GLfloat& angle) {
	angle = angle + 0.01f;
	if(angle >= 360) {
		angle = 0;
	}
}

void drawQuad() {
	glBindTexture(GL_TEXTURE_2D, textureImage.id);

	glPushMatrix();
	glRotatef((GLfloat) quadXRotate, 1, 0, 0);
	glRotatef((GLfloat) quadYRotate, 0, 1, 0);
	glBegin(GL_QUADS);
	for(GLubyte i=0;i<6;i++){
		glTexCoord2f(0.0, 0.0);glVertex3fv(quadVertices[quadIndices[i][0]]);
		glTexCoord2f(0.0, 1.0);glVertex3fv(quadVertices[quadIndices[i][1]]);
		glTexCoord2f(1.0, 1.0);glVertex3fv(quadVertices[quadIndices[i][2]]);
		glTexCoord2f(1.0, 0.0);glVertex3fv(quadVertices[quadIndices[i][3]]);
	}
	glEnd();
	glPopMatrix();
}

void display(void) {
	GLfloat zeroPos[] = {0, 0, 0, 1};
	GLfloat lightPos[] = {10, 10, 10, 1};
	GLfloat lineColor[] = {1.0, 0, 0, 0};

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-moveVector[0],-moveVector[1],-moveVector[2]);
	glRotatef(cameraAngles[0],0,1,0);
	glRotatef(cameraAngles[1],1,0,0);
	glTranslatef(-cameraPosition[0],-cameraPosition[1], -cameraPosition[2]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xFFFF);
	glLineWidth(2);

	glColor3fv(lineColor);
	glBegin(GL_LINES);
		glVertex4fv(zeroPos);
		glVertex4fv(lightPos);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	drawQuad();

	glFlush();

	incAngle(quadXRotate);
	incAngle(quadYRotate);
	glutPostRedisplay();
}

void keyboard(GLubyte key, int x, int y) {
	switch (key)
	{
	case 'w':
		glMatrixMode(GL_MODELVIEW);
		moveVector[2] += -0.5;
		glutPostRedisplay();
		break;
	case 'a':
		glMatrixMode(GL_MODELVIEW);
		moveVector[0] +=-0.5;
		glutPostRedisplay();
		break;
	case 'd':
		glMatrixMode(GL_MODELVIEW);
		moveVector[0] += 0.5;
		glutPostRedisplay();
		break;
	case 's':
		glMatrixMode(GL_MODELVIEW);
		moveVector[2] += 0.5;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void reshape(int width, int height) {
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, width * 1.0f/ height, 1.0, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static boolean rightMousePressed = false;
static int rightClickInitialPosition[] = {0,0};

void mouse(int button, int state, int x, int y){
	if(button == GLUT_RIGHT_BUTTON){
		if(state == GLUT_DOWN){
			rightMousePressed = true;
			rightClickInitialPosition[0] = x;
			rightClickInitialPosition[1] = y;
		}else{
			rightMousePressed = false;
		}
	}
}

void motion(int x, int y){
	if(rightMousePressed){
		int deltaX = x - rightClickInitialPosition[0];
		int deltaY = - (y - rightClickInitialPosition[1]);

		int width = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);

		GLfloat xAngle = deltaX * 1.0f / width * 180;
		GLfloat yAngle = deltaY * 1.0f / height* 180;

		cameraAngles[0] += xAngle;
		cameraAngles[1] -= yAngle;

		rightClickInitialPosition[0] = x;
		rightClickInitialPosition[1] = y;

		glutPostRedisplay();
	}
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
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return;
}