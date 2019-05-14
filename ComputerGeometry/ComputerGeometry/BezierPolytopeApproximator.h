class BezierTriangle {
	Point p1, p2, p3;
	vector<vector<Point>> bezierNet;
	Vector normal;
	Point basicCenter;

	GLdouble curveDegree = 0;
	size_t resolution = 3;

public:
	BezierTriangle(Point p1, Point p2, Point p3, Vector normal) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->normal = normal;

		makeNet();
		makeCurve();

		basicCenter = bezierNet[bezierNet.size()/2][bezierNet.size() / 2];
	}
	void setCurveDegree(GLdouble val) {
		curveDegree = val;
		makeCurve();
	}
	void setResolution(size_t val) {
		resolution = val;
		makeNet();
	}
	vector<vector<Point>> getBezierNet() {
		return bezierNet;
	}
	Point getComparePoint() {
		return Point(basicCenter.x + normal.x, basicCenter.y + normal.y, basicCenter.z + normal.z);
	}
	void inverseNormal() {
		normal.x = -normal.x;
		normal.y = -normal.y;
		normal.z = -normal.z;
	}
private:

	void makeNet() {
		bezierNet = vector<vector<Point>>(resolution);
		for (size_t i = 0; i < bezierNet.size(); i++)
			bezierNet[i] = vector<Point>(resolution);

		GLdouble dist12 = MyMath::pointDistance(p1, p2);
		GLdouble dist13 = MyMath::pointDistance(p1, p3);
		GLdouble dist23 = MyMath::pointDistance(p2, p3);

		Vector vec12 = Vector(p1, p2);
		Vector vec13 = Vector(p1, p3);
		Vector vec23 = Vector(p2, p3);

		for (size_t i = 0; i < resolution; i++) {
			bezierNet[0][i] = Point(
				p1.x + i * vec13.x * dist13 / (resolution),
				p1.y + i * vec13.y * dist13 / (resolution),
				p1.z + i * vec13.z * dist13 / (resolution)
				);
			bezierNet[resolution - 1][i] = Point(
				p2.x + i * vec23.x * dist23 / (resolution),
				p2.y + i * vec23.y * dist23 / (resolution),
				p2.z + i * vec23.z * dist23 / (resolution)
				);
		}

		for (size_t i = 1; i < resolution - 1; i++) {
			GLdouble curr_dist = MyMath::pointDistance(bezierNet[0][i], bezierNet[resolution - 1][i]);
			for (size_t j = 1; j < resolution - 1; j++) {
				bezierNet[i][j] = Point(
					p1.x + i * vec13.x*dist13 + j * vec12.x*curr_dist / (resolution),
					p1.y + i * vec13.y*dist13 + j * vec12.y*curr_dist / (resolution),
					p1.z + i * vec13.z*dist13 + j * vec12.z*curr_dist / (resolution)
				);
			}
		}


	}
	void makeCurve() {

	}
};

class BezierPolytopeApproximator {
public:
	static vector<BezierTriangle> getBezierTriangles(vector<TriangleFace> &polytopeTriangles) {
		vector<BezierTriangle> result;
		for (size_t i = 0; i < polytopeTriangles.size(); i++) {
			BezierTriangle tr = BezierTriangle(polytopeTriangles[i].vertices[0], polytopeTriangles[i].vertices[1], polytopeTriangles[i].vertices[2], polytopeTriangles[i].getNormal());
			if (MyMath::trianglePointCrossInside(polytopeTriangles[i], tr.getComparePoint(), QuickHull3DAlgorithm::pivotPoint))
				tr.inverseNormal();
			result.push_back(tr);
		}
		return result;
	}
};