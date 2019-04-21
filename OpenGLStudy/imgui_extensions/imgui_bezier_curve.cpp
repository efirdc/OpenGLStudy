#include "imgui_bezier_curve.h"

ImBezierCurve::ImBezierCurve(ImVec2 * initialControlPoints, int curve1DSize, int curve2DSize) :
	curve1DSize(curve1DSize),
	curve2DSize(curve2DSize)
{
	controlPoints[0] = initialControlPoints[0];
	controlPoints[1] = initialControlPoints[1];
	curve1D = new float[curve1DSize]();
	curve2D = new glm::vec2[curve2DSize]();
	utl::bezierTable((glm::vec2 *)controlPoints, curve2D, curve2DSize);
	utl::curve2Dto1D(curve2D, curve2DSize, curve1D, curve1DSize);
}

void ImBezierCurve::recalculate()
{
	delete[] curve1D;
	delete[] curve2D;
	curve1D = new float[curve1DSize]();
	curve2D = new glm::vec2[curve2DSize]();
	utl::bezierTable((glm::vec2 *)controlPoints, curve2D, curve2DSize);
	utl::curve2Dto1D(curve2D, curve2DSize, curve1D, curve1DSize);
}
