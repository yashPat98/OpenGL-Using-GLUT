#include <GL/gl.h>
#include <GL/glut.h>

void SetupRC(void){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
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
	GLfloat y;
	GLint factor = 1;
	GLushort pattern = 0x5555;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_LINE_STIPPLE);
	
	for(y = -90.0f; y <= 90.0f; y += 20.0f){
		glLineStipple(factor, pattern);
		
		glBegin(GL_LINES);
			glVertex2f(-80.0f, y);
			glVertex2f(80.0f, y);
		glEnd();
		
		factor++;
	}
	
	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("Line Sizes");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	
	SetupRC();
	glutMainLoop();
	return 0;
}
