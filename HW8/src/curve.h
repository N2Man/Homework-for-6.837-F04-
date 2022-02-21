#pragma once
#include "arg_parser.h"
#include "glCanvas.h"
#include "matrix.h"
#include "spline_parser.h"
#include "triangle_mesh.h"
#include "vectors.h"
#include "spline.h"

class Curve : public Spline {

};

class BezierCurve : public Curve {

};

class BSplineCurve : public Curve {

};