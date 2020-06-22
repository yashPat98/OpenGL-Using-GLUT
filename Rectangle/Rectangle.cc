#include <GL/gl.h>
#include <GL/glut.h>

//called to draw scene
void RenderScene(void){
	glClear(GL_COLOR_BUFFER_BIT); //Clear window with current colour

	glColor3f(1.0f, 0.0f, 0.0f); //Set drawing color to red

	glRectf(-25.0f, 25.0f, 25.0f, -25.0f); //Draw a filled rectangle with current color

	glFlush();
}

//Set up the rendering state
void SetupRC(void){
	//Set Clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

//Called by GLUT library when the window has changed size
void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio;

	//prevent divide by 0
	if(h == 0)
		h = 1;

	//Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	//Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Establish clipping volume(left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;

	if(w <= h)
		glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GLRect");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();
	glutMainLoop();

	return 0;
}
