#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

bool bFull = false;
GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat sourceLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat whiteLight[] = {0.2f, 0.2f, 0.2f, 1.0f};

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
    glutCreateWindow("Solar");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
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

    glFrontFace(GL_CCW);

    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspectRatio, 1.0, 425.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

void RenderScene(void)
{
    //Earth and moon angle of revolution
    static float fMoonRot = 0.0f;
    static float fEarthRot = 0.0f;

    //Clear the window with current current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Translate whole scene out and into view
    glTranslatef(0.0f, 0.0f, -300.0f);

    glPushMatrix();
    //Set material color to yellow
    //Sun
    glColor3ub(255, 255, 0);
    glDisable(GL_LIGHTING);
    glutSolidSphere(15.0f, 36, 18);
    glEnable(GL_LIGHTING);

    //Position the light after we draw the Sun
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //Rotate the coordinate system
    glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);

    //Draw the Earth
    glColor3ub(0, 0, 255);
    glTranslatef(105.0f, 0.0f, 0.0f);
    glutSolidSphere(15.0f, 36, 18);

    //Rotate from Earth based coordinates and draw moon
    glColor3ub(200, 200, 200);
    glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(30.0f, 0.0f, 0.0f);

    fMoonRot += 15.0f;
    if(fMoonRot > 360.0f)
        fMoonRot = 0.0f;

    glutSolidSphere(6.0f, 36, 18);

    glPopMatrix();

    fEarthRot += 5.0f;
    if(fEarthRot > 360.0f)
        fEarthRot = 0.0f;

    glutSwapBuffers();
}
