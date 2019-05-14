class BezierTriangle {
	Point p1, p2, p3;
	vector<vector<Point>> bezierNet;
	Vector normal;
	Point basicCenter;

public:
	BezierTriangle(Point p1, Point p2, Point p3, Vector normal) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->normal = normal;

		bezierNet = vector<vector<Point>>(3);
		for (size_t i = 0; i < bezierNet.size(); i++)
			bezierNet[i] = vector<Point>(3);

		bezierNet[0][0] = p1;
		bezierNet[0][2] = p2;

		bezierNet[2][0] = p3;
		bezierNet[2][1] = p3;
		bezierNet[2][2] = p3;

		bezierNet[0][1] = Point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0, (p1.z + p2.z) / 2.0);
		bezierNet[1][0] = Point((p1.x + p3.x) / 2.0, (p1.y + p3.y) / 2.0, (p1.z + p3.z) / 2.0);
		bezierNet[1][2] = Point((p2.x + p3.x) / 2.0, (p2.y + p3.y) / 2.0, (p2.z + p3.z) / 2.0);

		bezierNet[1][1] = Point((p1.x + p2.x + p3.x) / 3.0, (p1.y + p2.y + p3.y) / 3.0, (p1.z + p2.z + p3.z) / 3.0);

		basicCenter = bezierNet[1][1];
	}
	void setCurveDegree(GLdouble val) {
		bezierNet[1][1] = Point(basicCenter.x + normal.x*val, basicCenter.y + normal.y*val, basicCenter.z + normal.z*val);
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