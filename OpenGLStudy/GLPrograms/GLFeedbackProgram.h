#pragma once

#include <iostream>
#include <math.h>
#include <map>
#include <array>
#include "imgui_bezier_curve.h"
#include "SpectrumFilter.h"
#include "SpectrumAnalyzer.h"
#include "imgui_extras.h"
#include "Texture.h"

using namespace std::string_literals;

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glDebug.h"

#include "loopback.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui_color_gradient.h"
#include "imgui_preset_menu.h"
#include "imgui_boost_serialization.h"
#include "imGuIZMOquat.h"
#include "glmSerialization.h"

#include "Shader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "View.h"
#include "ColorGradientTexture.h"
#include "GLProgram.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "utilities.h"

#include "xtensor/xarray.hpp"
#include "xtensor/xnpy.hpp"
#include <random>

#include <boost/serialization/version.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

class AudioTexture
{
public:
	AudioTexture(unsigned int textureUnit, int frameSize, int frameGap, int frequencyBins, int spectrumsInAverage, float domainShift, float peakRadius) :
		textureUnit(textureUnit),
	
		frameSize(frameSize),
		frameGap(frameGap),
		numAudioSamples(frameSize * 2),
		frequencyBins(frequencyBins),
		spectrumsInAverage(spectrumsInAverage),
		domainShift(domainShift),
		peakRadius(peakRadius),

		frequencyTexture{ GL_R32F, frequencyBins, GL_RED, GL_FLOAT, 1, 4, true },
		audioBuffer(numAudioSamples),
		peakCurveSize((int)((float)frequencyBins* peakRadius)),
		frequencyAmplitudeCurve({ 0.016f, 0.016f }, { 0.0f, 1.0f }, bezierCurveSize, bezierCurveSize),
		peakShapingCurve{ { 1.00f, 0.00f }, { 0.0f, 1.00f }, peakCurveSize, bezierCurveSize },

		analyzer{ frameSize },
		amplitudeFilter{ frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize },
		domainShiftFilter{ domainShift, frequencyBins },
		peakFilter{ peakShapingCurve.curve1D, peakShapingCurve.curve1DSize },
		averageFilter{ spectrumsInAverage }
	{
		/*
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_1D, ID);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		std::vector<float> data(frequencyBins, 0.5f);*/
		
		//glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, frequencyBins, 0, GL_RED, GL_FLOAT, NULL);
		//glGenerateMipmap(GL_TEXTURE_1D);
		
		loopback_init();
		bind();
		frequencyTexture.getPixelBuffer();
		frequencyTexture.unmapPixelBuffer();
		
		/*
		float* frequencyPixelBuffer = (float*)frequencyTexture.getPixelBuffer();
		for (int i = 0; i < frequencyTexture.width; i++)
			frequencyPixelBuffer[i] = 0.0f;*/
		
	}

	
	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_1D, ID);
	}
	
	unsigned int textureUnit;
	unsigned int ID;
	
	const int bezierCurveSize = 4096;
	int frameSize;
	int frameGap;
	int numAudioSamples;
	int frequencyBins;
	int spectrumsInAverage;
	float domainShift;

	StreamTexture1D frequencyTexture;
	std::vector<float> audioBuffer;

	float peakRadius;
	int peakCurveSize;
	ImBezierCurve frequencyAmplitudeCurve;
	ImBezierCurve peakShapingCurve;

	SpectrumAnalyzer analyzer;
	AmplitudeFilter amplitudeFilter;
	DomainShiftFilter domainShiftFilter;
	PeakFilter peakFilter;
	AverageFilter averageFilter;

	void update()
	{
		static int newSamples = 0;
		newSamples += loopback_getSound(audioBuffer.data(), numAudioSamples);
		static const FrequencySpectrum* frequencySpectrum = averageFilter.getFrequencySpectrum();
		while (newSamples >= frameGap)
		{
			newSamples -= frameGap;
			float* inputBuffer = analyzer.getFrameInputBuffer();
			int frameSize = analyzer.getFrameSize();
			int frameStart = frameSize - newSamples;
			frameStart = utl::max(frameStart, 0);
			for (int i = 0; i < frameSize; i++)
				inputBuffer[i] = audioBuffer[frameStart + i];
			analyzer.processFrame();
			frequencySpectrum = analyzer.getFrequencySpectrum();
			frequencySpectrum = amplitudeFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = domainShiftFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = peakFilter.applyFilter(frequencySpectrum);
			frequencySpectrum = averageFilter.applyFilter(frequencySpectrum);
		}
		float* frequencyData = frequencySpectrum->data;
		/*
		glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, frequencySpectrum->size, 0, GL_RED, GL_FLOAT, frequencyData);
		glGenerateMipmap(GL_TEXTURE_1D);*/
		
		
		float* frequencyPixelBuffer = (float*)frequencyTexture.getPixelBuffer();
		for (int i = 0; i < frequencySpectrum->size; i++)
			frequencyPixelBuffer[i] = frequencyData[i];
		frequencyTexture.unmapPixelBuffer();
	}

	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar& frameSize& frameGap& numAudioSamples& frequencyBins& spectrumsInAverage& domainShift
			& peakRadius& peakCurveSize& frequencyAmplitudeCurve& peakShapingCurve;
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar& frameSize& frameGap& numAudioSamples& frequencyBins& spectrumsInAverage& domainShift
			& peakRadius& peakCurveSize& frequencyAmplitudeCurve& peakShapingCurve;
		//glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, frequencyBins, 0, GL_RED, GL_FLOAT, NULL);
		//frequencyTexture.resize(frequencyBins);
		analyzer.setFrameSize(frameSize);
		audioBuffer.resize(numAudioSamples);
		domainShiftFilter.setDomainShiftFactor(domainShift);
		frequencyAmplitudeCurve.recalculate();
		amplitudeFilter.setAmplitudeCurve(frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize);
		peakCurveSize = (int)((float)frequencyBins * peakRadius);
		peakShapingCurve.curve1DSize = peakCurveSize;
		peakShapingCurve.recalculate();
		peakFilter.setPeakCurve(peakShapingCurve.curve1D, peakShapingCurve.curve1DSize);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	void menu()
	{
		if(ImGui::TreeNode("Audio Visualizer"))
		{
			if (ImGui::expArrowButtons("audio frame size: %d", &frameSize, 2, 65536))
			{
				analyzer.setFrameSize(frameSize);
				numAudioSamples = frameSize * 2;
				audioBuffer.resize(numAudioSamples);
			}
			ImGui::SameLine(); ImGui::ShowHelpMarker("Number of audio samples used in the fourier transform.\nHigher values have more frequency information, but less time information.");
			ImGui::TreePop();

			int audioFPS = loopback_samplesPerSec() / frameGap;
			if (ImGui::SliderInt("audio frames per sec", &audioFPS, 20, 600))
			{
				audioFPS = utl::clamp(audioFPS, 1, 3000);
				frameGap = loopback_samplesPerSec() / audioFPS;
			}
			ImGui::SameLine(); ImGui::ShowHelpMarker("Controls how many fourier transforms happen per second.\nThis is essentially the framerate of the frequency spectrum.");

			if (ImGui::SliderInt("audio frames used", &spectrumsInAverage, 1, 40))
				averageFilter.setNumSpectrumsInAverage(utl::clamp(spectrumsInAverage, 1, 200));
			ImGui::SameLine(); ImGui::ShowHelpMarker("The final displayed frequency spectrum is an average of this many spectrums.\nRaise to increase smoothness.");

			int totalSamples = analyzer.getFrameSize() + frameGap * averageFilter.getNumSpectrumsInAverage();
			ImGui::Text("time of utilized audio: %.3f sec", (float)totalSamples / (float)loopback_samplesPerSec());

			ImGui::SliderFloat("log domain shift factor", &domainShift, 1.0f, 10.0f);
			domainShiftFilter.setDomainShiftFactor(domainShift);
			ImGui::SameLine(); ImGui::ShowHelpMarker("Shifts frequency domain onto a logarithmic scale.\
				\n1.0: all frequency bins are spaced evenly.\
				\n10.0: frequency bins are spaced at powers of 10\
				\nIt looks good to use values greater than 10.0 with very large frame sizes. CTRL + CLICK the slider to enter a value manually.");

			bool changed = false;
			if (ImGui::TreeNode("Frequency Amplitude Curve"))
			{
				changed = frequencyAmplitudeCurve.menu("");
				ImGui::TreePop();
			}
			if (changed)
			{
				frequencyAmplitudeCurve.recalculate();
				amplitudeFilter.setAmplitudeCurve(frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize);
			}

			// Control the frequency peak curve
			if (ImGui::TreeNode("Frequency Peak Curve"))
			{
				changed = ImGui::SliderFloat("Blur Radius", &peakRadius, 0.0f, 0.1f);
				changed |= peakShapingCurve.menu("");
				ImGui::TreePop();
			}
			if (changed)
			{
				peakCurveSize = (int)((float)frequencyBins * peakRadius);
				peakShapingCurve.curve1DSize = peakCurveSize;
				peakShapingCurve.recalculate();
				peakFilter.setPeakCurve(peakShapingCurve.curve1D, peakShapingCurve.curve1DSize);
			}
		}
	}
};

