#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rsize = 25.0f;

GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat windowWidth;
GLfloat windowHeight;

int main(int argc, char **argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei width, GLsizei height);
    void RenderScene(void);
    void TimerFunction(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Stencil");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(20, TimerFunction, 1);

    glutMainLoop();

    return 0; //This line is not necessary
}

void SetupRC(void)
{
    //Set Clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Set Stencil clear
    glClearStencil(0.0f);

    //Enable Stencil test
    glEnable(GL_STENCIL_TEST);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(width <= height)
    {
        windowWidth = 100.0f;
        windowHeight = 100.0f / aspectRatio;
        glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -100.0f, 100.0f);
    }
    else
    {
        windowHeight = 100.0f;
        windowWidth = 100.0f * aspectRatio;

        glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -100.0f, 100.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TimerFunction(int value)
{
    if(x > windowWidth - rsize || x < -windowWidth)
        xstep = -xstep;

    if(y > windowHeight || y < -windowHeight + rsize)
        ystep = -ystep;

    x += xstep;
    y += ystep;

    if(x > (windowWidth - rsize + xstep))
        x = windowWidth - rsize -1;
    else if(x < -(windowWidth + xstep))
        x = -windowWidth - 1;

    if(y > (windowHeight + ystep))
		y = windowHeight - 1;
	else if(y < - (windowHeight - rsize + ystep))
		y = -windowHeight + rsize - 1;

    glutPostRedisplay();
    glutTimerFunc(20, TimerFunction, 1);
}

void RenderScene(void)
{
    GLdouble dRadius = 0.1;  //Initial radius of spiral
    GLdouble dAngle;

    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //All drawing commands fail the stencil test and are not drawn but
    //increment value in stencil buffer
    glStencilFunc(GL_NEVER, 0x0, 0x0);
    glStencilOp(GL_INCR, GL_INCR ,GL_INCR);

    //Spiral Pattern will create stencil pattern
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
        for(dAngle = 0; dAngle < 400.0; dAngle += 0.1)
        {
            glVertex2d(dRadius * cos(dAngle), dRadius * sin(dAngle));
            dRadius *= 1.002;
        }
    glEnd();

    //Now allow drawing where stencil pattern is 0x1
    //and do not make any further changes to stencil buffer
    glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    //Now draw red bouncing square
    //x and y are modified by timer Function
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(x, y, x + rsize, y - rsize);

    //Swap and flush buffers
    glutSwapBuffers();
}
