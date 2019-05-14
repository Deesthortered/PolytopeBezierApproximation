class BezierTriangle {
	Point p1, p2, p3;
	vector<vector<Point>> bezierNet;
	Vector normal;
	Point basicCenter;

	GLdouble curveDegree = 0;
	size_t resolution = 4;

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

		Vector v12 = Vector(p1, p2);
		Vector v13 = Vector(p1, p3);
		Vector v23 = Vector(p2, p3);

		GLdouble d12 = MyMath::pointDistance(p1, p2);
		GLdouble d13 = MyMath::pointDistance(p1, p3);
		GLdouble d23 = MyMath::pointDistance(p2, p3);

		bezierNet[0][0] = p1;
		bezierNet[0][resolution-1] = p2;
		for (size_t j = 0; j < resolution; j++)
			bezierNet[resolution-1][j] = p3;

		for (size_t i = 1; i < resolution-1; i++) {
			bezierNet[i][0] = Point(
				p1.x + i * v13.x * d13 / (resolution - 1),
				p1.y + i * v13.y * d13 / (resolution - 1),
				p1.z + i * v13.z * d13 / (resolution - 1)
				);
			bezierNet[i][resolution-1] = Point(
				p2.x + i * v23.x * d23 / (resolution - 1),
				p2.y + i * v23.y * d23 / (resolution - 1),
				p2.z + i * v23.z * d23 / (resolution - 1)
			);
		}

		for (size_t i = 0; i < resolution - 1; i++) {
			GLdouble curr_dis = MyMath::pointDistance(bezierNet[i][0], bezierNet[i][resolution-1]) / (resolution - 1);
			for (size_t j = 1; j < resolution - 1; j++)
				bezierNet[i][j] = Point(
					bezierNet[i][0].x + j * v12.x * curr_dis,
					bezierNet[i][0].y + j * v12.y * curr_dis,
					bezierNet[i][0].z + j * v12.z * curr_dis
				);
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