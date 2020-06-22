#include <GL/gl.h>
#include <GL/glut.h>

bool bEdgeFlag = false;
bool bOutline = false;

//This function does all the needed initialization
void SetupRC(void){
	//Set Background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Set Drawing color
	glColor3f(1.0f, 0.0f, 0.0f);
	
	//set front face to clockwise wound polygon
	glFrontFace(GL_CW);
}

void ProcessMenu(int value){
	switch(value){
		case 1: 
			bEdgeFlag = !bEdgeFlag;
			break;
		case 2:
			bOutline = !bOutline;
		default:
			break;
	}
	glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	
	if(h == 0)
		h = 1;
	
	glViewport(0, 0, w, h);
	
	//Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(w <= h)
		glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -100.0f, 100.0f);
	else
		glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -100.0f, 100.0f);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

void RenderScene(void){
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	
	glBegin(GL_TRIANGLES);
		glEdgeFlag(bEdgeFlag);
		glVertex2f(-20.0f, 0.0f);
		glEdgeFlag(true);
		glVertex2f(20.0f, 0.0f);
		glVertex2f(0.0f, 40.0f);
		
		glVertex2f(-20.0f, 0.0f);
		glVertex2f(-60.0f, -20.0f);
		glEdgeFlag(bEdgeFlag);
		glVertex2f(-20.0f,-40.0f);
		glEdgeFlag(true);
		
		glVertex2f(-20.0f,-40.0f);
		glVertex2f(0.0f, -80.0f);
		glEdgeFlag(bEdgeFlag);		
		glVertex2f(20.0f, -40.0f);
		glEdgeFlag(true);
		
		glVertex2f(20.0f, -40.0f);
		glVertex2f(60.0f, -20.0f);
		glEdgeFlag(bEdgeFlag);
		glVertex2f(20.0f, 0.0f);
		glEdgeFlag(true);
		
		//Centre square as two triangles
		glEdgeFlag(bEdgeFlag);
		glVertex2f(-20.0f, 0.0f);
		glVertex2f(-20.0f,-40.0f);
		glVertex2f(20.0f, 0.0f);
		
		glVertex2f(-20.0f,-40.0f);
		glVertex2f(20.0f, -40.0f);
		glVertex2f(20.0f, 0.0f);
		glEdgeFlag(true);
	glEnd();
	
	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("Polygon Stipple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle Edge Flag", 1);
	glutAddMenuEntry("Toggle Polygon outline", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	SetupRC();
	glutMainLoop();
	return 0;
}
