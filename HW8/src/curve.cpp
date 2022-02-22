#include "curve.h"
#include "windows.h"
#include "GL/glut.h"

double M_PI;

void Curve::Paint(ArgParser* args)
{
	drawLines();
	drawCurves(args);
	drawPoints();
}

void Curve::drawLines()
{
	glLineWidth(3);
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (Vec3f v : vertice) {
		glVertex3f(v.x(), v.y(), v.z());
	}
	glEnd();
}

void Curve::drawPoints()
{
	glPointSize(5);
	glColor3f(1, 0, 0);
	glBegin(GL_POINT);
	for (Vec3f v : vertice) {
		glVertex3f(v.x(), v.y(), v.z());
	}
	glEnd();

}

void BezierCurve::OutputBezier(FILE* file)
{
	fprintf(file, "\n");
	fprintf(file, "BezierNum: %d\n", num_vertices);
	for (Vec3f v : vertice) {
		fprintf(file, "%f,%f,%f\n",v.x(), v.y(), v.z());
	}

}

void BezierCurve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertice[selectedPoint].Set(x, y, 0);
}

void BezierCurve::addControlPoint(int selectedPoint, float x, float y)
{
}

void BezierCurve::deleteControlPoint(int selectedPoint)
{
}

TriangleMesh* BezierCurve::OutputTriangles(ArgParser* args)
{
	int curve_tessellation = args->curve_tessellation;
	int revolution_tessellation = args->revolution_tessellation;
	float curve_d = 1.0f / curve_tessellation;
	float revolution_d = 2.0 * M_PI / revolution_tessellation;
	TriangleNet* triangleNet = new TriangleNet(revolution_tessellation, num_vertices / 3 * curve_tessellation);

	for (int i = 0; i < num_vertices - 3; i += 3) {
		Matrix P;
		for (int j = 0; j < 4; ++j) {
			P.Set(j, 0, vertice[i + j].x());
			P.Set(j, 1, vertice[i + j].y());
			P.Set(j, 2, vertice[i + j].z());
		}
		for (int j = 0; j <= curve_tessellation; ++j) {
			float t = curve_d * j;
			float t2 = t * t;
			float t3 = t2 * t;
			Vec4f T(t3, t2, t, 1);
			Bezier_M.Transform(T);
			P.Transform(T);
			for (int k = 0; k <= revolution_tessellation; ++k) {
				float theta = revolution_d * k;
				float x = cos(theta) * T.x() + sin(theta) * T.z();
				float y = T.y();
				float z = -sin(theta) * T.x() + cos(theta) * T.z();
				triangleNet->SetVertex(k, j + (i / 3 * curve_tessellation), Vec3f(x, y, z));
			}
		}
	}
	return triangleNet;
}

void BezierCurve::drawCurves(ArgParser* args)
{
	glLineWidth(3);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int tes = args->curve_tessellation;
	float d = 1.0 / tes;
	for (int i = 0; i < num_vertices - 3; i += 3) {
		Matrix P;
		for (int j = 0; j < 4; j++) {
			P.Set(j, 0, vertice[i + j].x());
			P.Set(j, 1, vertice[i + j].y());
			P.Set(j, 2, vertice[i + j].z());
		}
		for (int j = 0; j < tes; j++) {
			float t = d * j;
			float t2 = t * t;
			float t3 = t2 * t;
			Vec4f T(t3, t2, t, 1);
			Bezier_M.Transform(T);
			P.Transform(T);
			glVertex3f(T.x(), T.y(), T.z());
		}
	}
	glEnd();
}

void BSplineCurve::OutputBSpline(FILE* file)
{
}

void BSplineCurve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertice[selectedPoint].Set(x, y, 0);
}

void BSplineCurve::addControlPoint(int selectedPoint, float x, float y)
{
	num_vertices++;
	auto it = vertice.begin();
	while (selectedPoint--)
		it++;
	vertice.insert(it, Vec3f(x, y, 0));
}

void BSplineCurve::deleteControlPoint(int selectedPoint)
{
	if (num_vertices <= 4)
		return;
	auto it = vertice.begin();
	while (selectedPoint--)
		it++;
	vertice.erase(it);
	num_vertices--;
}

TriangleMesh* BSplineCurve::OutputTriangles(ArgParser* args)
{
	int curve_tessellation = args->curve_tessellation;
	int revolution_tessellation = args->revolution_tessellation;
	float curve_d = 1.0f / curve_tessellation;
	float revolution_d = 2.0 * M_PI / revolution_tessellation;
	TriangleNet* triangleNet = new TriangleNet(revolution_tessellation, num_vertices / 3 * curve_tessellation);

	for (int i = 0; i < num_vertices - 3; i += 3) {
		Matrix P;
		for (int j = 0; j < 4; ++j) {
			P.Set(j, 0, vertice[i + j].x());
			P.Set(j, 1, vertice[i + j].y());
			P.Set(j, 2, vertice[i + j].z());
		}
		for (int j = 0; j <= curve_tessellation; ++j) {
			float t = curve_d * j;
			float t2 = t * t;
			float t3 = t2 * t;
			Vec4f T(t3, t2, t, 1);
			Bezier_M.Transform(T);
			P.Transform(T);
			for (int k = 0; k <= revolution_tessellation; ++k) {
				float theta = revolution_d * k;
				float x = cos(theta) * T.x() + sin(theta) * T.z();
				float y = T.y();
				float z = -sin(theta) * T.x() + cos(theta) * T.z();
				triangleNet->SetVertex(k, j + (i / 3 * curve_tessellation), Vec3f(x, y, z));
			}
		}
	}
	return triangleNet;
}

void BSplineCurve::drawCurves(ArgParser* args)
{
	glLineWidth(3);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int tes = args->curve_tessellation;
	float d = 1.0 / tes;
	for (int i = 0; i < num_vertices - 3; i += 3) {
		Matrix P;
		for (int j = 0; j < 4; j++) {
			P.Set(j, 0, vertice[i + j].x());
			P.Set(j, 1, vertice[i + j].y());
			P.Set(j, 2, vertice[i + j].z());
		}
		for (int j = 0; j < tes; j++) {
			float t = d * j;
			float t2 = t * t;
			float t3 = t2 * t;
			Vec4f T(t3, t2, t, 1);
			Bezier_M.Transform(T);
			P.Transform(T);
			glVertex3f(T.x(), T.y(), T.z());
		}
	}
	glEnd();
}
