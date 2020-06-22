#include <GL/gl.h>
#include <GL/glut.h>

//Bitmap of Campfire
GLubyte fire[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x01, 0xf0,
0x00, 0x00, 0x07, 0xf0, 0x0f, 0x00, 0x1f, 0xe0,
0x1f, 0x80, 0x1f, 0xc0, 0x0f, 0xc0, 0x3f, 0x80,
0x07, 0xe0, 0x7e, 0x00, 0x03, 0xf0, 0xff, 0x80,
0x03, 0xf5, 0xff, 0xe0, 0x07, 0xfd, 0xff, 0xf8,
0x1f, 0xfc, 0xff, 0xe8, 0xff, 0xe3, 0xbf, 0x70,
0xde, 0x80, 0xb7, 0x00, 0x71, 0x10, 0x4a, 0x80,
0x03, 0x10, 0x4e, 0x40, 0x02, 0x88, 0x8c, 0x20,
0x05, 0x05, 0x04, 0x40, 0x02, 0x82, 0x14, 0x40,
0x02, 0x40, 0x10, 0x80, 0x02, 0x64, 0x1a, 0x80,
0x00, 0x92, 0x29, 0x00, 0x00, 0xb0, 0x48, 0x00,
0x00, 0xc8, 0x90, 0x00, 0x00, 0x85, 0x10, 0x00,
0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00 };

//This function does all the needed initialization
void SetupRC(void){
	//Set Background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Set Drawing color
	glColor3f(1.0f, 0.0f, 0.0f);
	
	//enable plygon stippling
	glEnable(GL_POLYGON_STIPPLE);
	
	//Specify stippling pattern (GLubyte *)
	glPolygonStipple(fire);
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
	
	//Begin the stop sign polygon
	glBegin(GL_POLYGON);
		glVertex2f(-20.0f, 50.0f);
		glVertex2f(20.0f, 50.0f);
		glVertex2f(50.0f, 20.0f);
		glVertex2f(50.0f, -20.0f);
		glVertex2f(20.0f, -50.0f);
		glVertex2f(-20.0f, -50.0f);
		glVertex2f(-50.0f, -20.0f);
		glVertex2f(-50.0f, 20.0f);
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
	
	SetupRC();
	glutMainLoop();
	return 0;
}
