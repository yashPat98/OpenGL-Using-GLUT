#include <GL/gl.h>
#include <GL/glut.h>

void SetupRC(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	//Reset Coordination system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//Called to draw scene
void RenderScene(void){
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("Triangle");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();
	glutMainLoop();
	return 0;
}
