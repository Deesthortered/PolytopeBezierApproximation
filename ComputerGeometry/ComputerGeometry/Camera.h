#pragma once
#include <math.h>
#include <glut.h>

class Camera {
	GLdouble rotare_scale = 0.1f;
	GLdouble step;

	Vector up;
	Vector position;
	Vector direction;

	GLdouble perspectiveAngle = 45.0f;
	GLdouble perspectiveZNear = 0.1f;
	GLdouble perspectiveZFar = 1000.0f;

public:
	Camera() {
		up = Vector(0.0f, 1.0f, 0.0f);
	}

	void SetPosition(GLdouble x, GLdouble y, GLdouble z) {
		position = Vector(x, y, z);
		setView();
	}
	void SetDirection(GLdouble x, GLdouble y, GLdouble z) {
		direction = Vector(x, y, z);
		setView();
	}
	void SetStep(GLdouble val) {
		step = val;
	}

	void ReshapeCamera(GLsizei width, GLsizei height) {
		if (height == 0) height = 1;
		GLdouble aspect = (GLdouble)width / (GLdouble)height;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(perspectiveAngle, aspect, perspectiveZNear, perspectiveZFar);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
		setView();
	}

	void MoveUp() {
		position = MyMath::vectorPlus(position , MyMath::vectorMyltiplyByScalar(up, step));
		setView();
	}
	void MoveDown() {
		position = MyMath::vectorMinus(position, MyMath::vectorMyltiplyByScalar(up, step));
		setView();
	}
	void MoveLeft() {
		Vector rightVector = MyMath::vectorCrossProduct(direction, up);
		position = MyMath::vectorMinus(position, MyMath::vectorMyltiplyByScalar(rightVector, step));
		setView();
	}
	void MoveRight() {
		Vector rightVector = MyMath::vectorCrossProduct(direction, up);
		position = MyMath::vectorPlus(position, MyMath::vectorMyltiplyByScalar(rightVector, step));
		setView();
	}
	void MoveForward() {
		position = MyMath::vectorPlus(position, MyMath::vectorMyltiplyByScalar(direction, step));
		setView();
	}
	void MoveBack() {
		position = MyMath::vectorMinus(position, MyMath::vectorMyltiplyByScalar(direction, step));
		setView();
	}

	void RotareUp() {
		direction = MyMath::vectorPlus(direction, MyMath::vectorMyltiplyByScalar(up, step * rotare_scale));
		direction.Normalize();
		setView();
	}
	void RotareDown() {
		direction = MyMath::vectorMinus(direction, MyMath::vectorMyltiplyByScalar(up, step * rotare_scale));
		direction.Normalize();
		setView();
	}
	void RotareLeft() {
		Vector leftVector = MyMath::vectorCrossProduct(direction, up);
		direction = MyMath::vectorMinus(direction, MyMath::vectorMyltiplyByScalar(leftVector, step * rotare_scale));
		direction.Normalize();
		setView();
	}
	void RotareRight() {
		Vector leftVector = MyMath::vectorCrossProduct(direction, up);
		direction = MyMath::vectorPlus(direction, MyMath::vectorMyltiplyByScalar(leftVector, step * rotare_scale));
		direction.Normalize();
		setView();
	}

private:
	void setView() {
		gluLookAt(
			position.x, position.y, position.z,
			position.x + direction.x, position.y + direction.y, position.z + direction.z,
			up.x, up.y, up.z
		);
	}
};