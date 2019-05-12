#include <windows.h>
#include <glut.h>
#include "Engine.h"

char windowTitle[] = "Computer Geometry Lab.";

GLint screenPosX = 50;
GLint screenPosY = 50;
GLint screenHeight = 700;
GLint screenWidth = 1400;

Engine engine = Engine(screenWidth, screenHeight, 700);

void WindowInitializing() {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // RGDA mode, double buffering, depth buffer
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(screenPosX, screenPosY);
	glutCreateWindow(windowTitle);
}

void RenderInitializing() {
	engine.InitialiningGL();
}
void DisplayFunc() {
	engine.DisplayAction();
}
void IdleFunc() {
	engine.IdleAction();
}
void ReshapeFunc(GLint w, GLint h) {
	engine.ReshapeAction(w, h);
}
void KeyboardFunc(unsigned char key, int x, int y) {
	engine.handleUsualKeys(key);
}
void SpecialFunc(int key, int x, int y) {
	engine.handleSpecialKeys(key);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	WindowInitializing();

	RenderInitializing();
	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}