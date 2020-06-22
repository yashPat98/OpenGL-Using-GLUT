#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math3d.h>

#define TRUE    1
#define FALSE   0

bool bFullscreen;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//Light values and coordinates
GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specular[] =     {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos[] =     {-75.0f, 150.0f, -50.0f, 0.0f};
GLfloat specref[] =      {1.0f, 1.0f, 1.0f, 1.0f};

//Transformation matrix to project Shadows
M3DMatrix44f shadowMat;

int main(int argc, char *argv[])
{
    //function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void Keyboard(unsigned char, int, int);
    void SpecialKeys(int, int, int);

    //initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shadow");

    SetupRC();

    //Registering callback functions
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);

    glutMainLoop();

    return (0);
}

void SetupRC(void)
{
    //Any three points on ground (CCW order)
    M3DVector3f points[3] = {
        {-30.0f, -149.0f, -20.0f},
        {-30.0f, -149.0f, 20.0f},
        {40.0f, -149.0f, 20.0f}
    };


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);    //Hidden surface removal
    glFrontFace(GL_CCW);        //Counterclockwise Winding
    glEnable(GL_CULL_FACE);     //Do not calculate inside of JET

    //Lighting
    glEnable(GL_LIGHTING);

    //Setup light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    //Material
    //Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    //front material ambient and diffuse
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    //get plane eqation from three points on ground
    M3DVector4f vPlaneEquation;
    m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);

    //calculate projection matrix to draw shadows on ground
    m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, lightPos);

    //rescale normals to unit length
    glEnable(GL_NORMALIZE);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspectRatio = (GLfloat)width / (GLfloat)height;

    gluPerspective(60.0f, aspectRatio, 1.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -400.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Keyboard(unsigned char key, int x, int y)
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

void DrawJet(int iShadow)
{
    M3DVector3f vNormal;

    if(iShadow == 0)
        glColor3ub((GLubyte)128, (GLubyte)128, (GLubyte)128);
    else
        glColor3ub((GLubyte)0, (GLubyte)0, (GLubyte)0);

    glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 60.0f);
        glVertex3f(-15.0f, 0.0f, 30.0f);
        glVertex3f(15.0f,0.0f,30.0f);

        // Black
        {
            M3DVector3f vPoints[3] = {
                {15.0f,0.0f,30.0f},
                {0.0f, 15.0f, 30.0f},
                {0.0f, 0.0f, 60.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // Red
        {
            M3DVector3f vPoints[3] = {
                {0.0f, 0.0f, 60.0f},
                {0.0f, 15.0f, 30.0f},
                {-15.0f,0.0f,30.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

    // Body of the Plane ////////////////////////
        // Green
        {
            M3DVector3f vPoints[3] = {
                {-15.0f,0.0f,30.0f},
                {0.0f, 15.0f, 30.0f},
                {0.0f, 0.0f, -56.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        {
            M3DVector3f vPoints[3] = {
                {0.0f, 0.0f, -56.0f},
                {0.0f, 15.0f, 30.0f},
                {15.0f,0.0f,30.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        {
            M3DVector3f vPoints[3] = {
                {15.0f,0.0f,30.0f},
                {-15.0f, 0.0f, 30.0f},
                {0.0f, 0.0f, -56.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

    ///////////////////////////////////////////////
    // Left wing
    // Large triangle for bottom of wing
        {
            M3DVector3f vPoints[3] = {
                {0.0f,2.0f,27.0f},
                {-60.0f, 2.0f, -8.0f},
                {60.0f, 2.0f, -8.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        {
            M3DVector3f vPoints[3] = {
                {60.0f, 2.0f, -8.0f},
                {0.0f, 7.0f, -8.0f},
                {0.0f,2.0f,27.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        {
            M3DVector3f vPoints[3] = {
                {60.0f, 2.0f, -8.0f},
                {-60.0f, 2.0f, -8.0f},
                {0.0f,7.0f,-8.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

    // Other wing top section
        {
            M3DVector3f vPoints[3] = {
                {0.0f,2.0f,27.0f},
                {0.0f, 7.0f, -8.0f},
                {-60.0f, 2.0f, -8.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

    // Tail section///////////////////////////////
    // Bottom of back fin
        {
            M3DVector3f vPoints[3] = {
                {-30.0f, -0.50f, -57.0f},
                {30.0f, -0.50f, -57.0f},
                {0.0f,-0.50f,-40.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // top of left side
        {
            M3DVector3f vPoints[3] = {
                {0.0f,-0.5f,-40.0f},
                {30.0f, -0.5f, -57.0f},
                {0.0f, 4.0f, -57.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // top of right side
        {
            M3DVector3f vPoints[3] = {
                {0.0f, 4.0f, -57.0f},
                {-30.0f, -0.5f, -57.0f},
                {0.0f,-0.5f,-40.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // back of bottom of tail
        {
            M3DVector3f vPoints[3] = {
                {30.0f,-0.5f,-57.0f},
                {-30.0f, -0.5f, -57.0f},
                {0.0f, 4.0f, -57.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // Top of Tail section left
        {
            M3DVector3f vPoints[3] = {
                {0.0f,0.5f,-40.0f},
                {3.0f, 0.5f, -57.0f},
                {0.0f, 25.0f, -65.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        {
            M3DVector3f vPoints[3] = {
                {0.0f, 25.0f, -65.0f},
                {-3.0f, 0.5f, -57.0f},
                {0.0f,0.5f,-40.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

        // Back of horizontal section
        {
            M3DVector3f vPoints[3] = {
                {3.0f,0.5f,-57.0f},
                {-3.0f, 0.5f, -57.0f},
                {0.0f, 25.0f, -65.0f}
            };

            m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);
            glNormal3fv(vNormal);
            glVertex3fv(vPoints[0]);
            glVertex3fv(vPoints[1]);
            glVertex3fv(vPoints[2]);
        }

    glEnd();
}

void RenderScene(void)
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw the ground
    //Darker green for background to give illusion of depth
    glBegin(GL_QUADS);
        glColor3ub((GLubyte)0, (GLubyte)32, (GLubyte)0);
        glVertex3f(400.0f, -150.0f, -200.0f);
        glVertex3f(-400.0f, -150.0f, -200.0f);
        glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);
        glVertex3f(-400.0f, -150.0f, 200.0f);
        glVertex3f(400.0f, -150.0f, 200.0f);
    glEnd();

    //save the matrix state and do the rotation
    glPushMatrix();
        //Draw jet at new orientation, put light in correct position
        //before rotating the JET
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);

        DrawJet(FALSE);
        //restore original matrix state
    glPopMatrix();

    //For drawing shadow
    //disable ligting and save the projection state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPushMatrix();
        //multiply by shadow projection matrix
        glMultMatrixf((GLfloat *)shadowMat);
        //now rotate the JET around in new flattened state
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        //pass true to indicate drawing shadow
        DrawJet(TRUE);
    //restore projection to normal
    glPopMatrix();

    //Draw the light source
    glPushMatrix();
        glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
        glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)0);
        glutSolidSphere(5.0f, 10, 10);
    glPopMatrix();

    //restore lighting state
    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
}
