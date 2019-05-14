#pragma once

class Point {
public:
	GLdouble x, y, z;

	Point() : x(0), y(0), z(0) {}
	Point(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	bool friend operator== (const Point &p1, const Point &p2) {
		return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
	}
};
class Vector {
public:
	GLdouble x, y, z;

	Vector() : x(0), y(0), z(0) {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	Vector(Point p1, Point p2) {
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
		Normalize();
	}
	void Normalize() {
		GLdouble len = pow(x*x + y * y + z * z, 0.5);
		x /= len;
		y /= len;
		z /= len;
	}
};
class TriangleFace {
public:
	Point vertices[3];
	GLdouble a, b, c, d;
	GLint neighbouringFaces[3];

	TriangleFace(Point v1, Point v2, Point v3, GLint n_face1, GLint n_face2, GLint n_face3) {
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		neighbouringFaces[0] = n_face1;
		neighbouringFaces[1] = n_face2;
		neighbouringFaces[2] = n_face3;
		getSurfaceEquantion();
	}
	Vector getNormal() {
		Vector res = Vector(a, b, c);
		res.Normalize();
		return res;
	}

private:
	void getSurfaceEquantion() {
		Point p1 = vertices[0];
		Point p2 = vertices[1];
		Point p3 = vertices[2];

		GLdouble x_minor =  minor(p2.y - p1.y, p2.z - p1.z, p3.y - p1.y, p3.z - p1.z);
		GLdouble y_minor = -minor(p2.x - p1.x, p2.z - p1.z, p3.x - p1.x, p3.z - p1.z);
		GLdouble z_minor =  minor(p2.x - p1.x, p2.y - p1.y, p3.x - p1.x, p3.y - p1.y);

		a = x_minor;
		b = y_minor;
		c = z_minor;
		d = -(p1.x * x_minor + p1.y * y_minor + p1.z * z_minor);
	}
	GLdouble minor(GLdouble i00, GLdouble i01, GLdouble i10, GLdouble i11) {
		return i00 * i11 - i01 * i10;
	}
};

class MyMath
{
public:
	template <typename T> static GLint sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}
	static GLdouble pointDistance(Point p1, Point p2) {
		return pow(((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z)), 0.5);
	}

	static Vector vectorPlus(Vector vec1, Vector vec2) {
		return Vector(
			vec1.x + vec2.x,
			vec1.y + vec2.y,
			vec1.z + vec2.z
		);
	}
	static Vector vectorMinus(Vector vec1, Vector vec2) {
		return Vector(
			vec1.x - vec2.x,
			vec1.y - vec2.y,
			vec1.z - vec2.z
		);
	}
	static Vector vectorMyltiplyByScalar(Vector vec1, GLdouble a) {
		return Vector(
			vec1.x * a,
			vec1.y * a,
			vec1.z * a
			);
	}
	static Vector vectorCrossProduct(Vector vec1, Vector vec2) {
		return Vector(
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x
		);
	}
	static GLdouble vectorScalarProduct(Vector vec1, Vector vec2) {
		return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
	}

	static GLboolean trianglePointCrossInside(TriangleFace face, Point pivotPoint, Point point) {
		GLdouble pivotPos = pivotPoint.x * face.a + pivotPoint.y * face.b + pivotPoint.z * face.c + face.d;
		GLdouble pointPos = point.x * face.a + point.y * face.b + point.z * face.c + face.d;
		return sgn(pivotPos) == sgn(pointPos);
	}
	static GLdouble trianglePointDistance(TriangleFace face, Point point) {
		return (abs(face.a*point.x + face.b*point.y + face.c*point.z + face.d)) / (pow(face.a*face.a + face.b*face.b + face.c*face.c, 0.5));
	}
};

