#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#define GL_PI 3.141592

bool bFull = false;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void SpecialKeys(int, int, int);
    void keyboard(unsigned char, int, int);
    void Nucleus(int);
    void TimerFunction(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Carbon");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();

    return (0);
}

void SetupRC()
{
    //Set background clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspectRatio, 1.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        xRot -= 5.0f;

    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if(key > 356.0f)
        xRot = 0.0f;

    if(key < -1.0f)
        xRot = 355.0f;

    if(key > 356.0f)
        yRot = 0.0f;

    if(key < -1.0f)
        yRot = 355.0f;

    glutPostRedisplay();
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
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void drawNucleus()
{
    glColor3ub(235, 64, 52);
    glutSolidSphere(15.0f, 36, 18);

    glPushMatrix();
    glColor3ub(0, 0, 255);
    glTranslatef(26.0f, 0.0f, 0.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(0, 0, 255);
    glTranslatef(-26.0f, 0.0f, 0.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(235, 64, 52);
    glTranslatef(0.0f, 26.0f, 0.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(0, 0, 255);
    glTranslatef(0.0f, -26.0f, 0.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(235, 64, 52);
    glTranslatef(0.0f, 0.0f, 26.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(0, 0, 255);
    glTranslatef(0.0f, 0.0f, -26.0f);
    glutSolidSphere(15.0f, 36, 18);
    glPopMatrix();
}

void RenderScene(void)
{
    static float fElectRot = 0.0f;
    static float fElectRot2 = 0.0f;
    static float fElectRot3 = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -300.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    drawNucleus();

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElectRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(70.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElectRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(-70.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    fElectRot += 10.0f;
    if(fElectRot > 360.0f)
        fElectRot = 0.0f;

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(fElectRot2, 0.0f, 1.0f, 0.0f);
    glTranslatef(100.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(fElectRot2, 0.0f, 1.0f, 0.0f);
    glTranslatef(-100.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    fElectRot2 += 6.0f;
    if(fElectRot2 > 360.0f)
        fElectRot2 = 0.0f;

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElectRot3, 0.0f, 1.0f, 0.0f);
    glTranslatef(100.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(29, 163, 65);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElectRot3, 0.0f, 1.0f, 0.0f);
    glTranslatef(100.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 36, 18);
    glPopMatrix();

    fElectRot3 += 4.0f;
    if(fElectRot3 > 360.0f)
        fElectRot3 = 0.0f;

    glutSwapBuffers();
}
