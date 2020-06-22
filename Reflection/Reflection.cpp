#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

bool bFull = false;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat tRot = 0.0f;

GLfloat fLightPos[4] =       {-100.0f, 100.0f, 50.0f, 1.0f};
GLfloat fLightPosMirror[4] = {-100.0f, -100.0f, 50.0f, 1.0f};
GLfloat fNoLight[4] =        {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat fAmbientLight[4] =   {0.25f, 0.25f, 0.25f, 1.0f};
GLfloat fDiffuseLight[4] =   {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fSpecular[4] =       {1.0f, 1.0f, 1.0f, 1.0f};

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void SpecialKeys(int, int, int);
    void keyboard(unsigned char, int, int);
    void timerFunction(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Reflection");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, timerFunction, 1);

    glutMainLoop();

    return (0);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);  //Hidden surface removal
    glEnable(GL_CULL_FACE);   //dont calculate inside
    glFrontFace(GL_CCW);      //counterclockwise windings
    glCullFace(GL_BACK);      //cull backs of polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //select polygon rasterization mode

    //Use lighting and material properties
    glEnable(GL_LIGHTING);

    //Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);   //light intensity of entire scene
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecular);
    glEnable(GL_LIGHT0);

    //use color tracking using glColor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(35.0f, aspectRatio, 1.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, -0.4f, 0.0f);
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

void timerFunction(int val)
{
    tRot += 1.0f;
    glutPostRedisplay();
    glutTimerFunc(10, timerFunction, 1);
}

//Draw black and white checkerboard pattern
void DrawGround(void)
{
    GLfloat fExtent = 20.0f;
    GLfloat fStep   = 0.5f;
    GLfloat y       = 0.0f;
    GLint iBounce   = 0;
    GLfloat iStrip, iRun ,fColor;

    glShadeModel(GL_FLAT);
    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
    {
        glBegin(GL_TRIANGLE_STRIP);
            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
            {
                if((iBounce % 2) == 0)
                    fColor = 1.0f;
                else
                    fColor = 0.0f;

                glColor4f(fColor, fColor, fColor, 0.5f);
                glVertex3f(iStrip, y, iRun);
                glVertex3f(iStrip + fStep, y, iRun);

                iBounce++;
            }
        glEnd();
    }
    glShadeModel(GL_SMOOTH);
}

void DrawWorld(void)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
        glTranslatef(0.0f, 0.5f, -3.5f);

        glPushMatrix();
            glRotatef(-tRot * 2.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f);
            glutSolidSphere(0.1f, 60, 90);
        glPopMatrix();

        glRotatef(tRot, 0.0f, 1.0f, 0.0f);
        glutSolidTorus(0.15f, 0.35f, 120, 64);
    glPopMatrix();
}

void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        //Move the light under floor to light the reflected World
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPosMirror);
        glPushMatrix();
            glFrontFace(GL_CW);         //Geometry is mirrored
            glScalef(1.0f, -1.0f, 1.0f);    //swap orientations
            DrawWorld();
            glFrontFace(GL_CCW);
        glPopMatrix();

        //Draw the ground transparently over the Reflection
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        DrawGround();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

        //Restore correct ligting and draw the world correctly
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
        DrawWorld();
    glPopMatrix();

    // Do the buffer Swap
    glutSwapBuffers();
}
