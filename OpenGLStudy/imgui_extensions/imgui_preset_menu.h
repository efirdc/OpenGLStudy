#ifndef IMGUI_PRESET_MENU_H
#define IMGUI_PRESET_MENU_H

#include <filesystem>
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
	std::map<std::string, std::string> presets;

	ImguiPresetMenu(T & instance, std::string fileName) :
		instance(instance),
		fileName(fileName)
	{ 
		std::ifstream inFile(fileName);
		if (inFile.good())
		{
			boost::archive::text_iarchive ia(inFile);
			ia >> presets;
		}
		loadPreset("default");
	}

	void saveToDisk()
	{
		std::ofstream outFile(fileName);
		boost::archive::text_oarchive oa(outFile);
		oa << presets;
	}
	
	void loadPreset(const std::string& presetName)
	{
		std::string preset = presets[presetName];
		if (preset == "")
			return;
		std::stringstream sstream;
		sstream.str(preset);
		boost::archive::text_iarchive ia(sstream);
		ia >> instance;
	}

	void savePreset(const std::string& presetName)
	{
		std::stringstream sstream;
		boost::archive::text_oarchive oa(sstream);
		oa << instance;
		presets[presetName] = sstream.str();
		saveToDisk();
	}

	bool Menu()
	{
		float loadedPreset = false;
		if (ImGui::TreeNode("Presets"))
		{
			static std::string selection = "default";
			if (ImGui::BeginCombo("##loadpreset", selection.c_str()))
			{
				for (auto& element : presets)
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
				loadPreset(selection);
				loadedPreset = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				if (selection != "default")
				{
					presets.erase(selection);
					saveToDisk();
					selection = "default";
				}
			}
			static char presetName[64] = "";
			ImGui::InputText("##savepreset", presetName, IM_ARRAYSIZE(presetName));
			ImGui::SameLine();
			if (ImGui::Button("Save Preset"))
				savePreset(presetName);
			ImGui::TreePop();
		}
		return loadedPreset;
	}

};

#endif