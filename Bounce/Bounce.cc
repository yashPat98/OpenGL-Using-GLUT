#include <GL/gl.h>
#include <GL/glut.h>

//Initial square position and size
GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 25;

//Step size in x and y directions
// (number of pixels to move each time)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

//keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

//Called to draw scene
void RenderScene(void){
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Set current drawing color
	glColor3f(1.0f, 0.0f, 0.0f);
	
	//Draw a filled rectangle with current color
	glRectf(x1, y1, x1 + rsize, y1 - rsize);
	
	//Flush drawing commands and swap
	glutSwapBuffers();
}

//Called by GLUT library when idle (window not being resized or moved)
void TimerFunction(int value){
	//Reverse direction when you reach left or right edge
	if(x1 > windowWidth - rsize || x1 < -windowWidth)
		xstep = -xstep;
		
	//Reverse direction when you reach top or bottom edge
	if(y1 > windowHeight || y1 < -windowHeight + rsize)
		ystep = -ystep;
		
	//Actually move the square
	x1 += xstep;
	y1 += ystep;
	
	//Check bounds. This is in case the window is made smaller
	// while the rectangle is bouncing and rectangle suddenly finds itself
	//outside new clipping volume
	if(x1 > (windowWidth-rsize + xstep))
		x1 = windowWidth - rsize - 1;
	else if(x1 < - (windowWidth + xstep))
		x1 = - windowWidth - 1;
		
	if(y1 > (windowHeight + ystep))
		y1 = windowHeight - 1;
	else if(y1 < - (windowHeight - rsize + ystep))
		y1 = -windowHeight + rsize - 1;
		
	//Redraw the scene with new coordinates
	glutPostRedisplay();                     //RenderScene function should be called
	glutTimerFunc(33, TimerFunction, 1);     //Reset TimerFunction 
}

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio;
	
	if(h == 0)
	    h = 1;
	   
	//Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	
	//Reset coordination system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	aspectRatio = (GLfloat)w / (GLfloat)h;
	
	if(w <= h){
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -100.0 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	}
	else {
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC(void){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char * argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Bounce");
	glutDisplayFunc(RenderScene);  //called whenever we need to draw scene
	glutReshapeFunc(ChangeSize);   //called atleast once 
	glutTimerFunc(33, TimerFunction, 1);   //Triggers function after 33msecs
	
	SetupRC();
	glutMainLoop();
	
	return 0;
}













