#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <math3d.h>

bool bFullscreen = false;

int main(int argc, char **argv)
{
    //function prototypes
    void SetupRC();
    void changeSize(GLsizei , GLsizei);
    void keyboard(unsigned char , int, int);
    void renderScene(void);
    void timerFunction(int);

    //initialize glut window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Torus");

    SetupRC();

    //register callback functions
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, timerFunction, 1);

    glutMainLoop();
    return (0);
}

void changeSize(GLsizei w, GLsizei h)
{
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

    if(h <= 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //z-near and z-far are always positive
    gluPerspective(45.0, aspectRatio, 1.0, 400.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 32:
            if(bFullscreen == false)
            {
                glutFullScreen();
                bFullscreen = true;
            }
            else
            {
                glutReshapeWindow(800, 600);
                bFullscreen = false;
            }
            break;
        default:
            break;
    }
}

//triggers after every 33 msec
void timerFunction(int val)
{

    glutPostRedisplay();
    glutTimerFunc(50, timerFunction, 1);
}

//initialization before rendering scene
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);

    glEnable(GL_CULL_FACE);
}

//called to draw renderScene
void renderScene(void)
{
    M3DMatrix44f transformationMatrix, rotationMatrix, translationMatrix;    //storage for rotation matrix
    static GLfloat yRot = 0.0f;           //rotation angle for animation
    yRot += 5.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    //build a rotation matrix
    m3dRotationMatrix44(transformationMatrix, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);

    transformationMatrix[12] = 0.0f;
    transformationMatrix[13] = 0.0f;
    transformationMatrix[14] = -100.0f;

    glLoadMatrixf(transformationMatrix);

    glColor3ub(201, 91, 66);
    glutWireTorus(8, 20, 72, 36);

    /*
    glPushMatrix();
        m3dRotationMatrix44(rotationMatrix, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
        m3dTranslationMatrix44(translationMatrix ,0.0f, 0.0f, -100.0f);

        glMultMatrixf(translationMatrix);
        glMultMatrixf(rotationMatrix);
        glColor3ub(201, 91, 66);
        glutSolidTorus(5, 20, 72, 36);
    glPopMatrix();
    */

    glutSwapBuffers();
}
