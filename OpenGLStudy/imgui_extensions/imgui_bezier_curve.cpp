#include "imgui_bezier_curve.h"

using namespace ImGui;

ImBezierCurve::ImBezierCurve(glm::vec2 controlPoint1, glm::vec2 controlPoint2, int curve1DSize, int curve2DSize) :
	controlPoint1{ controlPoint1 },
	controlPoint2{ controlPoint2 },
	curve1DSize{ curve1DSize },
	curve2DSize{ curve2DSize }
{

	curve1D = new float[curve1DSize]();
	curve2D = new glm::vec2[curve2DSize]();
	recalculate();
}

ImBezierCurve::~ImBezierCurve()
{
	delete[] curve1D;
	delete[] curve2D;
}

void ImBezierCurve::recalculate()
{
	delete[] curve1D;
	delete[] curve2D;
	curve1D = new float[curve1DSize]();
	curve2D = new glm::vec2[curve2DSize]();
	utl::bezierTable(controlPoint1, controlPoint2, curve2D, curve2DSize);
	utl::curve2Dto1D(curve2D, curve2DSize, curve1D, curve1DSize);
}

bool ImBezierCurve::menu(const char* label)
{
	// visual constants
	const ImVec2 SPACING(6.0f, 10.0f);
	const int GRID_SIZE = 4;
	const float CURVE_WIDTH = 4.0f; // main curved line width
	const float LINE_WIDTH = 1.0f; // handlers: small lines width
	const float GRAB_RADIUS = 6.0f; // handlers: circle radius
	const float GRAB_BORDER = 2.0f; // handlers: circle border width

	// Get ImGui state objects
	ImGuiStyle& style = GetStyle();
	ImGuiIO& io = GetIO();
	ImDrawList* drawList = GetWindowDrawList();
	ImGuiWindow* window = GetCurrentWindow();

	if (window->SkipItems)
		return false;

	// sliders
	float points[4] = { controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y };
	int changed = SliderFloat4(label, points, 0, 1, "%.3f", 1.0f);
	controlPoint1.x = points[0]; controlPoint1.y = points[1]; controlPoint2.x = points[2]; controlPoint2.y = points[3];

	// prepare canvas
	float size = ImMin(GetContentRegionAvailWidth(), 256.0f);
	ImVec2 topLeft = window->DC.CursorPos + SPACING;
	ImVec2 topRight = topLeft + ImVec2(size, 0.0f);
	ImVec2 bottomLeft = topLeft + ImVec2(0.0f, size);
	ImVec2 bottomRight = topLeft + ImVec2(size, size);
	ImRect bb(topLeft, bottomRight);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return changed;
	int hovered = IsItemActive() || IsItemHovered();

	// draw background and grid
	RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		float t = (float)i / (float)GRID_SIZE;
		drawList->AddLine(ImLerp(topLeft, topRight, t), ImLerp(bottomLeft, bottomRight, t), GetColorU32(ImGuiCol_TextDisabled));
		drawList->AddLine(ImLerp(topLeft, bottomLeft, t), ImLerp(topRight, bottomRight, t), GetColorU32(ImGuiCol_TextDisabled));
	}

	// move control points
	ImVec2 windowControlPoints[2];
	ImVec2 controlPoints[2] = { controlPoint1, controlPoint2 };
	for (int i = 0; i < 2; i++)
	{
		PushID(i);
		windowControlPoints[i] = ImLerp(bottomLeft, topRight, controlPoints[i]);
		SetCursorScreenPos(windowControlPoints[i] - ImVec2(GRAB_RADIUS, GRAB_RADIUS));
		InvisibleButton(label, ImVec2(2 * GRAB_RADIUS, 2 * GRAB_RADIUS));
		if (IsItemActive() && IsMouseDragging(0))
		{
			controlPoints[i] = (io.MousePos - bottomLeft) / (topRight - bottomLeft);
			controlPoints[i].x = ImClamp(controlPoints[i].x, 0.0f, 1.0f);
			changed = true;
		}
		if (IsItemActive() || IsItemHovered())
			SetTooltip("(%4.3f, %4.3f)", controlPoints[i].x, controlPoints[i].y);
		PopID();
	}
	controlPoint1 = controlPoints[0]; controlPoint2 = controlPoints[1];

	// if the widget is hovered or changed then draw it over the whole screen instead of just the window
	const bool drawFullScreen = hovered || changed;
	if (drawFullScreen)
		drawList->PushClipRectFullScreen();

	// draw the bezier
	ImColor color(style.Colors[ImGuiCol_PlotLines]);
	drawList->AddBezierCurve(bottomLeft, windowControlPoints[0], windowControlPoints[1], topRight, color, CURVE_WIDTH);

	// draw lines and grabbers
	ImVec4 pink(1.00f, 0.00f, 0.75f, 0.7f), cyan(0.00f, 0.75f, 1.00f, 0.7f);
	ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
	drawList->AddLine(bottomLeft, windowControlPoints[0], ImColor(white), LINE_WIDTH);
	drawList->AddLine(topRight, windowControlPoints[1], ImColor(white), LINE_WIDTH);
	drawList->AddCircleFilled(windowControlPoints[0], GRAB_RADIUS, ImColor(white));
	drawList->AddCircleFilled(windowControlPoints[0], GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
	drawList->AddCircleFilled(windowControlPoints[1], GRAB_RADIUS, ImColor(white));
	drawList->AddCircleFilled(windowControlPoints[1], GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

	if (drawFullScreen)
		drawList->PopClipRect();

	// restore cursor pos
	SetCursorScreenPos(bottomLeft + SPACING * ImVec2(-1.0f, 1.0f));

	return changed;
}

