#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math3d.h>

bool bFull = false;

GLfloat lightPos[4] =        {-100.0f, 100.0f, 50.0f, 1.0f};
GLfloat fNoLight[4] =        {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat fAmbientLight[4] =   {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat fDiffuseLight[4] =   {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fSpecular[4] =       {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fFogColor[4] =       {0.847656f, 0.84375f, 0.83984f, 1.0f};

M3DMatrix44f mShadowMat;

class GLFrame
{
    protected:
        M3DVector3f vOrigin;
        M3DVector3f vForward;
        M3DVector3f vUp;

    public:
        GLFrame(void)
        {
            //at origin
            vOrigin[0] = 0.0f;
            vOrigin[1] = 0.0f;
            vOrigin[2] = 0.0f;

            //up is up (+Y)
            vUp[0] = 0.0f;
            vUp[1] = 1.0f;
            vUp[2] = 0.0f;

            //foreward is -Z (OpenGL default)
            vForward[0] = 0.0f;
            vForward[1] = 0.0f;
            vForward[2] = -1.0f;
        }

        inline void SetOrigin(float x, float y, float z)
        {
            vOrigin[0] = x;
            vOrigin[1] = y;
            vOrigin[2] = z;
        }

        inline void MoveForward(float fDelta)
        {
            vOrigin[0] += vForward[0] * fDelta;
            vOrigin[1] += vForward[1] * fDelta;
            vOrigin[2] += vForward[2] * fDelta;
        }

        void RotateLocalY(float fAngle)
        {
            M3DMatrix44f rotMat;
            // Just Rotate around the up vector
            // Create a rotation matrix around my Up (Y) vector
            m3dRotationMatrix44(rotMat, fAngle, vUp[0], vUp[1], vUp[2]);

            M3DVector3f newVect;

            // Rotate forward pointing vector (inlined 3x3 transform)
            newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] *  vForward[2];
            newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] *  vForward[2];
            newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
            m3dCopyVector3(vForward, newVect);
        }

        void GetMatrix(M3DMatrix44f	matrix, bool bRotationOnly = false)
        {
            // Calculate the right side (x) vector, drop it right into the matrix
            M3DVector3f vXAxis;
            m3dCrossProduct(vXAxis, vUp, vForward);

            // Set matrix column does not fill in the fourth value...
            m3dSetMatrixColumn44(matrix, vXAxis, 0);
            matrix[3] = 0.0f;

            // Y Column
            m3dSetMatrixColumn44(matrix, vUp, 1);
            matrix[7] = 0.0f;

            // Z Column
            m3dSetMatrixColumn44(matrix, vForward, 2);
            matrix[11] = 0.0f;

            // Translation (already done)
            if(bRotationOnly == true)
            {
                matrix[12] = 0.0f;
                matrix[13] = 0.0f;
                matrix[14] = 0.0f;
            }
            else
            m3dSetMatrixColumn44(matrix, vOrigin, 3);

            matrix[15] = 1.0f;
        }

        inline void GetCameraOrientation(M3DMatrix44f m)
        {
			M3DVector3f x, z;

			// Make rotation matrix
			// Z vector is reversed
			z[0] = -vForward[0];
			z[1] = -vForward[1];
			z[2] = -vForward[2];

			// X vector = Y cross Z
			m3dCrossProduct(x, vUp, z);

			// Matrix has no translation information and is
			// transposed.... (rows instead of columns)
			#define M(row,col)  m[col*4+row]
			   M(0, 0) = x[0];
			   M(0, 1) = x[1];
			   M(0, 2) = x[2];
			   M(0, 3) = 0.0;
			   M(1, 0) = vUp[0];
			   M(1, 1) = vUp[1];
			   M(1, 2) = vUp[2];
			   M(1, 3) = 0.0;
			   M(2, 0) = z[0];
			   M(2, 1) = z[1];
			   M(2, 2) = z[2];
			   M(2, 3) = 0.0;
			   M(3, 0) = 0.0;
			   M(3, 1) = 0.0;
			   M(3, 2) = 0.0;
			   M(3, 3) = 1.0;
			#undef M
        }

        inline void ApplyCameraTransform()
        {
            M3DMatrix44f m;

            GetCameraOrientation(m);
            // Camera Transform
            glMultMatrixf(m);

            glTranslatef(-vOrigin[0], -vOrigin[1], -vOrigin[2]);
        }
};

#define NUM_SPHERES 100
GLFrame spheres[NUM_SPHERES];
GLFrame frameCamera;
M3DVector3f vColor[NUM_SPHERES];

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sphere-World");

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
    int iSphere, iColor;

    M3DVector3f vPoints[3] = {{0.0f, -0.4f, 0.0f},
                              {10.0f, -0.4f, 0.0f},
                              {5.0f, -0.4f, -5.0f}};

    glClearColor(fFogColor[0], fFogColor[1], fFogColor[2], fFogColor[3]);

    glEnable(GL_DEPTH_TEST);  //Hidden surface removal
    glEnable(GL_CULL_FACE);   //dont calculate inside
    glFrontFace(GL_CCW);      //counterclockwise windings
    glCullFace(GL_BACK);      //cull backs of polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //select polygon rasterization mode
    glEnable(GL_MULTISAMPLE_ARB);

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
    glMaterialfv(GL_FRONT, GL_SPECULAR, fSpecular);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    //Setup Fog parameters
    glHint(GL_FOG_HINT, GL_NICEST);
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fFogColor);
    glFogf(GL_FOG_START, 5.0f);
    glFogf(GL_FOG_END, 30.0f);
    glFogi(GL_FOG_MODE, GL_LINEAR);

    M3DVector4f vPlaneEquation;
    m3dGetPlaneEquation(vPlaneEquation, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMat, vPlaneEquation, lightPos);

    for(iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
    {
        // Pick a random location between -20 and 20 at .1 increments
        float x = ((float)((rand() % 400) - 200) * 0.1f);
        float z = (float)((rand() % 400) - 200) * 0.1f;
        spheres[iSphere].SetOrigin(x, 0.0f, z);
    }

    for(iColor = 0; iColor < NUM_SPHERES; iColor++)
    {
        unsigned int xCol = rand() % 256;
        unsigned int yCol = rand() % 256;
        unsigned int zCol = rand() % 256;

        vColor[iColor][0] = xCol;
        vColor[iColor][1] = yCol;
        vColor[iColor][2] = zCol;
    }

}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(35.0f, aspectRatio, 1.0, 450.0);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        frameCamera.MoveForward(0.1f);

    if(key == GLUT_KEY_DOWN)
        frameCamera.MoveForward(-0.1f);

    if(key == GLUT_KEY_LEFT)
        frameCamera.RotateLocalY(0.1f);

    if(key == GLUT_KEY_RIGHT)
        frameCamera.RotateLocalY(-0.1f);

    // Refresh the Window
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
    glutPostRedisplay();
    glutTimerFunc(10, timerFunction, 1);
}

