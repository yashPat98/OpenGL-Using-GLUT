#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <math3d.h>

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
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    //Set background clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glEnable(GL_CCW);

    //enable lighting and material
    glEnable(GL_LIGHTING);

    //set up light0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);

    //enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    //front material ambient and diffuse colors track glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    //Rescale normals to unit length
    glEnable(GL_NORMALIZE);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
    GLfloat lightPos[] = {-50.0f, 50.0f, 100.0f, 1.0f};

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspectRatio, 1.0, 450.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
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
    M3DVector3f vNormal;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -300.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();

    // Nose Cone /////////////////////////////
    // White
    glColor3ub(128, 128, 128);
    glBegin(GL_TRIANGLES);
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

    glEnd(); // Of Jet

    glPopMatrix();


    glutSwapBuffers();
}
