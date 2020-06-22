#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#define GL_PI 3.1415f

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

	if(h == 0)
		h = 1;

	glViewport(0 , 0, w, h);

	//Reset Coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w <= h)
		glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -100.0f, 100.0f);
	else
		glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Called to Draw scene
void RenderScene(void){
	GLfloat x, y, z, angle;

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);


	//Lines are in X-Y plane
	z = 0.0f;

	glBegin(GL_LINES);
	for(angle = 0.0f; angle <= GL_PI; angle += (GL_PI/10.0f)){
		//Top half of circle
		x = 50 * sin(angle);
		y = 50 * cos(angle);
		glVertex3f(x, y, z);

		//Bottom half of circle
		x = 50 * sin(angle + GL_PI);
		y = 50 * cos(angle + GL_PI);
		glVertex3f(x, y, z);
	}
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}

void SetupRC(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("Fanned Lines");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();
	glutMainLoop();
	return 0;
}
