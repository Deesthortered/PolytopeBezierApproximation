#pragma once
#include <glut.h>
#include <vector>
using namespace std;
typedef unsigned long long ull;

class BezierSurfaceAlgorithm {
public:
	static GLint surfaceResolution;

	static vector<Point> getBezierSurface(vector<vector<Point>> lines) {
		if (lines.empty()) return vector<Point>();
		size_t len = lines.size();

		vector<vector<Point>> resultLines = vector<vector<Point>>(len);
		for (int i = 0; i < surfaceResolution + 1; i++) {
			for (size_t j = 0; j < lines.size(); j++) {
				resultLines[j].push_back(BezierDeCasteljau(lines[j], (double(i))/surfaceResolution) );
			}
		}

		vector<Point> result = vector<Point>();
		for (int i = 0; i < surfaceResolution + 1; i++) {
			vector<Point> tmpline = vector<Point>();
			for (size_t j = 0; j < lines.size(); j++) {
				tmpline.push_back(resultLines[j][i]);
			}
			for (size_t j = 0; j < (size_t)(surfaceResolution + 1); j++) {
				result.push_back(BezierDeCasteljau(tmpline, (double(j)) / surfaceResolution));
			}
		}
		return result;
	}
	static Point BezierDeCasteljau(vector<Point> points, double u) {
		int n = (int)points.size();
		for (int k = 1; k < n; k++) {
			for (int i = 0; i < (n - k); i++) {
				points[i].z = static_cast<float>((1.0 - u) * points[i].z + u * points[i + 1].z);
				points[i].y = static_cast<float>((1.0 - u) * points[i].y + u * points[i + 1].y);
				points[i].x = static_cast<float>((1.0 - u) * points[i].x + u * points[i + 1].x);
			}
		}
		return points[0];
	}
};
GLint BezierSurfaceAlgorithm::surfaceResolution = 3; 