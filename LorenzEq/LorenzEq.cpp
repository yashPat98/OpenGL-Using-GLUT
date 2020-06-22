#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>

#define ROWS    10000
#define COLUMNS 3

bool bFull = false;

GLfloat a = 10.0f;
GLfloat b = 28.0f;
GLfloat c = 8.0f/3.0f;

GLfloat x = 0.01f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;

GLfloat Points[ROWS][COLUMNS];

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void keyboard(unsigned char, int, int);
    void TimerFunction(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lorenz Attractor");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(20, TimerFunction, 1);

    glutMainLoop();

    return (0);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspectRatio, 1.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -400.0f);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 32:
            if(bFull == false)
            {
                glutFullScreen();
                bFull = true;
            }
            else
            {
                glutReshapeWindow(800, 600);
                bFull = false;
            }
            break;
        default:
            break;
    }
}

void TimerFunction(int val)
{
    static int count = 0;
    count++;

    glutPostRedisplay();
    if(count != (ROWS - 2))
        glutTimerFunc(20, TimerFunction, 1);
}

void SetupRC()
{
    int i, j;

    //Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLUMNS; j++)
        {
            Points[i][j] = 0.0f;
        }
    }
}

void RenderScene(void)
{
    //Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static int i = 0;

    GLfloat dt = 0.01;
    GLfloat dx = (a * (y - x)) * dt;
    GLfloat dy = (x * (b - z) - y) * dt;
    GLfloat dz = (x * y - c * z) * dt;

    x = x + dx;
    y = y + dy;
    z = z + dz;

    glColor3ub((GLfloat)250, (GLfloat)197, (GLfloat)50);

    Points[i][0] = x;
    Points[i][1] = y;
    Points[i][2] = z;

    if(i != 0)
    {
        glPushMatrix();
        glScalef(3.0f, 3.0f, 3.0f);
        glBegin(GL_LINES);
            for(int j = i; j > 0; j--)
            {
                glVertex3f(Points[j][0], Points[j][1], Points[j][2]);
                glVertex3f(Points[j - 1][0], Points[j - 1][1], Points[j - 1][2]);
            }
        glEnd();
        glPopMatrix();
    }

    i++;
    glutSwapBuffers();
}
