/*
The MIT License (MIT)

Copyright (c) 2016 David Gallardo (Original class from: https://github.com/Galloman/GTF)
Copyright (c) 2018 Cory Efird (Refactor)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "imgui_color_gradient.h"

static const float GRADIENT_BAR_WIDGET_HEIGHT = 25;
static const float GRADIENT_BAR_EDITOR_HEIGHT = 40;
static const int GRADIENT_BAR_LINEAR_COLOR_SEGMENTS = 200;
static const float GRADIENT_MARK_DELETE_DIFFY = 40;

ImGradient::ImGradient()
{
	m_colorSpace = (int)ColorSpace::rgb;
    addMark(0.0f, ImColor(0.0f,0.0f,0.0f));
    addMark(1.0f, ImColor(1.0f,1.0f,1.0f));
}

ImGradient::~ImGradient()
{
	for (Mark * mark : m_marks)
		delete mark;
}

void ImGradient::addMark(float position, ImColor color)
{
	// Create the new mark, and set it as the selected mark
	Mark* newMark = new Mark();
    newMark->position = ImClamp(position, 0.0f, 1.0f);
	newMark->color = color;
	selectedMark = newMark;

	// Add mark to mark list and refresh cache
    m_marks.push_back(newMark);
    refreshCache();
}

void ImGradient::removeMark(Mark* mark)
{
    m_marks.remove(mark);
    refreshCache();
}

void ImGradient::setColorSpace(int colorSpace)
{
	m_colorSpace = colorSpace;
	refreshCache();
}

ImVec4 ImGradient::getColorAt(float position) const
{
	int cachePos = (int)(ImClamp(position, 0.0f, 1.0f) * (GRADIENT_CACHE_SIZE - 1));
	return m_cachedValues[cachePos];
}

void ImGradient::computeColorAt(float position, ImVec4 & color) const
{
	// Set color to black if there are no marks.
	if (m_marks.empty())
	{
		color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}
		
	position = ImClamp(position, 0.0f, 1.0f);

	// Find the closest marks on the left and ride side of the position
    Mark * leftMark = nullptr;
	Mark * rightMark = nullptr;
	for (Mark * mark : m_marks)
	{
		if (mark->position <= position && (!leftMark || leftMark->position < mark->position))
			leftMark = mark;
		if (mark->position >= position && (!rightMark || rightMark->position > mark->position))
			rightMark = mark;
	}
    
	// Make the left and right mark the same if there is no mark on one side of position
    if(leftMark && !rightMark)
		rightMark = leftMark;
    else if(!leftMark && rightMark)
		leftMark = rightMark;

	// Convert the color from rgb to the gradients color space
	ImVec4 colorLeft = leftMark->color;
	ImVec4 colorRight = rightMark->color;
	if (m_colorSpace == (int)ColorSpace::hsv)
	{
		ImGui::ColorConvertRGBtoHSV(colorLeft.x, colorLeft.y, colorLeft.z, colorLeft.x, colorLeft.y, colorLeft.z);
		ImGui::ColorConvertRGBtoHSV(colorRight.x, colorRight.y, colorRight.z, colorRight.x, colorRight.y, colorRight.z);
	}
	else if (m_colorSpace == (int)ColorSpace::hpluv)
	{
		double temp[3];
		rgb2hpluv(colorLeft.x, colorLeft.y, colorLeft.z, &temp[0], &temp[1], &temp[2]);
		colorLeft.x = (float)temp[0];
		colorLeft.y = (float)temp[1];
		colorLeft.z = (float)temp[2];
		rgb2hpluv(colorRight.x, colorRight.y, colorRight.z, &temp[0], &temp[1], &temp[2]);
		colorRight.x = (float)temp[0];
		colorRight.y = (float)temp[1];
		colorRight.z = (float)temp[2];
	}
	else if (m_colorSpace == (int)ColorSpace::hsluv)
	{
		double temp[3];
		rgb2hsluv(colorLeft.x, colorLeft.y, colorLeft.z, &temp[0], &temp[1], &temp[2]);
		colorLeft.x = (float)temp[0];
		colorLeft.y = (float)temp[1];
		colorLeft.z = (float)temp[2];
		rgb2hsluv(colorRight.x, colorRight.y, colorRight.z, &temp[0], &temp[1], &temp[2]);
		colorRight.x = (float)temp[0];
		colorRight.y = (float)temp[1];
		colorRight.z = (float)temp[2];
	}

	// Linear interpolate from the left to right color
	if (leftMark != rightMark)
	{
		float t = (position - leftMark->position) / (rightMark->position - leftMark->position);
		color = ImLerp(colorLeft, colorRight, t);
	}
	else
		color = colorLeft;

	// Convert back from the gradient color space to rgb color space
	if (m_colorSpace == (int)ColorSpace::hsv)
	{
		ImGui::ColorConvertHSVtoRGB(color.x, color.y, color.z, color.x, color.y, color.z);
	}
	else if (m_colorSpace == (int)ColorSpace::hpluv)
	{
		double temp[3];
		hpluv2rgb(color.x, color.y, color.z, &temp[0], &temp[1], &temp[2]);
		color.x = (float)temp[0];
		color.y = (float)temp[1];
		color.z = (float)temp[2];
	}
	else if (m_colorSpace == (int)ColorSpace::hsluv)
	{
		double temp[3];
		hsluv2rgb(color.x, color.y, color.z, &temp[0], &temp[1], &temp[2]);
		color.x = (float)temp[0];
		color.y = (float)temp[1];
		color.z = (float)temp[2];
	}

	color = ImVec4(ImClamp(color.x, 0.0f, 1.0f), ImClamp(color.y, 0.0f, 1.0f), ImClamp(color.z, 0.0f, 1.0f), ImClamp(color.w, 0.0f, 1.0f));
}

void ImGradient::refreshCache()
{
    m_marks.sort([](const Mark * a, const Mark * b) { return a->position < b->position; });
    
    for(int i = 0; i < GRADIENT_CACHE_SIZE; ++i)
        computeColorAt((float)i / (float)(GRADIENT_CACHE_SIZE - 1), m_cachedValues[i]);
}

namespace ImGui
{
	void DrawGradientBar(ImGradient * gradient, ImVec2 barPos, float width, float height)
    {
        // Get bar positions
		ImVec2 topLeftPos = barPos;
		ImVec2 bottomLeftPos = barPos + ImVec2(0.0f, height);
		ImVec2 bottomRightPos = barPos + ImVec2(width, height);

		// Draw a border around the gradient bar
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 borderSize = ImVec2(2.0f, 2.0f);
        draw_list->AddRectFilled(topLeftPos - borderSize, bottomRightPos + borderSize, IM_COL32(100, 100, 100, 255));
        
		// Fill gradient bar with horizontal gradients between each mark
		ImVec4 prevColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float prevX = 0.0f;
		for (int i = 0; i < GRADIENT_BAR_LINEAR_COLOR_SEGMENTS; i++)
		{
			float t = (float)i / (float)(GRADIENT_BAR_LINEAR_COLOR_SEGMENTS - 1);
			float nextX = ImLerp(0.0f, width, t);
			ImVec4 nextColor = gradient->getColorAt(t);
			if (t > 0.0f)
			{
				ImU32 prevColorU32 = ImGui::ColorConvertFloat4ToU32(prevColor);
				ImU32 nextColorU32 = ImGui::ColorConvertFloat4ToU32(nextColor);
				draw_list->AddRectFilledMultiColor(
					topLeftPos + ImVec2(prevX, 0.0f), 
					bottomLeftPos + ImVec2(nextX, 0.0f), 
					prevColorU32, nextColorU32, nextColorU32, prevColorU32);
			}
			prevX = nextX;
			prevColor = nextColor;
		}

		// Set cursor below the gradient bar
        ImGui::SetCursorScreenPos(bottomLeftPos + ImVec2(0.0f, 10.0f));
    }
    
    void DrawGradientMarks(ImGradient * gradient, ImVec2 barPos, float width, float height)
    {
		// Get bar positions
		ImVec2 bottomLeftPos = barPos + ImVec2(0.0f, height);
		ImVec2 bottomRightPos = barPos + ImVec2(width, height);

		// Draw all marks
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        for(ImGradient::Mark * mark : gradient->getMarks())
        {
			// Get screenspace mark position, and mark color
			ImVec2 markPos = ImLerp(bottomLeftPos, bottomRightPos, mark->position);
            ImU32 colorU32 = ImGui::ColorConvertFloat4ToU32(mark->color);
            
			// Draw gray triangle, gray rect, and inner black rect
			draw_list->AddTriangleFilled(
				markPos + ImVec2( 0.0f,-6.0f), 
				markPos + ImVec2(-6.0f, 0.0f), 
				markPos + ImVec2( 6.0f, 0.0f), 
				IM_COL32(100, 100, 100, 255));
			draw_list->AddRectFilled(
				markPos + ImVec2(-6.0f, 0.0f), 
				markPos + ImVec2( 6.0f, 12.0f), 
				IM_COL32(100, 100, 100, 255), 1.0f, 1);
            draw_list->AddRectFilled(
				markPos + ImVec2(-5.0f, 1.0f),
				markPos + ImVec2( 5.0f, 11.0f),
				IM_COL32(0, 0, 0, 255), 1.0f, 1);
            
			// Draw inner green triangle and rect if mark is selected
            if(gradient->selectedMark == mark)
            {
                draw_list->AddTriangleFilled(
					markPos + ImVec2( 0.0f,-3.0f),
					markPos + ImVec2(-4.0f, 1.0f),
					markPos + ImVec2( 4.0f, 1.0f),
                    IM_COL32(0, 255, 0, 255));
                draw_list->AddRect(
					markPos + ImVec2(-5.0f, 1.0f),
					markPos + ImVec2( 5.0f, 11.0f),
					IM_COL32(0, 255, 0, 255), 1.0f, 1);
            }
            
			// Draw last inner rect with the marks color
			draw_list->AddRectFilled(
				markPos + ImVec2(-3.0f,  3.0f),
				markPos + ImVec2( 3.0f,  9.0f),
				colorU32, 1.0f, 1);
        }
        
		// Set cursor below the gradient bar
        ImGui::SetCursorScreenPos(bottomLeftPos + ImVec2(0.0f, 20.0f));
    }
    
    bool GradientButton(ImGradient* gradient)
    {
		// Get bar position, width, and height
        ImVec2 barPos = ImGui::GetCursorScreenPos();
        float width = ImMax(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
		float height = GRADIENT_BAR_WIDGET_HEIGHT;

		// Draw the bar
		DrawGradientBar(gradient, barPos, width, height);

		// Button behavior
        return ImGui::InvisibleButton("gradient_bar", ImVec2(width, height));
    }
    
    bool GradientEditor(ImGradient * gradient)
    {
        bool modified = false;
        
		// Get bar position, width, and height
        ImVec2 barPos = ImGui::GetCursorScreenPos() + ImVec2(10.0f, 0.0f);
        float width = ImGui::GetContentRegionAvailWidth() - 20;
        float height = GRADIENT_BAR_EDITOR_HEIGHT;

		// Other bar positions
		ImVec2 bottomLeftPos = barPos + ImVec2(0.0f, height);
		ImVec2 bottomRightPos = barPos + ImVec2(width, height);
        
		// Add a new mark on click
        ImGui::InvisibleButton("gradient_editor_bar", ImVec2(width, height));
        if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            float newMarkPos = (ImGui::GetIO().MousePos.x - barPos.x) / width;
            gradient->addMark(newMarkPos, gradient->getColorAt(newMarkPos));
			modified = true;
        }

		// When a mark is left clicked, set it as the selected and dragging mark.
		// On right click, delete the mark.
		ImGradient::Mark * deleteMark = nullptr;
		for (ImGradient::Mark * mark : gradient->getMarks())
		{
			ImVec2 markPos = ImLerp(bottomLeftPos, bottomRightPos, mark->position);
			ImGui::SetCursorScreenPos(markPos - ImVec2(6.0f, 0.0f));
			ImGui::InvisibleButton("mark", ImVec2(12.0f, 12.0f));
			if(ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseClicked(0))
				{
					gradient->selectedMark = mark;
					gradient->draggingMark = mark;
				}
				else if (ImGui::IsMouseClicked(1))
					deleteMark = mark;
			}
		}
		if (deleteMark)
		{
			gradient->removeMark(deleteMark);
			if (deleteMark == gradient->draggingMark)
				gradient->draggingMark = nullptr;
			if (deleteMark == gradient->selectedMark)
				gradient->selectedMark = nullptr;
			modified = true;
		}
			

		// When the mouse is released, clear the dragging mark
		if (!ImGui::IsMouseDown(0) && gradient->draggingMark)
			gradient->draggingMark = nullptr;
       
		// Mark dragging behavior
        if(ImGui::IsMouseDragging(0) && gradient->draggingMark)
        {
			// Move the mark to its new horizontal position when dragged
			ImVec2 mousePos = ImGui::GetIO().MousePos;
			float newMarkPos = ImClamp((mousePos.x - barPos.x) / width, 0.0, 1.0);
            if(gradient->draggingMark->position != newMarkPos)
            {
				gradient->draggingMark->position = newMarkPos;
                gradient->refreshCache();
                modified = true;
            }
            
			// Remove the mark if it is dragged too far downwards
            float diffY = mousePos.y - (barPos.y + height);
            if(diffY >= GRADIENT_MARK_DELETE_DIFFY)
            {
                gradient->removeMark(gradient->draggingMark);
                gradient->draggingMark = nullptr;
				gradient->selectedMark = nullptr;
                modified = true;
            }
        }
        
		// Select the first mark if no mark is currently selected
        if(!gradient->selectedMark && !gradient->getMarks().empty())
			gradient->selectedMark = gradient->getMarks().front();

		// Draw gradient bar and marks
		DrawGradientBar(gradient, barPos, width, height);
		DrawGradientMarks(gradient, barPos, width, height);
        
		// Show color picker for currently selected mark
        if(gradient->selectedMark)
        {
            bool colorModified = ImGui::ColorPicker3("", (float *)&(gradient->selectedMark->color));
            if(gradient->selectedMark && colorModified)
            {
                modified = true;
                gradient->refreshCache();
            }
        }

		// Set gradient color space with radio buttons
		int newColorSpace = gradient->getColorSpace();
		ImGui::RadioButton("rgb", &newColorSpace, (int)ImGradient::ColorSpace::rgb); ImGui::SameLine();
		ImGui::RadioButton("hsv", &newColorSpace, (int)ImGradient::ColorSpace::hsv); ImGui::SameLine();
		ImGui::RadioButton("hsluv", &newColorSpace, (int)ImGradient::ColorSpace::hsluv); ImGui::SameLine();
		ImGui::RadioButton("hpluv", &newColorSpace, (int)ImGradient::ColorSpace::hpluv);
		if (gradient->getColorSpace() != newColorSpace)
		{
			gradient->setColorSpace(newColorSpace);
			modified = true;
		}
        
        return modified;
    }
};
