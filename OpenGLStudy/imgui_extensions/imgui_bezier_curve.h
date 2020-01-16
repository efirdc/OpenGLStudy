#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "utilities.h"

#include <boost/serialization/version.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ImBezierCurve
{
public:
	glm::vec2 controlPoint1;
	glm::vec2 controlPoint2;
	float* curve1D;
	glm::vec2* curve2D;
	int curve1DSize;
	int curve2DSize;

	ImBezierCurve(glm::vec2 controlPoint1, glm::vec2 controlPoint2, int curve1DSize, int curve2DSize);
	~ImBezierCurve();
	void recalculate();
	bool menu(const char* label);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & controlPoint1 & controlPoint2 & curve1DSize & curve2DSize;
		recalculate();
	}
};
