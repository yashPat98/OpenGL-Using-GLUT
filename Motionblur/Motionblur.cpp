#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

bool bFull               = false;
static GLfloat yRot      = 45.0f;

GLfloat fAmbientLight[4] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat fDiffuseLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fSpecular[4]     = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fNoLight[4]      = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat fLightPos[4]     = {-100.0f, 100.0f, 50.0f, 1.0f};

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void keyboard(unsigned char, int, int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Motion Blur");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboard);

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

    gluPerspective(35.0f, aspectRatio, 1.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, -0.4f, 0.0f);
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

void SetupRC()
{
    //Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Use lighting and material properties
    glEnable(GL_LIGHTING);

    //Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    glEnable(GL_LIGHT0);

    //Use color tracking using glcolor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

}

void DrawGround(void)
{
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 0.5f;
    GLfloat y = 0.0f;
    GLfloat fColor;
    GLfloat iStrip, iRun;
    GLint iBounce = 0;

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

            glColor4f(fColor, fColor, fColor, 0.5);
            glVertex3f(iStrip, y, iRun);
            glVertex3f(iStrip + fStep, y, iRun);

            iBounce++;
        }
        glEnd();
    }
    glShadeModel(GL_SMOOTH);
}

void DrawGeometry(void)
{
    //Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        DrawGround();

        //Place the moving sphere
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.5f, -3.5f);
        glRotatef(-(yRot * 2.0f), 0.0f, 1.0f, 0.0f);
        glTranslatef(1.0f, 0.0f, 0.0f);
        glutSolidSphere(0.1f, 64, 42);
    glPopMatrix();
}

void RenderScene(void)
{
    GLfloat fPass;
    GLfloat fPasses = 10.0f;

    //Set current rotation back a few degrees
    yRot = 35.0f;

    for(fPass = 0.0f; fPass < fPasses; fPass += 1.0f)
    {
        yRot += 0.75f;

        DrawGeometry();

        //accumulate back Buffer
        if(fPass == 0.0f)
            glAccum(GL_LOAD, 0.5f);
        else
            glAccum(GL_ACCUM, 0.5f * (1.0f / fPasses));
    }

    //Copy accumulation buffer to color buffer
    //and do buffer swap
    glAccum(GL_RETURN, 1.0f);
    glutSwapBuffers();
}
