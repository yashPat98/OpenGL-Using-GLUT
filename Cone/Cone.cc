#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#define GL_PI 3.1415f
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
bool bCull = false;
bool bDepth = false;
bool bOutline = false;

//This function does any needed initializations.
void SetupRC(void){
	//Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set drawing color
	glColor3f(1.0f, 0.0f, 0.0f);

	//set color shading model to flat
	glShadeModel(GL_FLAT);

	//Clockwise-wound polygons are set to front facing
	//because we are using trinangle fans
	glFrontFace(GL_CW);
}

void ChangeSize(GLsizei w, GLsizei h){
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	//Reset Coordination system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w <= h)
		glOrtho(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -100.0f, 100.0f);
	else
		glOrtho(-100.0f * aspectRatio, 100.0f * aspectRatio, -100.0f, 100.0f, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SpecialKeys(int key, int x, int y){
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

	glutPostRedisplay(); //Call RenderScene to Draw again....(Refresh)
}

void ProcessMenu(int value){
	switch(value){
		case 1:
			bDepth = !bDepth;
			break;
		case 2 :
			bCull = !bCull;
			break;
		case 3:
			bOutline = !bOutline;
		default:
			break;
	}
	glutPostRedisplay(); //Call RenderScene to draw again
}

//Called to draw scene
void RenderScene(void){
	GLfloat x, y, angle;
	int iPivot = 1;			//Used to flag alternating colors

	//clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Turn culling on if flag is set
	if(bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	//Enable depth testing if flag is set
	if(bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//Draw the back side as wireframe only, if flag is set
	if(bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	//Save Matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	//Begin a triangle fan
	glBegin(GL_TRIANGLE_FAN);

	//pinnacle of cone is shared vertex for fan, moved up Z-axis to produce cone
	glVertex3f(0.0f, 0.0f, 75.0f);

	//Loop around in circle and specify even points along the circle as vertices of triangle
	for(angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f)){
		//calculate x and y position for next vertex
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);

		//Alternate color between red and blue
		if((iPivot % 2) == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f, 0.0f, 1.0f);

		//increment pivot to change color next time
		iPivot++;

		//Specify next vertex
		glVertex2f(x, y);
	}
	//Done drawing for cone
	glEnd();

	//Begin a new triangle fan to cover the bottom
	glBegin(GL_TRIANGLE_FAN);

	//Center of fan is at origin
	glVertex3f(0.0f, 0.0f, 0.0f);

	//Loop around circle and specify even points
	for(angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f)){
		//Calculate x and y position of next vertex
		x = 50 * sin(angle);
		y = 50 * cos(angle);

		//Alternate color between red and blue
		if((iPivot % 2) == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f, 0.0f, 1.0f);

		//increment pivot to change color next time
		iPivot++;

		//Specify the next vertex for triangle fan
		glVertex2f(x, y);

	}
	//Done drawing fan that covers bottom
	glEnd();

	//Restore transformations
	glPopMatrix();

	//Flush and swap buffers
	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("3D-Triangle");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);

	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle depth test", 1);
	glutAddMenuEntry("Toggle Cull backface", 2);
	glutAddMenuEntry("Toggle outline back", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutMainLoop();
	return 0;
}
