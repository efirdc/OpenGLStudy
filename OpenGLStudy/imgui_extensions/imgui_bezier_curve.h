#pragma once

#include "imgui/imgui.h"
#include "utilities.h"

class ImBezierCurve
{
public:
	ImVec2 controlPoints[2];
	float * curve1D;
	glm::vec2 * curve2D;
	int curve1DSize;
	int curve2DSize;

	ImBezierCurve(ImVec2 * initialControlPoints, int curve1DSize, int curve2DSize);
	void recalculate();
};