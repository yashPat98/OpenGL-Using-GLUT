#include <GL/gl.h>
#include <GL/glut.h>

bool bFull = false;

int main(int argc, char** argv)
{
    //Function prototypes
    void SetupRC(void);
    void ChangeSize(GLsizei, GLsizei);
    void RenderScene(void);
    void keyboard(unsigned char, int, int);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLUT: Yash Patel");

    SetupRC();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return (0);
}

void SetupRC()
{
    //Set background clearing color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    if(height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
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

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_TRIANGLES);
    	glColor3f(0.0f, 1.0f, 0.0f);
    	glVertex3f(0.0f, 0.3f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(0.5f, -0.5f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(-0.5f, -0.5f, 0.0f);

    
    	glColor3f(0.0f, 1.0f, 0.0f);
    	glVertex3f(0.0f, 0.7f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(0.4f, 0.0f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(-0.4f, 0.0f, 0.0f);	
    	
    
    	glColor3f(0.0f, 1.0f, 0.0f);
    	glVertex3f(0.0f, 1.0f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(-0.3f, 0.5f, 0.0f);

    	glColor3f(0.0f, 0.5f, 0.0f);
    	glVertex3f(0.3f, 0.5f, 0.0f);
  
    glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.4f, 0.26f, 0.13f);
		glVertex3f(-0.05f, -0.5f, 0.0f);
		glVertex3f(0.05f, -0.5f, 0.0f);
		glVertex3f(0.05f, -1.0f, 0.0f);
		glVertex3f(-0.05f, -1.0f, 0.0f);
	glEnd();

    glutSwapBuffers();
}
