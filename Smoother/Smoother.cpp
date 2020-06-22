#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

bool bFull = false;

#define GL_PI          3.141592f
#define SMALL_STARS    100
#define MEDIUM_STARS   40
#define LARGE_STARS    15
#define SCREEN_X       800
#define SCREEN_Y       600

typedef GLfloat M3DVector2f[2];

M3DVector2f vSmallStars[SMALL_STARS];
M3DVector2f vMediumStars[MEDIUM_STARS];
M3DVector2f vLargeStars[LARGE_STARS];

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void keyboard(unsigned char, int, int);
    void ProcessMenu(int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Smoother");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboard);

    //Menu
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Antialiased Rendering", 1);
    glutAddMenuEntry("Normal Rendering", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

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

    gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y);

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

void ProcessMenu(int value)
{
    switch(value)
    {
        case 1:
            //Turn on antialiasing, and give hint to do the
            //best job possible
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            break;

        case 2:
            //Turn off blending and all Smoothing
            glDisable(GL_BLEND);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

void SetupRC()
{
    int i;

    //Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Populate star list
    for(i = 0; i < SMALL_STARS; i++)
    {
        vSmallStars[i][0] = (GLfloat)(rand() % SCREEN_X);
        vSmallStars[i][1] = (GLfloat)(rand() % (SCREEN_Y - 100)) + 100.0f;
    }

    //Populate star list
    for(i = 0; i < MEDIUM_STARS; i++)
    {
        vMediumStars[i][0] = (GLfloat)(rand() % SCREEN_X * 10) / 10.0f;
        vMediumStars[i][1] = (GLfloat)(rand() % SCREEN_Y - 100) + 100.0f;
    }

    //Populate star list
    for(i = 0; i < LARGE_STARS; i++)
    {
        vLargeStars[i][0] = (GLfloat)(rand() % SCREEN_X * 10) / 10.0f;
        vLargeStars[i][1] = (GLfloat)(rand() % (SCREEN_Y - 100) * 10.0f) / 10.0f + 100.0f;
    }
}

void RenderScene(void)
{
    int i;    //Loop variable
    GLfloat x = 700.0f;     //Location and radius of moon
    GLfloat y = 500.0f;
    GLfloat r = 50.0f;
    GLfloat angle = 0.0f;

    //Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    //Set drawing color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    //Draw small stars
    glPointSize(1.0f);
    glBegin(GL_POINTS);
        for(i = 0; i < SMALL_STARS; i++)
            glVertex2fv(vSmallStars[i]);
    glEnd();

    //Draw medium sized stars
    glPointSize(3.05f);
    glBegin(GL_POINTS);
        for(i = 0; i < MEDIUM_STARS; i++)
            glVertex2fv(vMediumStars[i]);
    glEnd();

    //Draw Large sized stars
    glPointSize(5.5f);
    glBegin(GL_POINTS);
        for(i = 0; i < LARGE_STARS; i++)
            glVertex2fv(vLargeStars[i]);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for(angle = 0; angle < (2.0f * GL_PI); angle += 0.1f)
            glVertex2f(x + (float)cos(angle) * r, y + (float)sin(angle) * r);
        glVertex2f(x + r, y);
    glEnd();

    //Drae distant horizon
    glLineWidth(3.5);
    glBegin(GL_LINE_STRIP);
        glVertex2f(0.0f, 25.0f);
        glVertex2f(50.0f, 100.0f);
        glVertex2f(100.0f, 25.0f);
        glVertex2f(225.0f, 125.0f);
        glVertex2f(300.0f, 50.0f);
        glVertex2f(375.0f, 100.0f);
        glVertex2f(460.0f, 25.0f);
        glVertex2f(525.0f, 100.0f);
        glVertex2f(600.0f, 20.0f);
        glVertex2f(675.0f, 70.0f);
        glVertex2f(750.0f, 25.0f);
        glVertex2f(800.0f, 90.0f);
    glEnd();

    glutSwapBuffers();
}
