#pragma once
#include "arg_parser.h"
#include "glCanvas.h"
#include "matrix.h"
#include "spline_parser.h"
#include "triangle_mesh.h"
#include "vectors.h"
#include "spline.h"
#include <vector>

class Curve : public Spline {
public:
	Curve(int num) {
		num_vertices = num;
		vertice.resize(num_vertices);
	}

	void set(int i, Vec3f v) override {
		vertice[i] = v;
	}

	int getNumVertices() override {
		return num_vertices;
	}

	Vec3f getVertex(int i) override {
		return vertice[i];
	}

	void Paint(ArgParser* args) override;

protected:
	int num_vertices;
	vector<Vec3f> vertice;

	void drawLines();
	void drawPoints();
	virtual void drawCurves(ArgParser* args) = 0;
};

class BezierCurve : public Curve {
public:
	BezierCurve(int num) : Curve(num) {}

	void OutputBezier(FILE* file) override;
	void moveControlPoint(int selectedPoint, float x, float y) override;
	void addControlPoint(int selectedPoint, float x, float y) override;
	void deleteControlPoint(int selectedPoint) override;

	TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
	void drawCurves(ArgParser* args) override;

};

class BSplineCurve : public Curve {
public:
	BSplineCurve(int num) : Curve(num) {}

	void OutputBSpline(FILE* file) override;
	void moveControlPoint(int selectedPoint, float x, float y) override;
	void addControlPoint(int selectedPoint, float x, float y) override;
	void deleteControlPoint(int selectedPoint) override;

	TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
	void drawCurves(ArgParser* args) override;
};