class PhysicalMaterial
{
public:
	glm::vec3 albedo;
	float roughness;
	float metallic;
	float ao;
	
	void bindGlobalUniforms(std::string uniformName)
	{
		Shader::bindGlobalUniform(uniformName + ".albedo", &albedo);
		Shader::bindGlobalUniform(uniformName + ".metallic", &metallic);
		Shader::bindGlobalUniform(uniformName + ".roughness", &roughness);
		Shader::bindGlobalUniform(uniformName + ".ao", &ao);
	}

	void Menu(const char * name)
	{
		if (ImGui::TreeNode(name))
		{
			const int flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR;
			ImGui::ColorEdit3("albedo", (float*)&albedo, flags);
			ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f);
			ImGui::SliderFloat("ao", &ao, 0.0f, 1.0f);
			ImGui::TreePop();
		}
	}
	
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& albedo& roughness& metallic& ao;
	}
};

class GLFeedbackProgram : public GLProgram {
public:
	struct FluidSplat {
		float radius, velocity, pressure, density;
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 euler = eulerAngles(rotation);
		glm::mat3 rotationMatrix{ rotation };

		void bindGlobalUniforms(std::string uniformName)
		{
			Shader::bindGlobalUniform(uniformName + ".radius", &radius);
			Shader::bindGlobalUniform(uniformName + ".velocity", &velocity);
			Shader::bindGlobalUniform(uniformName + ".pressure", &pressure);
			Shader::bindGlobalUniform(uniformName + ".density", &density);
			Shader::bindGlobalUniform(uniformName + ".rotation", &rotationMatrix);
		}
		void Menu(const char* name)
		{
			if (ImGui::TreeNode(name))
			{
				ImGui::SliderFloat("radius", &radius, 1.0f, 150.0f);
				ImGui::SliderFloat("velocity add scalar", &velocity, 0.00f, 15.0f);

				if (ImGui::SliderFloat3("Euler Angles", (float*)&euler, -360.0f, 360.0))
				{
					rotation = glm::quat(glm::radians(euler));
					rotationMatrix = glm::mat3(rotation);
				}

				if (ImGui::gizmo3D("velocity rotation", rotation, 100, imguiGizmo::modeDirPlane))
				{
					euler = eulerAngles(rotation);
					rotationMatrix = glm::mat3(rotation);
				}

				ImGui::SliderFloat("pressure add scalar", &pressure, 0.00f, 100.0f);
				ImGui::SliderFloat("density add scalar", &density, 0.0f, 15.0f);
				ImGui::TreePop();
			}
		}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (version > 0)
				ar& rotation;
			ar& radius& velocity& pressure& density;
			rotationMatrix = glm::mat3(rotation);
		}
	};

	struct ScatteringData
	{
		glm::vec3 extinction;
		glm::vec3 scattering;
		float phaseScalar;
		void bindGlobalUniforms(std::string uniformName)
		{
			Shader::bindGlobalUniform(uniformName + ".extinction", &extinction);
			Shader::bindGlobalUniform(uniformName + ".scattering", &scattering);
			Shader::bindGlobalUniform(uniformName + ".phaseScalar", &phaseScalar);
		}
	};

	struct Settings {
		glm::ivec3 gridSize = { 64, 64, 64 };
		int numPressureIterations = 12;
		float velocityDissipation = 1.0f;
		float densityDissipation = 0.970f;
		bool vorticityEnabled = true;
		float vorticityScalar = 0.1f;
		float timeScale = 1.0f;
		FluidSplat mouseFluidSplat = { 25.0f, 0.75f, 1.0f, 0.4f };
		FluidSplat externalFluidSplat = { 8.0f, 30.0f, 1.0f, 0.1f };
		ImGradient fluidGradient;

		glm::vec3 backgroundColor{ 0.0f };
		float rayStepSize = 1.0f;
		float shadowStepSize = 4.0f;
		float maxShadowingLength = 12.0f;
		float maxDensity = 8.0f;
		float directionalLightExtinction = 20.0f;
		float ambientLightExtinction = 12.0f;

		glm::vec3 scattering{ 6.25f, 12.5f, 25.0f };
		glm::vec3 absorption{ 0.75f, 0.5f, 0.0f };
		glm::vec3 directionalLightLuminance{ 1.0f, 1.0f, 1.0f };
		glm::vec3 directionalLightDirection{ -1.0, -1.0, -1.0 };

		int phaseMode = 1;
		int mieMode = 1;
		bool mieMultiLobe = false;
		float mieG1 = 0.7f;
		float mieG2 = 0.2f;
		float mieLobeMix = 0.5f;

		int multiScatteringOctaves = 1;
		float octaveExtinctionDecay = 0.5;
		float octaveScatteringDecay = 0.5;
		float octavePhaseDecay = 0.5;

		AudioTexture audioTexture{ 7, 1024, 128, 1024, 8, 5.0f, 0.005f };

		int velocityBoundaryMode = 1;
		int pressureBoundaryMode = 1;

		PhysicalMaterial planeMaterial;

		float buoyancy = 0.0;

		float azimuth = 90.0f;
		float elevation = 45.0f;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (version > 6)
				ar& azimuth& elevation;
			if (version > 5)
				ar& buoyancy;
			if (version > 4)
				ar& planeMaterial;
			if (version > 3)
				ar& velocityBoundaryMode& pressureBoundaryMode;
			if (version > 2)
				ar& directionalLightDirection& multiScatteringOctaves& octaveExtinctionDecay& octaveScatteringDecay
				& octavePhaseDecay& audioTexture;
			if (version > 1)
				ar& scattering& absorption& directionalLightLuminance& phaseMode& mieMode& mieMultiLobe
				& mieG1& mieG2& mieLobeMix;
			if (version > 0)
				ar& gridSize& backgroundColor& rayStepSize& shadowStepSize& maxShadowingLength
				& maxDensity& directionalLightExtinction& ambientLightExtinction;
			ar& numPressureIterations& velocityDissipation& densityDissipation& vorticityEnabled
				& vorticityScalar& timeScale& mouseFluidSplat& externalFluidSplat& fluidGradient;
		}
	} settings;

	View view;
	Shader raymarch;
	Shader compute;
	Shader shadowMap;
	Shader advection, curl, vorticity, divergence, pressure, subtractPressureGradient;
	ImguiPresetMenu<Settings> presetMenu;
	//ColorGradientTexture fluidGradientTexture;

	/*
	struct TextureDefinition {
		unsigned int textureUnit;
		glm::vec3 size;
		GLenum internalFormat{ GL_RGBA32F };
		GLenum format{ GL_RGBA };
		GLenum filter{ GL_LINEAR };
		GLenum wrap{ GL_CLAMP_TO_EDGE };
		bool image{ false };
		GLenum imageMode{ GL_READ_WRITE };
		glm::vec4 borderColor{ 0.0f };
	};*/

	SlabTexture fluidTexture{
		{ 0, GL_TEXTURE_3D, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_MIRRORED_REPEAT, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	SlabTexture pressureTexture{
		{ 1, GL_TEXTURE_3D, settings.gridSize, GL_RG32F, GL_RG, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_EDGE, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	Texture curlTexture{
		{ 2, GL_TEXTURE_3D, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	SlabTexture densityTexture{
		{ 3, GL_TEXTURE_3D, settings.gridSize, GL_R32F, GL_RED, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	Texture shadowMapTexture{
		{ 5, GL_TEXTURE_3D, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};

	Texture combinedTexture, sceneTexture, luminanceTexture, transmittanceTexture;

	xt::xarray<float> captureArray{ xt::xarray<float>::shape_type{1, 1, 1} };

	unsigned int quadVAO{};

	unsigned int captureFBO{};

	#define MAX_SCATTERING_OCTAVES 8
	ScatteringData scatteringData[MAX_SCATTERING_OCTAVES];

	glm::vec3 mouseSplatPos, prevMouseSplatPos;
	bool mouseSplatActive;
	float mouseDepth = 0.0f;
	
	GLFeedbackProgram() :
		GLProgram(4, 4, true, 1600, 900, "Fluid sim", true),
		raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/feedback.frag"),
		compute(GL_COMPUTE_SHADER, "shaders/compute/feedback.comp"),
		shadowMap(GL_COMPUTE_SHADER, "shaders/compute/shadowMap.comp"),
		advection(GL_COMPUTE_SHADER, "shaders/compute/advection.comp"),
		curl(GL_COMPUTE_SHADER, "shaders/compute/curl.comp"),
		vorticity(GL_COMPUTE_SHADER, "shaders/compute/vorticity.comp"),
		divergence(GL_COMPUTE_SHADER, "shaders/compute/divergence.comp"),
		pressure(GL_COMPUTE_SHADER, "shaders/compute/pressure.comp"),
		subtractPressureGradient(GL_COMPUTE_SHADER, "shaders/compute/subtractPressureGradient.comp"),
		presetMenu(settings, "fluidSettings.txt"),
		combinedTexture{ {8, GL_TEXTURE_2D, glm::ivec3(screenSize, 0), GL_RGBA32F, GL_RGBA, GL_FLOAT} },
		sceneTexture{ {9, GL_TEXTURE_2D, glm::ivec3( screenSize, 0), GL_RGBA32F, GL_RGBA, GL_FLOAT} },
		luminanceTexture{ {10, GL_TEXTURE_2D, glm::ivec3(screenSize, 0), GL_R32F, GL_RED, GL_FLOAT} },
		transmittanceTexture{ {11, GL_TEXTURE_2D, glm::ivec3(screenSize, 0), GL_R32F, GL_RED, GL_FLOAT} }
		//fluidGradientTexture(4, settings.fluidGradient)
	{
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		unsigned int quadVBO;
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		combinedTexture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, combinedTexture.ID, 0);
		transmittanceTexture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, transmittanceTexture.ID, 0);
		luminanceTexture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, luminanceTexture.ID, 0);
		sceneTexture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, sceneTexture.ID, 0);
		
		GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		
		glDrawBuffers(4, attachments);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		initializeScatteringData();
		
		bindGlobalUniforms();
		updateVelocityBoundary();
		updatePressureBoundary();
		
		keyInputs[GLFW_KEY_T];
		keyInputs[GLFW_KEY_L];
		keyInputs[GLFW_KEY_C];

		glDisable(GL_BLEND);
	}

	void updateVelocityBoundary()
	{
		if (settings.velocityBoundaryMode == 0)
			fluidTexture.setWrap(GL_CLAMP_TO_BORDER, glm::vec4(0.0f));
		else if (settings.velocityBoundaryMode == 1)
			fluidTexture.setWrap(GL_MIRRORED_REPEAT);
		Shader::setGlobalDefinition("VELOCITY_BOUNDARY_MODE", std::to_string(settings.velocityBoundaryMode));
	}

	void updatePressureBoundary()
	{
		if (settings.pressureBoundaryMode == 0)
			pressureTexture.setWrap(GL_CLAMP_TO_BORDER, glm::vec4(0.0f));
		else if (settings.velocityBoundaryMode == 1)
			pressureTexture.setWrap(GL_CLAMP_TO_EDGE);
	}
	
	void initializeScatteringData()
	{
		scatteringData[0].extinction = settings.absorption + settings.scattering;
		scatteringData[0].scattering = settings.scattering;
		scatteringData[0].phaseScalar = 1.0f;

		for (int i = 1; i < settings.multiScatteringOctaves; i++)
		{
			scatteringData[i].extinction = scatteringData[0].extinction * glm::pow(settings.octaveExtinctionDecay, i);
			scatteringData[i].scattering = scatteringData[0].scattering * glm::pow(settings.octaveScatteringDecay, i);
			scatteringData[i].phaseScalar = scatteringData[0].phaseScalar * glm::pow(settings.octavePhaseDecay, i);
		}
	}

	void resizeFluidTextures()
	{
		fluidTexture.setSize(settings.gridSize);
		pressureTexture.setSize(settings.gridSize);
		curlTexture.setSize(settings.gridSize);
		densityTexture.setSize(settings.gridSize);
		shadowMapTexture.setSize(settings.gridSize);
	}

	vec2 intersectBox(vec3 rayOrigin, vec3 rayDir, vec3 boxMin, vec3 boxMax)
	{
		vec3 tMin = (boxMin - rayOrigin) / rayDir;
		vec3 tMax = (boxMax - rayOrigin) / rayDir;
		vec3 t1 = min(tMin, tMax);
		vec3 t2 = max(tMin, tMax);
		float tNear = max(max(t1.x, t1.y), t1.z);
		float tFar = min(min(t2.x, t2.y), t2.z);
		return vec2(tNear, tFar);
	}

	bool intersectPlane(const vec3& planeNorm, const vec3& planePos, const vec3& rayPos, const vec3& rayDir, float& t)
	{
		// assuming vectors are all normalized
		float denom = dot(planeNorm, rayDir);
		if (denom > 1e-6) 
		{
			vec3 deltaPlane = planePos - rayPos;
			t = dot(deltaPlane, planeNorm) / denom;
			return t >= 0.0f;
		}
		return false;
	}

	void updateMouseSplat(float aspect)
	{
		prevMouseSplatPos = mouseSplatPos;
		
		vec3 Eye = vec3(view.matrix[3]);
		vec2 viewMouse = mousePos / glm::vec2(screenSize);
		viewMouse.y = 1.0f - viewMouse.y;
		viewMouse = viewMouse * 2.0f - vec2(1.0f);
		viewMouse.x *= aspect;
		vec3 worldMousePos = view.matrix * vec4(viewMouse, -2.0f, 1.0f);
		vec3 mouseRayDir = normalize(worldMousePos - Eye);
		
		vec3 boxCenter = vec3(0.0f);
		vec3 planeNorm = normalize(vec3(boxCenter - Eye));
		boxCenter += planeNorm * sin(time) * mouseDepth;
		
		float intersectDistance = 0.0f;
		bool intersection = intersectPlane(planeNorm, boxCenter, Eye, mouseRayDir, intersectDistance);
		mouseSplatActive = intersection;
		mouseSplatPos = Eye + mouseRayDir * intersectDistance;
	}

	void update() override
	{
		menu();

		std::random_device rd;
		std::mt19937 gen(rd());

		float phi = glm::radians(90.0f - settings.elevation);
		float theta = glm::radians(settings.azimuth);
		settings.directionalLightDirection = -glm::vec3(
			glm::cos(phi) * glm::cos(theta),
			glm::sin(phi),
			glm::cos(phi) * glm::sin(theta)
		);

		if (mouseInputs[GLFW_MOUSE_BUTTON_1].pressed)
		{
			std::uniform_real_distribution<> dis(-30.0f, 30.0f);
			mouseDepth = dis(gen);
		}
		
		auto fluidStep = [](auto& computeShader, glm::ivec3& gridSize)
		{
			computeShader.update();
			computeShader.use();
			glDispatchCompute(gridSize.x / 8, gridSize.y / 8, gridSize.z / 8);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
		};

		settings.audioTexture.update();

		fluidStep(advection, settings.gridSize);
		fluidTexture.swap();
		densityTexture.swap();
		pressureTexture.swap();

		if (settings.vorticityEnabled)
		{
			fluidStep(curl, settings.gridSize);
			fluidStep(vorticity, settings.gridSize);
			fluidTexture.swap();
		}
		
		fluidStep(divergence, settings.gridSize);
		pressureTexture.swap();
		
		for (int i = 0; i < settings.numPressureIterations; i++)
		{
			fluidStep(pressure, settings.gridSize);
			pressureTexture.swap();
		}
		
		fluidStep(subtractPressureGradient, settings.gridSize);
		fluidTexture.swap();

		fluidStep(shadowMap, settings.gridSize);

			
		// clear stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get view and projection matrices
		view.updateView2(window, deltaMousePos, deltaTime);
		const float aspect = screenSize.y != 0.0f ? (float)screenSize.x / (float)screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);
		updateMouseSplat(aspect);

		if (keyInputs[GLFW_KEY_V].pressed)
		{
			std::uniform_real_distribution<> azimuth_dis(0.0f, 360.0f);
			std::uniform_real_distribution<> elevation_dis(0.0f, 90.0f);
			settings.azimuth = azimuth_dis(gen);
			settings.elevation = elevation_dis(gen);
		}

		if (keyInputs[GLFW_KEY_C].pressed)
		{
			
			
			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			glViewport(0, 0, screenSize.x, screenSize.y);
			combinedTexture.bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, combinedTexture.ID, 0);
			transmittanceTexture.bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, transmittanceTexture.ID, 0);
			luminanceTexture.bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, luminanceTexture.ID, 0);
			sceneTexture.bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, sceneTexture.ID, 0);
			GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, attachments);
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
				std::cout << "capture framebuffer complete" << std::endl;
			else
				std::cout << "capture framebuffer incomplete" << std::endl;

			
			
			raymarch.update();
			raymarch.use();
			raymarch.setUniform("aspect", aspect);
			raymarch.setUniform("rayStepSize", 0.1f);
			raymarch.setUniform("shadowStepSize", 0.1f);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			captureArray.resize({ 5, (uint)screenSize.y, (uint)screenSize.x });
			int size = screenSize.x * screenSize.y;
			std::vector<float> transmittanceCaptureBuffer(size);
			std::vector<float> luminanceCaptureBuffer(size);
			std::vector<glm::vec4> sceneCaptureBuffer(size);
			
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glReadPixels(0, 0, screenSize.x, screenSize.y, transmittanceTexture.defn.format, transmittanceTexture.defn.type, transmittanceCaptureBuffer.data());
			glReadBuffer(GL_COLOR_ATTACHMENT2);
			glReadPixels(0, 0, screenSize.x, screenSize.y, luminanceTexture.defn.format, luminanceTexture.defn.type, luminanceCaptureBuffer.data());
			glReadBuffer(GL_COLOR_ATTACHMENT3);
			glReadPixels(0, 0, screenSize.x, screenSize.y, sceneTexture.defn.format, sceneTexture.defn.type, sceneCaptureBuffer.data());
			
			/*
			sceneTexture.bind();
			glGetTexImage(GL_TEXTURE_2D, 0, sceneTexture.defn.format, sceneTexture.defn.type, sceneCaptureBuffer.data());
			luminanceTexture.bind();
			glGetTexImage(GL_TEXTURE_2D, 0, luminanceTexture.defn.format, luminanceTexture.defn.type, luminanceCaptureBuffer.data());
			transmittanceTexture.bind();
			glGetTexImage(GL_TEXTURE_2D, 0, transmittanceTexture.defn.format, transmittanceTexture.defn.type, transmittanceCaptureBuffer.data());
			*/
			
			/*
			glGetTextureImage(sceneTexture.ID, 0, sceneTexture.defn.format, sceneTexture.defn.type, size * sizeof(glm::vec4), sceneCaptureBuffer.data());
			glGetTextureImage(transmittanceTexture.ID, 0, transmittanceTexture.defn.format, transmittanceTexture.defn.type, size * sizeof(float), transmittanceCaptureBuffer.data());
			glGetTextureImage(luminanceTexture.ID, 0, luminanceTexture.defn.format, luminanceTexture.defn.type, size * sizeof(float), luminanceCaptureBuffer.data());
			*/
			for (int j = 0; j < screenSize.y; j++)
			for (int i = 0; i < screenSize.x; i++)
			{
				captureArray(0, j, i) = transmittanceCaptureBuffer[j * screenSize.x + i];
				captureArray(1, j, i) = luminanceCaptureBuffer[j * screenSize.x + i];
				captureArray(2, j, i) = sceneCaptureBuffer[j * screenSize.x + i].x;
				captureArray(3, j, i) = sceneCaptureBuffer[j * screenSize.x + i].y;
				captureArray(4, j, i) = sceneCaptureBuffer[j * screenSize.x + i].z;
			}
			
			std::uniform_int_distribution<> dis(1000000, INT_MAX);
			std::string fileName = "fluid3D_captures/capture_" + std::to_string(dis(gen)) + ".npy";
			xt::dump_npy(fileName, captureArray);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		raymarch.update();
		raymarch.use();
		raymarch.setUniform("aspect", aspect);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	void bindGlobalUniforms()
	{
		Shader::bindGlobalUniform("time", &time);
		Shader::bindGlobalUniform("deltaTime", &deltaTime);
		Shader::bindGlobalUniform("view", &(view.matrix));

		Shader::bindGlobalUniform("screenMousePos", &mousePos);
		Shader::bindGlobalUniform("prevScreenMousePos", &prevMousePos);
		Shader::bindGlobalUniform("mouseSplatPos", &mouseSplatPos);
		Shader::bindGlobalUniform("prevMouseSplatPos", &prevMouseSplatPos);
		Shader::bindGlobalUniform("mouseSplatActive", &mouseSplatActive);
		Shader::bindGlobalUniform("leftMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_1].held);
		Shader::bindGlobalUniform("rightMouseDown", &mouseInputs[GLFW_MOUSE_BUTTON_2].held);
		
		Shader::bindGlobalUniform("fluidImage", &fluidTexture.defn.textureUnit);
		Shader::bindGlobalUniform("fluidSampler", &fluidTexture.defn.textureUnit);
		//Shader::bindGlobalUniform("pressureImage", &pressureTexture.textureUnit);
		//Shader::bindGlobalUniform("pressureSampler", &pressureTexture.textureUnit);
		Shader::bindGlobalUniform("curlImage", &curlTexture.defn.textureUnit);
		Shader::bindGlobalUniform("curlSampler", &curlTexture.defn.textureUnit);
		Shader::bindGlobalUniform("densityImage", &densityTexture.defn.textureUnit);
		Shader::bindGlobalUniform("densitySampler", &densityTexture.defn.textureUnit);
		Shader::bindGlobalUniform("shadowMapImage", &shadowMapTexture.defn.textureUnit);
		Shader::bindGlobalUniform("shadowMapSampler", &shadowMapTexture.defn.textureUnit);
		Shader::bindGlobalUniform("audioFrequencySampler", &settings.audioTexture.textureUnit);

		//Shader::bindGlobalUniform("cloudColorCurve", &fluidGradientTexture.textureUnit);

		Shader::bindGlobalUniform("fluidSize", &settings.gridSize);
		Shader::bindGlobalUniform("numPressureIterations", &settings.numPressureIterations);
		Shader::bindGlobalUniform("velocityDissipation", &settings.velocityDissipation);
		Shader::bindGlobalUniform("densityDissipation", &settings.densityDissipation);
		Shader::bindGlobalUniform("vorticityScalar", &settings.vorticityScalar);
		Shader::bindGlobalUniform("timeScale", &settings.timeScale);

		settings.mouseFluidSplat.bindGlobalUniforms("mouseSplat");
		settings.externalFluidSplat.bindGlobalUniforms("externalSplat");

		Shader::bindGlobalUniform("backgroundColor", &settings.backgroundColor);
		Shader::bindGlobalUniform("rayStepSize", &settings.rayStepSize);
		Shader::bindGlobalUniform("shadowStepSize", &settings.shadowStepSize);
		Shader::bindGlobalUniform("maxShadowingLength", &settings.maxShadowingLength);
		Shader::bindGlobalUniform("maxDensity", &settings.maxDensity);
		Shader::bindGlobalUniform("directionalLightExtinction", &settings.directionalLightExtinction);
		Shader::bindGlobalUniform("ambientLightExtinction", &settings.ambientLightExtinction);

		Shader::bindGlobalUniform("scattering", &settings.scattering);
		Shader::bindGlobalUniform("absorption", &settings.absorption);
		Shader::bindGlobalUniform("dirLight.color", &settings.directionalLightLuminance);
		Shader::bindGlobalUniform("dirLight.direction", &settings.directionalLightDirection);

		Shader::bindGlobalUniform("phaseMode", &settings.phaseMode);
		Shader::bindGlobalUniform("mieMode", &settings.mieMode);
		Shader::bindGlobalUniform("mieMultiLobe", &settings.mieMultiLobe);
		Shader::bindGlobalUniform("mieG1", &settings.mieG1);
		Shader::bindGlobalUniform("mieG2", &settings.mieG2);
		Shader::bindGlobalUniform("mieLobeMix", &settings.mieLobeMix);

		Shader::bindGlobalUniform("multiScatteringOctaves", &settings.multiScatteringOctaves);
		for (int i = 0; i < MAX_SCATTERING_OCTAVES; i++)
			scatteringData[i].bindGlobalUniforms("scatteringData[" + std::to_string(i) + "]");

		settings.planeMaterial.bindGlobalUniforms("planeMaterial");
		Shader::bindGlobalUniform("showLuminance", &keyInputs[GLFW_KEY_L].held);
		Shader::bindGlobalUniform("showTransmittance", &keyInputs[GLFW_KEY_T].held);
		Shader::bindGlobalUniform("captureSmoke", &keyInputs[GLFW_KEY_C].pressed);
		
		Shader::bindGlobalUniform("buoyancy", &settings.buoyancy);

		Shader::bindGlobalUniform("sceneImage", &sceneTexture.defn.textureUnit);
		Shader::bindGlobalUniform("transmittanceImage", &transmittanceTexture.defn.textureUnit);
		Shader::bindGlobalUniform("luminanceImage", &luminanceTexture.defn.textureUnit);

		Shader::bindGlobalUniform("screenSize", &screenSize);
	}

	template <typename Tex>
	void configureFluidTexture(Tex& texture, const std::string& macroPrefix, int numChannels, int bits, bool signedTex)
	{
		auto SetTexData = [&texture, &macroPrefix](GLenum iFmt, GLenum fmt, GLenum type,
			const std::string& imgFmt, const std::string& imgType, const std::string& samplerType, const std::string& storeOp)
		{
			texture.setFormat(iFmt, fmt, type);
			Shader::setGlobalDefinition(macroPrefix + "_FORMAT", imgFmt);
			Shader::setGlobalDefinition(macroPrefix + "_IMAGE_TYPE", imgType);
			Shader::setGlobalDefinition(macroPrefix + "_SAMPLER_TYPE", samplerType);
			Shader::setGlobalDefinition(macroPrefix + "_STORE_OP(data)", storeOp);
		};

		switch (numChannels)
		{
		case 4:
			switch (bits)
			{
			case 2: SetTexData(GL_RGBA32F, GL_RGBA, GL_FLOAT, "rgba32f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 1: SetTexData(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, "rgba16f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 0: signedTex ?
				SetTexData(GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE, "rgba8i"s, "iimage3D"s, "isampler3D"s, "ivec4(data * 255.0 - 128.0)"s) :
				SetTexData(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, "rgba8ui"s, "uimage3D"s, "usampler3D"s, "uvec4(data * 255.0)"s); break;
			}
			break;
		case 2:
			switch (bits)
			{
			case 2: SetTexData(GL_RG32F, GL_RG, GL_FLOAT, "rg32f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 1: SetTexData(GL_RG16F, GL_RG, GL_HALF_FLOAT, "rg16f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 0: signedTex ?
				SetTexData(GL_RG8I, GL_RG_INTEGER, GL_BYTE, "rg8i"s, "iimage3D"s, "isampler3D"s, "ivec4(data * 255.0 - 128.0)"s) :
				SetTexData(GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE, "rg8ui"s, "uimage3D"s, "usampler3D"s, "uvec4(data * 255.0)"s); break;
			}
			break;
		case 1:
			switch (bits)
			{
			case 2: SetTexData(GL_R32F, GL_RED, GL_FLOAT, "r32f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 1: SetTexData(GL_R16F, GL_RED, GL_HALF_FLOAT, "r16f"s, "image3D"s, "sampler3D"s, "data"s); break;
			case 0: signedTex ?
				SetTexData(GL_R8I, GL_RED_INTEGER, GL_BYTE, "r8i"s, "iimage3D"s, "isampler3D"s, "ivec4(data * 255.0 - 128.0)"s) :
				SetTexData(GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, "r8ui"s, "uimage3D"s, "usampler3D"s, "uvec4(data * 255.0)"s); break;
			}
			break;
		}

	};

	void menu()
	{
		ImGuiWindowFlags windowFlags = 0;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			windowFlags |= ImGuiWindowFlags_NoInputs;

		ImGui::Begin("Settings (Press CTRL to activate mouse)", nullptr, windowFlags);
		{
			ImGui::Text("Use WASD to move, SPACE to rise, LEFT SHIFT to fall.");
			const bool presetLoaded = presetMenu.Menu();
			if (presetLoaded)
			{
				//fluidGradientTexture.upload(settings.fluidGradient);
				resizeFluidTextures();
			}
				
			if (ImGui::TreeNode("Fluid Behavior"))
			{
				static glm::ivec3 gridSizeTemp = settings.gridSize;
				ImGui::SliderInt3("fluid size", (int*)&gridSizeTemp, 8, 256);
				const glm::ivec3 localWorkGroupSize(8, 8, 8);
				gridSizeTemp = (gridSizeTemp / localWorkGroupSize) * localWorkGroupSize;
				if (ImGui::Button("Set Fluid Size"))
				{
					settings.gridSize = gridSizeTemp;
					resizeFluidTextures();
					initializeScatteringData();
				}
				ImGui::SliderInt("pressure iterations", &settings.numPressureIterations, 1, 200);
				ImGui::SliderFloat("timeScale", &settings.timeScale, 0.01f, 5.0f);
				ImGui::SliderFloat("velocity dissipation", &settings.velocityDissipation, 0.9f, 1.0f);
				ImGui::SliderFloat("density dissipation", &settings.densityDissipation, 0.9f, 1.0f);
				ImGui::Checkbox("vorticity enabled", &settings.vorticityEnabled);
				ImGui::SliderFloat("vorticity", &settings.vorticityScalar, 0.0f, 1.0f);
				ImGui::SliderFloat("buoyancy", &settings.buoyancy, 0.0, 5.0);

				const char* velocityBoundaryModes[] = { "zero everywhere", "negative mirror" };
				if (ImGui::Combo("velocity boundary mode", &settings.velocityBoundaryMode, velocityBoundaryModes, IM_ARRAYSIZE(velocityBoundaryModes)))
					updateVelocityBoundary();

				const char* pressureBoundaryModes[] = { "zero everywhere", "clamp to edge" };
				if (ImGui::Combo("pressure boundary mode", &settings.pressureBoundaryMode, pressureBoundaryModes, IM_ARRAYSIZE(pressureBoundaryModes)))
					updatePressureBoundary();
				ImGui::TreePop();
			}
			settings.externalFluidSplat.Menu("External Forces");
			settings.mouseFluidSplat.Menu("Mouse Interaction");

			if (ImGui::TreeNode("Rendering"))
			{
				//fluidGradientTexture.Menu("Color Gradient", settings.fluidGradient);
				ImGui::ColorEdit3("background color", (float*)&settings.backgroundColor);
				
				ImGui::SliderFloat("ray step size", &settings.rayStepSize, 0.1f, 4.0f);
				settings.rayStepSize = utl::max(settings.rayStepSize, 0.05f);
				ImGui::SliderFloat("shadow step size", &settings.shadowStepSize, 0.1f, 4.0f);
				settings.shadowStepSize = utl::max(settings.shadowStepSize, 0.05f);
				ImGui::SliderFloat("max shadowing length", &settings.maxShadowingLength, 1.0f, 512.0f);
				//ImGui::SliderFloat("max density accumulation", &settings.maxDensity, 2.0f, 20.0f);
				//ImGui::SliderFloat("directional light extinction", &settings.directionalLightExtinction, 1.0, 100.0f);
				//ImGui::SliderFloat("ambient light extinction", &settings.ambientLightExtinction, 1.0f, 100.0f);

				static glm::vec3 extinction = settings.scattering + settings.absorption;
				static glm::vec3 albedo = settings.scattering / extinction;
				
				const int flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR;
				bool changed = ImGui::ColorEdit3("absorption", (float*)&settings.absorption, flags);
				changed |= ImGui::ColorEdit3("scattering", (float*)&settings.scattering, flags);
				if (changed)
				{
					extinction = settings.scattering + settings.absorption;
					albedo = settings.scattering / extinction;
					initializeScatteringData();
				}
				
				changed = ImGui::ColorEdit3("extinction", (float*)&extinction, flags);
				changed |= ImGui::ColorEdit3("albedo", (float*)&albedo, flags);
				if (changed)
				{
					settings.scattering = albedo * extinction;
					settings.absorption = extinction - settings.scattering;
					initializeScatteringData();
				}
				
				ImGui::ColorEdit3("directional light luminance", (float*)&settings.directionalLightLuminance, flags);

				ImGui::SliderFloat("directional light azimuth", &settings.azimuth, 0.0f, 360.0f);
				ImGui::SliderFloat("directional light elevation", &settings.elevation, 0.0f, 90.0f);

				const char* phaseModes[] = { "Isotropic", "Rayleigh", "Mie" };
				ImGui::Combo("phase mode", &settings.phaseMode, phaseModes, IM_ARRAYSIZE(phaseModes));
				if (settings.phaseMode == 2)
				{
					const char* mieModes[] = { "Henyey-Greenstein", "Schlick Approximation"};
					ImGui::Combo("mie mode", &settings.mieMode, mieModes, IM_ARRAYSIZE(mieModes));
					bool changed = false;
					changed = ImGui::SliderInt("multi scattering octaves", &settings.multiScatteringOctaves, 1, 8);
					settings.multiScatteringOctaves = glm::clamp(settings.multiScatteringOctaves, 1, 8);
					changed |= ImGui::SliderFloat("octave extinction decay", &settings.octaveExtinctionDecay, 0.0f, 1.0f);
					changed |= ImGui::SliderFloat("octave scattering decay", &settings.octaveScatteringDecay, 0.0f, 1.0f);
					changed |= ImGui::SliderFloat("octave phase decay", &settings.octavePhaseDecay, 0.0f, 1.0f);
					if (changed)
						initializeScatteringData();
					ImGui::Checkbox("multi lobe", &settings.mieMultiLobe);
					if (settings.mieMultiLobe)
					{
						char str0[128] = "https://www.desmos.com/calculator/zdkjxh8msf";
						ImGui::InputText("multi lobe phase graph", str0, IM_ARRAYSIZE(str0));
					}
					ImGui::SliderFloat("mie g1", &settings.mieG1, 0.0, 1.0);
					if (settings.mieMultiLobe)
					{
						ImGui::SliderFloat("mie g2", &settings.mieG2, 0.0, 1.0);
						ImGui::SliderFloat("mie lobe mix", &settings.mieLobeMix, 0.0, 1.0);
					}
				}

				settings.planeMaterial.Menu("plane material");
				ImGui::TreePop();
			}

			settings.audioTexture.menu();

			if (ImGui::TreeNode("Debug Settings (unsaved)"))
			{
				if (ImGui::TreeNode("Texture bits"))
				{
					static int velocityBitMode = 2, pressureBitMode = 2, curlBitMode = 2, densityBitMode = 2, shadowBitMode = 2;
					const char* bitModes[] = { "8 bit", "16 bit", "32 bit" };
					if (ImGui::Combo("velocity format", &velocityBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(fluidTexture, "FLUID", 4, velocityBitMode, true);
					if (ImGui::Combo("pressure format", &pressureBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(pressureTexture, "PRESSURE", 2, pressureBitMode, true);
					if (ImGui::Combo("curl format", &curlBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(curlTexture, "CURL", 4, curlBitMode, true);
					if (ImGui::Combo("density format", &densityBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(densityTexture, "DENSITY", 1, densityBitMode, false);
					if (ImGui::Combo("shadow format", &shadowBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(shadowMapTexture, "SHADOWMAP", 4, shadowBitMode, false);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			
			static int lastError = 0;
			const int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current OpenGL Error Code: %d", currentError);
			ImGui::Text("Last OpenGL Error Code: %d", lastError);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Text("Left mouse down: %d Right mouse down: %d", mouseInputs[GLFW_MOUSE_BUTTON_1].held, mouseInputs[GLFW_MOUSE_BUTTON_2].held);
			ImGui::Text("Splat plane intersection %d", mouseSplatActive);
			ImGui::Text("Mouse splat pos %.3f, %.3f, %.3f", mouseSplatPos.x, mouseSplatPos.y, mouseSplatPos.z);
			ImGui::Text("Mouse pos x: %.3f, y: %.3f", mousePos.x, mousePos.y);
		}
		ImGui::End();

		//bool showDemoWindow = false;
		//ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

BOOST_CLASS_VERSION(GLFeedbackProgram::Settings, 7)
BOOST_CLASS_VERSION(GLFeedbackProgram::FluidSplat, 1)


