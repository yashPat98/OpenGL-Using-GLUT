#include <GL/gl.h>
#include <GL/glut.h>

//Called to draw scene
void RenderScene(void){
	glClear(GL_COLOR_BUFFER_BIT);  //Clear the window with current clearing color

	glFlush();  //Flush drawing commands
}

//Setup Rendering state
void SetupRC(void){
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
}


int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Simple Window");
	glutDisplayFunc(RenderScene);

	SetupRC();
	glutMainLoop();

	return 0;
}
