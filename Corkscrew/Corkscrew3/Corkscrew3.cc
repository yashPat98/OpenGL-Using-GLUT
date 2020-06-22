#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>

#define GL_PI 3.1415f
static GLfloat xRot = 45.0f;
static GLfloat yRot = 45.0f;

//This function does needed initialization on the rendering context
void SetupRC(void){
	//Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Set Drawing color
	glColor3f(1.0f, 1.0f, 1.0f);
}

//Called to draw scene
void RenderScene(void){
	GLfloat x, y, z, angle;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Save matrix state and do rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	
	//Call only once for all remaining points
	glBegin(GL_LINE_STRIP);
		z = -50.0f;
		for(angle = 0.0f; angle <= (2.0f * GL_PI) * 3.0f; angle += 0.1f){
			x = 50.0f * sin(angle);
			y = 50.0f * cos(angle);
			
			//Specify the points and move the z value up a little
			glVertex3f(x, y, z);
			z += 0.5f;
		}
	glEnd();
	
	//Restore Transformation 
	glPopMatrix();

	//Flush drawing commands
	glutSwapBuffers();	
}

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	
	if(h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	
	//Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(w <= h)
		glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -100.0f, 100.0f);
	else
		glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -100.0f, 100.0f);
		
	//Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE  | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Corkscrew");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	
	SetupRC();
	glutMainLoop();
	
	return 0;
}















