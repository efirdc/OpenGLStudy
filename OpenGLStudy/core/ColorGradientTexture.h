#pragma once

#include "imgui/imgui.h"
#include "imgui_color_gradient.h"
#include "StreamTexture.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ColorGradientTexture
{
public:
	StreamTexture1D streamTexture;
	int textureUnit;

	ColorGradientTexture(int textureUnit, ImGradient & gradient) :
		textureUnit(textureUnit),
		streamTexture(GL_RGB32F, 256, GL_RGB, GL_FLOAT, 3, 4, false)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_1D, streamTexture.textureID);
		upload(gradient);
	}

	void Menu(const char * title, ImGradient & gradient)
	{
		bool changed = false;
		if (ImGui::TreeNode(title))
		{
			if (ImGui::GradientEditor(&gradient))
				upload(gradient);
			ImGui::TreePop();
		}
	}

	void upload(ImGradient& gradient)
	{
		glm::vec3* pixelBuffer = (glm::vec3*)streamTexture.getPixelBuffer();
		for (int i = 0; i < streamTexture.width; i++)
		{
			ImVec4 color = gradient.getColorAt((float)i / (float)(streamTexture.width - 1));
			pixelBuffer[i] = glm::vec3(color.x, color.y, color.z);
		}
		streamTexture.unmapPixelBuffer();
		streamTexture.getPixelBuffer();
		streamTexture.unmapPixelBuffer();
	}
};