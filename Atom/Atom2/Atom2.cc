#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

bool bFull = false;

int main(int argc, char** argv)
{
    //Function Prototypes
    void RenderScene(void);
    void ChangeSize(GLsizei , GLsizei);
    void keyboard(unsigned char, int, int);
    void SetupRC(void);
    void TimerFunction(int);

    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Atom");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, TimerFunction, 1);

    glutMainLoop();

    return (0);
}

void SetupRC(void)
{
    //Set Clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);

    glEnable(GL_CULL_FACE);
}

void ChangeSize(GLsizei w, GLsizei h)
{
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

    if(h <= 0)
        h = 1;

    glViewport(0, 0, w, h);

    //Set active matrix as GL_PROJECTION
    //and reset the matrix
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
    glutTimerFunc(50, TimerFunction, 1);
}

//Called to draw scene
void RenderScene(void)
{
    //Angle of revolution around Nucleus
    static GLfloat fElect1 = 0.0f;

    //Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set Modelview as active matrix and
    //reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Translate whole scene out into view
    //This is initial viewing transformation
    glTranslatef(0.0f, 0.0f, -100.0f);

    //Red Nucleus (ub - Unsigned byte/unsigned char generally 8-bit)
    glColor3ub(255, 0, 0);
    glutSolidSphere(10.0f, 15, 15);

    //Yellow Electrons
    glColor3ub(255, 255, 0);

    //First Electron Orbit
    //save Viewing transformation
    glPushMatrix();
    //Rotate by angle of revolution
    glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
    //Translate out from origin to orbit distance
    glTranslatef(90.0f, 0.0f, 0.0f);
    //Draw the Electron
    glutSolidSphere(6.0f, 15, 15);
    //Restore the viewing transformation
    glPopMatrix();


    //Second Electron orbit
    glPushMatrix();
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
    glTranslatef(-70.0f, 0.0f, 0.0f);
    glutSolidSphere(6.0f, 15, 15);
    glPopMatrix();

    //Third Electron orbit
    glPushMatrix();
    glRotatef(315.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 60.0f);
    glutSolidSphere(6.0f, 15, 15);
    glPopMatrix();

    //Increment angle of revolution
    fElect1 += 10.0f;
    if(fElect1 > 360.0f)
        fElect1 = 0.0f;

    //Swap and Flush the buffers
    glutSwapBuffers();
}
