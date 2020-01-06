#pragma once

#include <algorithm> 

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace ImGui
{
	bool Bezier(const char *label, ImVec2 controlPoints[2]);

	template <class T>
	static bool expArrowButtons(const char * fmt, T * v, T min, T max)
	{
		ImGui::PushID(v);
		T old = *v;
		if (ImGui::ArrowButton("##l", ImGuiDir_Left))
			*v = *v / 2;
		ImGui::SameLine();
		if (ImGui::ArrowButton("##r", ImGuiDir_Right))
			*v = *v * 2;
		*v = std::min(std::max(*v, min), max);
		ImGui::SameLine();
		ImGui::Text(fmt, *v);
		ImGui::PopID();
		return *v != old;
	}
	void ShowHelpMarker(const char* desc);

	enum class CurveEditorFlags
	{
		NO_TANGENTS = 1 << 0,
		SHOW_GRID = 1 << 1,
		RESET = 1 << 2
	};

	int CurveEditor(const char* label
		, float* values
		, int points_count
		, const ImVec2& size = ImVec2(-1, -1)
		, ImU32 flags = 0
		, int* new_count = nullptr
		, int* selected_point = nullptr);
};