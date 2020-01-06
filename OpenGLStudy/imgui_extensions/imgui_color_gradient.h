//
//  imgui_color_gradient.h
//  imgui extension
//
//  Created by David Gallardo on 11/06/16.

/*
 
 Usage:
 
 ::GRADIENT DATA::
 ImGradient gradient;
 
 ::BUTTON::
 if(ImGui::GradientButton(&gradient))
 {
    //set show editor flag to true/false
 }
 
 ::EDITOR::
 static ImGradientMark* draggingMark = nullptr;
 static ImGradientMark* selectedMark = nullptr;
 
 bool updated = ImGui::GradientEditor(&gradient, draggingMark, selectedMark);
 
 ::GET A COLOR::
 float color[3];
 gradient.getColorAt(0.3f, color); //position from 0 to 1
 
 ::MODIFY GRADIENT WITH CODE::
 gradient.getMarks().clear();
 gradient.addMark(0.0f, ImColor(0.2f, 0.1f, 0.0f));
 gradient.addMark(0.7f, ImColor(120, 200, 255));
 
 ::WOOD BROWNS PRESET::
 gradient.getMarks().clear();
 gradient.addMark(0.0f, ImColor(0xA0, 0x79, 0x3D));
 gradient.addMark(0.2f, ImColor(0xAA, 0x83, 0x47));
 gradient.addMark(0.3f, ImColor(0xB4, 0x8D, 0x51));
 gradient.addMark(0.4f, ImColor(0xBE, 0x97, 0x5B));
 gradient.addMark(0.6f, ImColor(0xC8, 0xA1, 0x65));
 gradient.addMark(0.7f, ImColor(0xD2, 0xAB, 0x6F));
 gradient.addMark(0.8f, ImColor(0xDC, 0xB5, 0x79));
 gradient.addMark(1.0f, ImColor(0xE6, 0xBF, 0x83));
 
 */

#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "hsluv/hsluv.h"
#include "imgui_boost_serialization.h"

#include <list>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/split_member.hpp>

static const int GRADIENT_CACHE_SIZE = 256;

class ImGradient
{
public:

	struct Mark
	{
		friend class boost::serialization::access;
		float position;
		ImVec4 color;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & position & color;
		}
	};

	enum class ColorSpace
	{
		rgb,
		hsv,
		hsluv,
		hpluv
	};

    ImGradient();
	ImGradient(const ImGradient & copy);
    ~ImGradient();
    ImVec4 getColorAt(float position) const;
    void addMark(float position, ImColor const color);
    void removeMark(Mark * mark);
	void setColorSpace(int colorSpace);
    void refreshCache();
    std::list<Mark *> & getMarks() { return m_marks; }
	int getColorSpace() { return m_colorSpace; }
	
	Mark * draggingMark;
	Mark * selectedMark;

private:
    void computeColorAt(float position, ImVec4 & color) const;
    
    ImVec4 m_cachedValues[GRADIENT_CACHE_SIZE];
	int m_colorSpace;
	std::list<Mark *> m_marks;

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & m_colorSpace;
		ar & m_marks;
	}
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar& m_colorSpace;
        ar& m_marks;
        refreshCache();
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

namespace ImGui
{
	void DrawGradientBar(ImGradient * gradient, ImVec2 barPos, float width, float height);
	bool GradientButton(ImGradient * gradient);
    bool GradientEditor(ImGradient * gradient);
}
