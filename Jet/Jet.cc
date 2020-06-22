#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("JET");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return (0);
}

void SetupRC()
{
    //bright white light full intensity of RGB
    GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};

    //Set background clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glEnable(GL_CCW);

    //enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    //front material ambient and diffuse colors track glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //enable lighting
    glEnable(GL_LIGHTING);

    //set light model to use ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -300.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Nose Cone /////////////////////////////
    // White
    glColor3ub(255, 255, 255);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 60.0f);
        glVertex3f(-15.0f, 0.0f, 30.0f);
        glVertex3f(15.0f,0.0f,30.0f);

        // Black
        glColor3ub(0,0,0);
        glVertex3f(15.0f,0.0f,30.0f);
        glVertex3f(0.0f, 15.0f, 30.0f);
        glVertex3f(0.0f, 0.0f, 60.0f);

        // Red
        glColor3ub(255,0,0);
        glVertex3f(0.0f, 0.0f, 60.0f);
        glVertex3f(0.0f, 15.0f, 30.0f);
        glVertex3f(-15.0f,0.0f,30.0f);


	// Body of the Plane ////////////////////////
        // Green
        glColor3ub(0,255,0);
        glVertex3f(-15.0f,0.0f,30.0f);
        glVertex3f(0.0f, 15.0f, 30.0f);
        glVertex3f(0.0f, 0.0f, -56.0f);

        glColor3ub(255,255,0);
        glVertex3f(0.0f, 0.0f, -56.0f);
        glVertex3f(0.0f, 15.0f, 30.0f);
        glVertex3f(15.0f,0.0f,30.0f);

        glColor3ub(0, 255, 255);
        glVertex3f(15.0f,0.0f,30.0f);
        glVertex3f(-15.0f, 0.0f, 30.0f);
        glVertex3f(0.0f, 0.0f, -56.0f);

	///////////////////////////////////////////////
	// Left wing
	// Large triangle for bottom of wing
        glColor3ub(128,128,128);
        glVertex3f(0.0f,2.0f,27.0f);
        glVertex3f(-60.0f, 2.0f, -8.0f);
        glVertex3f(60.0f, 2.0f, -8.0f);

        glColor3ub(64,64,64);
        glVertex3f(60.0f, 2.0f, -8.0f);
        glVertex3f(0.0f, 7.0f, -8.0f);
        glVertex3f(0.0f,2.0f,27.0f);

        glColor3ub(192,192,192);
        glVertex3f(60.0f, 2.0f, -8.0f);
        glVertex3f(-60.0f, 2.0f, -8.0f);
        glVertex3f(0.0f,7.0f,-8.0f);

	// Other wing top section
        glColor3ub(64,64,64);
        glVertex3f(0.0f,2.0f,27.0f);
        glVertex3f(0.0f, 7.0f, -8.0f);
        glVertex3f(-60.0f, 2.0f, -8.0f);

	// Tail section///////////////////////////////
	// Bottom of back fin
        glColor3ub(255,128,255);
        glVertex3f(-30.0f, -0.50f, -57.0f);
        glVertex3f(30.0f, -0.50f, -57.0f);
        glVertex3f(0.0f,-0.50f,-40.0f);

        // top of left side
        glColor3ub(255,128,0);
        glVertex3f(0.0f,-0.5f,-40.0f);
        glVertex3f(30.0f, -0.5f, -57.0f);
        glVertex3f(0.0f, 4.0f, -57.0f);

        // top of right side
        glColor3ub(255,128,0);
        glVertex3f(0.0f, 4.0f, -57.0f);
        glVertex3f(-30.0f, -0.5f, -57.0f);
        glVertex3f(0.0f,-0.5f,-40.0f);

        // back of bottom of tail
        glColor3ub(255,255,255);
        glVertex3f(30.0f,-0.5f,-57.0f);
        glVertex3f(-30.0f, -0.5f, -57.0f);
        glVertex3f(0.0f, 4.0f, -57.0f);

        // Top of Tail section left
        glColor3ub(255,0,0);
        glVertex3f(0.0f,0.5f,-40.0f);
        glVertex3f(3.0f, 0.5f, -57.0f);
        glVertex3f(0.0f, 25.0f, -65.0f);

        glColor3ub(255,0,0);
        glVertex3f(0.0f, 25.0f, -65.0f);
        glVertex3f(-3.0f, 0.5f, -57.0f);
        glVertex3f(0.0f,0.5f,-40.0f);

        // Back of horizontal section
        glColor3ub(128,128,128);
        glVertex3f(3.0f,0.5f,-57.0f);
        glVertex3f(-3.0f, 0.5f, -57.0f);
        glVertex3f(0.0f, 25.0f, -65.0f);

    glEnd(); // Of Jet

    glPopMatrix();


    glutSwapBuffers();
}
