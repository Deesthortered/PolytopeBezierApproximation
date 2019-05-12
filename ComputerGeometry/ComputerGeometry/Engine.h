#pragma once
#include <glut.h>
#include <ctime>
#include <vector>
#include <stack>
#include "Color.h"
#include "MyMath.h"
#include "Camera.h"
#include "QuickHull3DAlgorithm.h"
#include "BezierSurfaceAlgorithm.h"
using namespace std;

class Engine {
	GLint spaceWidth;
	GLint spaceHeight;
	GLint spaceDepth;
	Color color;

	Point startPointBorder = Point(50, 50, 50);
	GLint pointCount = 50;
	vector<Point> startPoints;
	vector<TriangleFace> hull;

public:
	Camera camera = Camera();
	Engine(GLdouble spWidth, GLdouble spHeight, GLdouble spDepth) {
		srand(time(NULL));
		this->spaceWidth = spWidth;
		this->spaceHeight = spHeight;
		this->spaceDepth = spDepth;
	}

	void InitialiningGL() {
		glClearColor(155 / 255.0, 155 / 255.0, 155 / 255.0, 1.0); // Background color
		glClearDepth(1.0f);									// Set background depth to farthest
		glEnable(GL_DEPTH_TEST);							// Enable depth testing for z-culling
		glDepthFunc(GL_LEQUAL);								// Set the type of depth-test
		glShadeModel(GL_SMOOTH);							// Enable smooth shading
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

		float lineWidth[2] = { 3.0f, 20.0f };
		glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
		glLineWidth(8.0f);
		
		camera.SetDirection(-0.395723104f, 0.0666912124f, 0.915945113f);
		camera.SetPosition(56.0f, 19.0f, -51.0f);
		camera.SetStep(1.0f);
		camera.ReshapeCamera(spaceWidth, spaceHeight);
	}
	void DisplayAction() {
		DrawAll();
	}
	void IdleAction() {
		DrawAll();
	}
	void ReshapeAction(GLint w, GLint h) {
		camera.ReshapeCamera(w, h);
	}
	void handleUsualKeys(unsigned char key) {
		if (key == 27) // Escape
			exit(0);
		else if (key == 'W' || key == 'w')
			camera.MoveForward();
		else if (key == 'S' || key == 's')
			camera.MoveBack();
		else if (key == 'A' || key == 'a')
			camera.MoveLeft();
		else if (key == 'D' || key == 'd')
			camera.MoveRight();
		else if (key == 'Q' || key == 'q')
			camera.MoveUp();
		else if (key == 'E' || key == 'e')
			camera.MoveDown();

		else if (key == 'I' || key == 'i')
			camera.RotareUp();
		else if (key == 'K' || key == 'k')
			camera.RotareDown();
		else if (key == 'J' || key == 'j')
			camera.RotareLeft();
		else if (key == 'L' || key == 'l')
			camera.RotareRight();

		else if (key == 'R' || key == 'r') {
			ReloadAll();
		}
	}
	void handleSpecialKeys(int key) {
		switch (key) {
		case GLUT_KEY_F1:
			break;
		case GLUT_KEY_F2:
			break;
		case GLUT_KEY_F3:
			break;
		case GLUT_KEY_F4:
			break;
		case GLUT_KEY_F5:
			break;
		case GLUT_KEY_F6:
			break;
		case GLUT_KEY_F7:
			break;
		case GLUT_KEY_F8:
			break;
		case GLUT_KEY_F9:
			break;
		case GLUT_KEY_F10:
			break;
		case GLUT_KEY_F11:
			break;
		case GLUT_KEY_F12:
			break;
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;
		case GLUT_KEY_INSERT:
			break;
		}

		int m = glutGetModifiers();
		if (m == GLUT_ACTIVE_SHIFT) {};
	}

private:
	void DrawAll() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.ReshapeCamera(spaceWidth, spaceHeight);

		DrawAxis();
		DrawPoints(startPoints, color.blackColor);
		DrawPolytop(hull);
		DrawLines(QuickHull3DAlgorithm::horizon, color.redColor);

