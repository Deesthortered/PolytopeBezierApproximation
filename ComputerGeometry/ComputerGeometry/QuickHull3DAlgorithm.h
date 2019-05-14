#pragma once
using namespace std;

class QuickHull3DAlgorithm {
public:
	static Point pivotPoint;

	static vector<TriangleFace> getConvexHull(vector<Point> startPoints) {
		if (startPoints.size() < 4) return vector<TriangleFace>();
		TriangleFace startTriangle = getStartTrinangle(startPoints);
		vector<TriangleFace> faces = getStartPolytop(startTriangle, startPoints);

		for (size_t i = 0; i < faces.size(); i++) {
			if (startPoints.empty()) break;
			GLdouble distance = 0;
			size_t ind = 0;
			bool found = false;
			for (size_t j = 0; j < startPoints.size(); j++) {
				if (!MyMath::trianglePointCrossInside(faces[i], pivotPoint, startPoints[j])) {
					found = true;
					GLdouble current_distance = MyMath::trianglePointDistance(faces[i], startPoints[j]);
					if (current_distance > distance) {
						ind = j;
						distance = current_distance;
					}
				}
			}
			if (found) {
				vector<char> used_faces = vector<char>(faces.size(), false);
				vector<size_t> edge_faces = vector<size_t>();
				vector<Point> horizon;
				try { horizon = getHorizonPoints(faces, used_faces, i, startPoints[ind], edge_faces); }
				catch (int e) { throw e; }
				makeNewFaces(faces, used_faces, startPoints[ind], edge_faces, horizon);
				startPoints.erase(startPoints.begin() + ind);
				i--;
			}
		}
		return faces;
	}
private:

	static TriangleFace getStartTrinangle(vector<Point> &startPoints) {
		size_t left = 0;
		size_t right = 0;
		size_t top = 0;

		for (size_t i = 0; i < startPoints.size(); i++) {
			if (startPoints[i].x < startPoints[left].x)
				left = i;
			if (startPoints[i].x > startPoints[right].x)
				right  = i;
			if (startPoints[i].y > startPoints[top].y)
				top = i;
		}
		TriangleFace result = TriangleFace(startPoints[left], startPoints[right], startPoints[top], 1, 2, 3);

		size_t min_ind = min(min(left, right), top);
		size_t max_ind = max(max(left, right), top);
		size_t midlle_ind = (left == min_ind || left == max_ind ? (right == min_ind || right == max_ind ? top : right) : left);

		startPoints.erase(startPoints.begin() + max_ind);
		startPoints.erase(startPoints.begin() + midlle_ind);
		startPoints.erase(startPoints.begin() + min_ind);

		return result;
	}
	static vector<TriangleFace> getStartPolytop(TriangleFace &startTriangle, vector<Point> &startPoints) {
		vector<TriangleFace> startPolytop;
		startPolytop.push_back(startTriangle);

		GLdouble distance = MyMath::trianglePointDistance(startTriangle, startPoints[0]);
		size_t ind = 0;
		for (size_t i = 0; i < startPoints.size(); i++) {
			GLdouble current_distance = MyMath::trianglePointDistance(startTriangle, startPoints[i]);
			if (current_distance > distance) {
				distance = current_distance;
				ind = i;
			}
		}

		startPolytop.push_back(TriangleFace(startTriangle.vertices[0], startTriangle.vertices[1], startPoints[ind], 0, 2, 3));
		startPolytop.push_back(TriangleFace(startTriangle.vertices[1], startTriangle.vertices[2], startPoints[ind], 0, 3, 1));
		startPolytop.push_back(TriangleFace(startTriangle.vertices[2], startTriangle.vertices[0], startPoints[ind], 0, 1, 2));

		pivotPoint = Point(
			(startTriangle.vertices[0].x + startTriangle.vertices[1].x + startTriangle.vertices[2].x + startPoints[ind].x) / 4.0,
			(startTriangle.vertices[0].y + startTriangle.vertices[1].y + startTriangle.vertices[2].y + startPoints[ind].y) / 4.0,
			(startTriangle.vertices[0].z + startTriangle.vertices[1].z + startTriangle.vertices[2].z + startPoints[ind].z) / 4.0
		);

		startPoints.erase(startPoints.begin() + ind);

		for (size_t i = 0; i < startPoints.size(); i++) {
			bool inside = true;
			for (size_t j = 0; j < startPolytop.size(); j++) {
				if (!MyMath::trianglePointCrossInside(startPolytop[j], pivotPoint, startPoints[i])) {
					inside = false;
					break;
				}
			}
			if (inside) {
				startPoints.erase(startPoints.begin() + i);
				i--;
			}
		}

		return startPolytop;
	}

