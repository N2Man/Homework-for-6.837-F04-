#pragma once
#include "arg_parser.h"
#include "glCanvas.h"
#include "matrix.h"
#include "spline_parser.h"
#include "triangle_mesh.h"
#include "vectors.h"

class Spline {
public:
	// FOR VISUALIZATION
	virtual void Paint(ArgParser* args);

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file);
	virtual void OutputBSpline(FILE* file);

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices();
	virtual Vec3f getVertex(int i);

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y);
	virtual void addControlPoint(int selectedPoint, float x, float y);
	virtual void deleteControlPoint(int selectedPoint);

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args);

protected:
	Matrix Bezier_M = new float[16]{ -1,3,-3,1,
									  3,-6,3,0,
									  -3,3,0,0,
									   1,0,0,0 };

	Matrix BSpline_M = new float[16]{ -1,3,-3,1,
									  3,-6,0,4,
									  -3,3,3,1,
									   1,0,0,0 };
};