		glutSwapBuffers();
	}

	void DrawAxis() {
		glBegin(GL_LINES);

		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d((spaceWidth >> 1), 0.0, 0.0);

		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, (spaceHeight >> 1), 0.0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, (spaceDepth >> 1));

		glEnd();
	}
	void DrawPoints(vector<Point> &Points, GLfloat *color) {
		for (GLuint i = 0; i < Points.size(); i++) {
			DrawPoint(Points[i], color);
		}
	}
	void DrawPolytop(vector<TriangleFace> polytop) {
		for (size_t i = 0; i < polytop.size(); i++) {
			DrawTrinagle(polytop[i], color.orangeColor, color.blackColor);
		}
	}
	void DrawLines(vector<Point> lines, GLfloat *color) {
		glColor3f(color[0], color[1], color[2]);
		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < lines.size(); i++) {
			glVertex3d(lines[i].x, lines[i].y, lines[i].z);
		}
		glEnd();
	}

	void DrawPoint(Point p, GLfloat *color) {
		glColor3f(color[0], color[1], color[2]);
		GLUquadric *quad;
		quad = gluNewQuadric();
		glTranslatef(p.x, p.y, p.z);
		gluSphere(quad, 0.5f, 30, 30);
		glTranslatef(-p.x, -p.y, -p.z);
	}
	void DrawTrinagle(TriangleFace triangle, GLfloat *colorFace, GLfloat *colorEdges) {
		glColor3f(colorFace[0], colorFace[1], colorFace[2]);
		/*
		glBegin(GL_TRIANGLES);
		glVertex3d(triangle.vertices[0].x, triangle.vertices[0].y, triangle.vertices[0].z);
		glVertex3d(triangle.vertices[1].x, triangle.vertices[1].y, triangle.vertices[1].z);
		glVertex3d(triangle.vertices[2].x, triangle.vertices[2].y, triangle.vertices[2].z);
		glEnd();
		*/
		glColor3f(colorEdges[0], colorEdges[1], colorEdges[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3d(triangle.vertices[0].x, triangle.vertices[0].y, triangle.vertices[0].z);
		glVertex3d(triangle.vertices[1].x, triangle.vertices[1].y, triangle.vertices[1].z);
		glVertex3d(triangle.vertices[2].x, triangle.vertices[2].y, triangle.vertices[2].z);
		glVertex3d(triangle.vertices[0].x, triangle.vertices[0].y, triangle.vertices[0].z);
		glEnd();
	}
	void DrawSurfaceLines(vector<Point> &points, GLfloat *color) {
		if (points.empty()) return;
		size_t len = pow(points.size(), 0.5);
		glBegin(GL_LINES);
		glColor3f(color[0], color[1], color[2]);
		for (size_t i = 0; i < len - 1; i++) {
			for (size_t j = 0; j < len - 1; j++) {
				Point p = points[len*i + j];

				Point p1 = points[len*i + j + 1];
				Point p2 = points[len*(i + 1) + j];
				Point p3 = points[len*(i + 1) + j + 1];

				glVertex3d(p.x, p.y, p.z);
				glVertex3d(p1.x, p1.y, p1.z);

				glVertex3d(p.x, p.y, p.z);
				glVertex3d(p2.x, p2.y, p2.z);

				glVertex3d(p.x, p.y, p.z);
				glVertex3d(p3.x, p3.y, p3.z);
			}
		}
		for (size_t i = 0; i < len - 1; i++) {
			Point p1 = points[len*(len - 1) + i];
			Point p2 = points[len*(len - 1) + i + 1];

			glVertex3d(p1.x, p1.y, p1.z);
			glVertex3d(p2.x, p2.y, p2.z);
		}
		for (size_t i = 0; i < len - 1; i++) {
			Point p3 = points[(len - 1) + len*i];
			Point p4 = points[(len - 1) + len*(i + 1)];

			glVertex3d(p3.x, p3.y, p3.z);
			glVertex3d(p4.x, p4.y, p4.z);
		}
		glEnd();
	}
	
	void ReloadAll() {
		startPoints = getStartPoints();
		hull = QuickHull3DAlgorithm::getConvexHull(startPoints);
	}
	vector<Point> getStartPoints() {
		vector<Point> result = vector<Point>();
		
		result.push_back(Point(10, 10, 10));
		result.push_back(Point(50, 10, 10));
		result.push_back(Point(25, 10, 50));
		result.push_back(Point(25, 50, 25));

		for (int i = 0; i < 1; i++) {
			result.push_back(
				Point(
					rand() % (GLint)startPointBorder.x,
					rand() % (GLint)startPointBorder.y,
					rand() % (GLint)startPointBorder.z
					)
			);
		}
		
		return result;
	}
};