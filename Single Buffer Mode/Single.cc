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
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Single buffer mode");

	SetupRC();
	
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	
	glutMainLoop();
	
	return (0);   //This line is not necessary
}

void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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
	static GLdouble dRadius = 0.1;
	static GLdouble dAngle = 0.0;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if(dAngle == 0.0)
		glClear(GL_COLOR_BUFFER_BIT);
		
	glBegin(GL_POINTS);
		glVertex2d(dRadius * cos(dAngle), dRadius * sin(dAngle));
	glEnd();
	
	dRadius *= 1.01;
	dAngle += 0.1;
	
	if(dAngle > 30)
	{
		dRadius = 0.1;
		dAngle = 0.0;
	}
	
	glFlush();
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

