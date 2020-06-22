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
	GLfloat y, step;
	GLfloat fSizes[2];
	GLfloat fCurrSize;
	
	glGetFloatv(GL_LINE_WIDTH_RANGE, fSizes);
	//glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &step);
	fCurrSize = fSizes[0];
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(y = -90.0f; y < 90.0f; y += 20.0f){
		glLineWidth(fCurrSize);
		glBegin(GL_LINES);
			glVertex2f(-80.0f, y);
			glVertex2f(80.0f, y);
		glEnd();
		fCurrSize += 1.0f;
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