void DrawGround(void)
{
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat iStrip, iRun;

    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
    {
        glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0.0f, -0.4f, 0.0f);
            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
            {
                glVertex3f(iStrip, y, iRun);
                glVertex3f(iStrip + fStep, y, iRun);
            }
        glEnd();

    }
}

void DrawInhabitants(bool bShadow)
{
    GLint i;
    M3DMatrix44f actor;
    static GLfloat yRot = 0.0f;         // Rotation angle for animation
    yRot += 0.5f;

    if(bShadow == true)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }

    // Draw the randomly located spheres
    for(i = 0; i < NUM_SPHERES; i++)
    {
        if(bShadow == true)
            glColor3f(0.0f, 0.0f, 0.0f);
        else
            glColor3ub((GLubyte)vColor[i][0], (GLubyte)vColor[i][1], (GLubyte)vColor[i][2]);

        glPushMatrix();
            spheres[i].GetMatrix(actor);
            glMultMatrixf(actor);
            glutSolidSphere(0.1f, 36, 18);
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(0.0f, 0.20f, -2.5f);

        if(bShadow == 0)
        {
            // Torus alone will be specular
            glColor3ub((GLubyte)235, (GLubyte)172, (GLubyte)77);
            glMaterialfv(GL_FRONT, GL_SPECULAR, fSpecular);
        }

        glPushMatrix();
            glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f);
            glutSolidSphere(0.1f, 60, 120);
        glPopMatrix();

        if(bShadow == 0)
        {
            // Torus alone will be specular
            glColor3f(1.0f, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT, GL_SPECULAR, fSpecular);
        }

        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glutSolidTorus(0.10, 0.25, 90, 60);
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    glPopMatrix();
}

void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        frameCamera.ApplyCameraTransform();

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Draw the ground
        glColor3ub((GLubyte)240, (GLubyte)232, (GLubyte)211);
        DrawGround();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_FOG);
        glPushMatrix();
            glMultMatrixf(mShadowMat);
            DrawInhabitants(true);
        glPopMatrix();
        glEnable(GL_FOG);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);

        DrawInhabitants(false);

        //Draw sun at light position
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glTranslatef(-50.0f, 25.0f, 25.0f);
            glColor3ub(245, 214, 17);
            glutSolidSphere(3.0f, 36, 18);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    glPopMatrix();

    // Do the buffer Swap
    glutSwapBuffers();
}