	static vector<Point> getHorizonPoints(vector<TriangleFace> &faces, vector<char> &used_faces, size_t face_ind, Point found_point, vector<size_t> &edge_faces) {
		used_faces[face_ind] = true;

		vector<RecursiveStackParams> start_params = vector<RecursiveStackParams>(3);

		start_params[0].face_index = faces[face_ind].neighbouringFaces[0];
		start_params[1].face_index = faces[face_ind].neighbouringFaces[1];
		start_params[2].face_index = faces[face_ind].neighbouringFaces[2];

		start_params[0].from = face_ind;
		start_params[1].from = face_ind;
		start_params[2].from = face_ind;

		for (size_t i = 0; i < 3; i++) {
			vector<size_t> indicies;
			for (size_t j = 0; j < 3; j++)
				for (size_t k = 0; k < 3; k++) {
					if (faces[face_ind].vertices[j] == faces[faces[face_ind].neighbouringFaces[i]].vertices[k])
						indicies.push_back(j);
					if (indicies.size() > 2) throw 1;
				}
			if (indicies.size() != 2) throw 2;
			start_params[i].start_point1 = faces[face_ind].vertices[indicies[0]];
			start_params[i].start_point2 = faces[face_ind].vertices[indicies[1]];
		}

		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 1; j < 3; j++) {
				if (start_params[i].start_point2 == start_params[(i + j) % 3].start_point2) {
					if (start_params[(i + 3 - j) % 3].start_point2 == start_params[i].start_point1) {
						Point tmp = start_params[(i + j) % 3].start_point2;
						start_params[(i + j) % 3].start_point2 = start_params[(i + j) % 3].start_point1;
						start_params[(i + j) % 3].start_point1 = tmp;
					}
					else {
						Point tmp = start_params[i].start_point2;
						start_params[i].start_point2 = start_params[i].start_point1;
						start_params[i].start_point1 = tmp;
					}
					break;
				}
			}
		}

		stack<RecursiveStackParams> stack_params;
		if (start_params[0].start_point2 == start_params[1].start_point1) {
			stack_params.push(start_params[2]);
			stack_params.push(start_params[1]);
			stack_params.push(start_params[0]);
		}
		else {
			stack_params.push(start_params[0]);
			stack_params.push(start_params[1]);
			stack_params.push(start_params[2]);
		}

		vector<Point> horizon_points;
		bool first = true;

		while (!stack_params.empty()) {
			RecursiveStackParams current_param = stack_params.top();
			stack_params.pop();

			bool inside = MyMath::trianglePointCrossInside(faces[current_param.face_index], pivotPoint, found_point);
			if (inside) {
				edge_faces.push_back(current_param.face_index);
				if (first) {
					first = false;
					horizon_points.push_back(current_param.start_point1);
					horizon_points.push_back(current_param.start_point2);
				}
				else {
					if (current_param.start_point1 == horizon_points.front() || current_param.start_point1 == horizon_points.back())
						horizon_points.push_back(current_param.start_point2);
					else if (current_param.start_point2 == horizon_points.front() || current_param.start_point2 == horizon_points.back())
						horizon_points.push_back(current_param.start_point1);
					else throw 3;
				}
			}
			else {
				used_faces[current_param.face_index] = true;
				RecursiveStackParams new_params1, new_params2;
				new_params1.from = current_param.face_index;
				new_params2.from = current_param.face_index;
				bool param1_found = false;
				bool param2_found = false;

				for (size_t i = 0; i < 3; i++) {
					size_t neighboring_face_ind = faces[current_param.face_index].neighbouringFaces[i];
					if (used_faces[neighboring_face_ind]) continue;

					int check_existance = 0;
					for (size_t i = 0; i < 3; i++) {
						for (size_t j = 0; j < 3; j++) {
							if (faces[neighboring_face_ind].vertices[i] == faces[current_param.face_index].vertices[j])
								check_existance++;
							if (check_existance > 2) throw 4;
						}
					}
					if (check_existance != 2) throw 5;

					if ((current_param.start_point1 == faces[neighboring_face_ind].vertices[0] ^ current_param.start_point1 == faces[neighboring_face_ind].vertices[1] ^ current_param.start_point1 == faces[neighboring_face_ind].vertices[2]) &&
						!(current_param.start_point2 == faces[neighboring_face_ind].vertices[0] || current_param.start_point2 == faces[neighboring_face_ind].vertices[1] || current_param.start_point2 == faces[neighboring_face_ind].vertices[2]))
					{
						new_params2.face_index = neighboring_face_ind;
						new_params2.start_point1 = current_param.start_point1;
						for (size_t j = 0; j < 3; j++)
							if (!(faces[current_param.face_index].vertices[j] == current_param.start_point1 || faces[current_param.face_index].vertices[j] == current_param.start_point2))
								new_params2.start_point2 = faces[current_param.face_index].vertices[j];
						param2_found = true;
					}
					else if ((current_param.start_point2 == faces[neighboring_face_ind].vertices[0] ^ current_param.start_point2 == faces[neighboring_face_ind].vertices[1] ^ current_param.start_point2 == faces[neighboring_face_ind].vertices[2]) &&
						    !(current_param.start_point1 == faces[neighboring_face_ind].vertices[0] || current_param.start_point1 == faces[neighboring_face_ind].vertices[1] || current_param.start_point1 == faces[neighboring_face_ind].vertices[2]))
					{
						new_params1.face_index = neighboring_face_ind;
						new_params1.start_point2 = current_param.start_point2;
						for (size_t j = 0; j < 3; j++)
							if (!(faces[current_param.face_index].vertices[j] == current_param.start_point1 || faces[current_param.face_index].vertices[j] == current_param.start_point2))
								new_params1.start_point1 = faces[current_param.face_index].vertices[j];
						param1_found = true;
					}
					else throw 6;
				}
				if (param1_found) stack_params.push(new_params1);
				if (param2_found) stack_params.push(new_params2);
			}
		}
		return horizon_points;
	}
	struct RecursiveStackParams {
		size_t face_index;
		Point start_point1;
		Point start_point2;
		size_t from;
	};

	static void makeNewFaces(vector<TriangleFace> &faces, vector<char> &used_faces, Point found_point, vector<size_t> &edge_faces, vector<Point> &horizon) {
		size_t previous_size = faces.size();
		for (size_t i = 0; i < edge_faces.size(); i++)
			used_faces.push_back(false);
		for (size_t i = 0; i < edge_faces.size(); i++) {
			TriangleFace face = TriangleFace(horizon[i], horizon[i + 1], found_point, edge_faces[i], previous_size + ((i+1) % edge_faces.size()), previous_size + (i == 0 ? edge_faces.size()-1 : i-1));
			faces.push_back(face);

			for (size_t j = 0; j < 3; j++)
				if (used_faces[faces[edge_faces[i]].neighbouringFaces[j]]) {
					faces[edge_faces[i]].neighbouringFaces[j] = faces.size() - 1;
					break;
				}
		}

		vector<size_t> less_face_indicies;
		for (size_t i = 0; i < used_faces.size(); i++) {
			if (!used_faces[i]) continue;
			less_face_indicies.push_back(i);
		}
		sort(less_face_indicies.begin(), less_face_indicies.end(), greater<size_t>());

		for (size_t i = 0; i < less_face_indicies.size(); i++) {
			faces.erase(faces.begin() + less_face_indicies[i]);
			for (size_t j = 0; j < faces.size(); j++)
				for (size_t k = 0; k < 3; k++) {
					if (faces[j].neighbouringFaces[k] >= (GLint)less_face_indicies[i])
						faces[j].neighbouringFaces[k]--;
				}
		}
	}
};
Point QuickHull3DAlgorithm::pivotPoint;