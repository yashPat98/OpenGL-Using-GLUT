#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

bool bFull = false;
//Rotation
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//Lighting
GLfloat lightPos[] =     {0.0f, 0.0f, 75.0f, 1.0f};
GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specular[] =     {1.0f, 1.0f, 1.0f, 1.0f};
//Material
GLfloat specref[] =      {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat spotDir[] =      {0.0f, 0.0f, -1.0f};

//Flags for effects
#define MODE_FLAT       1
#define MODE_SMOOTH     2
#define MODE_VERYLOW    3
#define MODE_MEDIUM     4
#define MODE_VERYHIGH   5
#define MODE_ULTRA      6

int iShade = MODE_FLAT;
int iTess =  MODE_VERYLOW;

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void SpecialKeys(int, int, int);
    void keyboard(unsigned char, int, int);
    void processMenu(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Spotlight");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(keyboard);

    //Menu
    glutCreateMenu(processMenu);
    glutAddMenuEntry("Flat Shading", 1);
    glutAddMenuEntry("Smooth Shading", 2);
    glutAddMenuEntry("Very low Tess", 3);
    glutAddMenuEntry("Medium Tess", 4);
    glutAddMenuEntry("Very high Tess", 5);
    glutAddMenuEntry("Ultra Tess", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return (0);
}

void SetupRC()
{
    //Set background clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);      //Hidden surface removal
    glEnable(GL_CULL_FACE);       //Don't display back sides
    glFrontFace(GL_CCW);          //Couter-clockwise polygon winding

    //Enable lighting
    glEnable(GL_LIGHTING);

    //Set up and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
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

    gluPerspective(45.0f, aspectRatio, 1.0, 450.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -250.0f);
}

void processMenu(int value)
{
    switch(value)
    {
        case 1:
            iShade = MODE_FLAT;
            break;
        case 2:
            iShade = MODE_SMOOTH;
            break;
        case 3:
            iTess = MODE_VERYLOW;
            break;
        case 4:
            iTess = MODE_MEDIUM;
            break;
        case 5:
            iTess = MODE_VERYHIGH;
            break;
        case 6:
            iTess = MODE_ULTRA;
        default:
            break;
    }

    glutPostRedisplay();
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

void RenderScene(void)
{
    //Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(iShade == MODE_FLAT)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);

    //First place the light
    //save the coordinate transformation
    glPushMatrix();
        //rotate coordinate system
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);

        //specify new position and direction of light
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

        //draw a red cone to enclose light source
        glColor3ub(255, 0, 0);

        //translate origin to draw cone where light is positioned
        glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
        glutSolidCone(4.0f,6.0f, 15, 15);

        //save the lighting state
        glPushAttrib(GL_LIGHTING_BIT);
            //turn off lighting so that bulb will not use
            //lighting and material ie using default glColor
            glDisable(GL_LIGHTING);
            glColor3ub(255, 255, 0);
            glutSolidSphere(3.0f, 15, 15);
        //restore lighting state
        glPopAttrib();
    glPopMatrix();

    //set material color and draw the sphere
    glColor3ub(0, 0, 255);

    if(iTess == MODE_VERYLOW)
        glutSolidSphere(30.0f, 7, 7);
    else if(iTess == MODE_MEDIUM)
        glutSolidSphere(30.0f, 15, 15);
    else if (iTess == MODE_VERYHIGH)
        glutSolidSphere(30.0f, 50, 50);
    else if(iTess == MODE_ULTRA)
        glutSolidSphere(30.0f, 150, 150);

    glutSwapBuffers();
}
