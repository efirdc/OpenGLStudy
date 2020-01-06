#ifndef IMGUI_PRESET_MENU_H
#define IMGUI_PRESET_MENU_H

#include <map>

#include "imgui/imgui.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

template<class T>
class ImguiPresetMenu {
public:
	T & instance;
	std::string fileName;

	ImguiPresetMenu(T & instance, std::string fileName) :
		instance(instance),
		fileName(fileName)
	{ 
		std::ifstream inFile(fileName);
		if (inFile.good())
		{
			boost::archive::text_iarchive ia(inFile);
			ia >> Presets;
		}
		inFile.close();
		instance = *Presets["default"];
	}

	bool Menu()
	{
		float loadedPreset = false;
		if (ImGui::TreeNode("Presets"))
		{
			static std::string selection = "default";
			if (ImGui::BeginCombo("##loadpreset", selection.c_str()))
			{
				for (auto& element : Presets)
				{
					std::string key = element.first;
					bool isSelected = selection == key;
					if (ImGui::Selectable(key.c_str(), isSelected))
						selection = key;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				instance = *Presets[selection];
				loadedPreset = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				if (selection != "default")
				{
					Presets.erase(selection);
					std::ofstream outFile(fileName);
					boost::archive::text_oarchive oa(outFile);
					oa << Presets;
					selection = "default";
				}
			}
			static char presetName[64] = "";
			ImGui::InputText("##savepreset", presetName, IM_ARRAYSIZE(presetName));
			ImGui::SameLine();
			if (ImGui::Button("Save Preset"))
			{
				Presets[presetName] = new T(instance);
				std::ofstream outFile(fileName);
				boost::archive::text_oarchive oa(outFile);
				oa << Presets;
			}
			ImGui::TreePop();
		}
		return loadedPreset;
	}
private:
	static std::map<std::string, T *> Presets;
};
template<typename T> std::map<std::string, T *> ImguiPresetMenu<T>::Presets;

#endif