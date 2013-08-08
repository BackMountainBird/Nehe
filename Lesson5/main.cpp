#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>

static GLfloat triangleRotate = 0;
static GLfloat quadRotate = 0;

static GLfloat triangleVertices[] = {
	0, 2.5, 0,
	0, -2.5, 2.5,
	2.5, -2.5,-2.5,
	-2.5, -2.5, -2.5
};

static GLfloat triangleColors[] = {
	1.0,  0,   0,
	0,    1.0, 0,
	0,	  0,   1.0,
	0,    0,   1.0
};

static GLubyte triangleIndices[4][3] = {
	{0,1,2},
	{0,2,3},
	{0,3,1},
	{1,2,3}
};

void initGL(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void incAngle(GLfloat& angle) {
	angle = angle + 0.01f;
	if(angle >= 360) {
		angle = 0;
	}
}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3,GL_FLOAT,0,triangleColors);
	glVertexPointer(3, GL_FLOAT, 0, triangleVertices);
	glPushMatrix();
	glTranslatef(-5,0,0);
	glRotatef((GLfloat) triangleRotate, 0, 1 ,0);
		for(int i=0;i<4;i++){
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, triangleIndices[i]);
		}
	glPopMatrix();

	glShadeModel(GL_FLAT);
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
	glRotatef(quadRotate,1,0,0);
		glBegin(GL_POLYGON);
			glVertex3f(2.5, -2.5,0);
			glVertex3f(2.5, 2.5, 0);
			glVertex3f(7.5, 2.5, 0);
			glVertex3f(7.5, -2.5,0);
		glEnd();
	glPopMatrix();
	glFlush();

	incAngle(triangleRotate);
	incAngle(quadRotate);
	glutPostRedisplay();
}

void reshape(int width, int height) {
	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f,-10.0f,10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(200,200);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Nehe");
	initGL();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return;
}