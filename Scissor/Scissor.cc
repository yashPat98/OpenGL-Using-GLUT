#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

int main(int argc, char **argv)
{
	//Function prototypes
	void SetupRC(void);
	void ChangeSize(GLsizei width, GLsizei height);
	void RenderScene(void);
	void TimerFunction(int);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scissors");

	SetupRC();

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutMainLoop();

	return (0);   //This line is not necessary
}

void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}

void ChangeSize(GLsizei width, GLsizei height)
{
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

	if(height <= 0)
		height = 1;

	glViewport(0, 0, width, height);

	//Reset the coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void RenderScene(void)
{
    //Clear the background with current clearing color
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Now set scissor to smaller red sub region
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glScissor(100, 100, 600, 400);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    //Finally even smaller sub region
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glScissor(200, 200, 400, 200);
    glClear(GL_COLOR_BUFFER_BIT);

    //Turn scissors back off for next render
    glDisable(GL_SCISSOR_TEST);

    glutSwapBuffers();
}
