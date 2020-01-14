#pragma once

#include <iostream>
#include <math.h>
#include <map>
#include <array>
#include "imgui_bezier_curve.h"
#include "SpectrumFilter.h"
#include "SpectrumAnalyzer.h"
#include "imgui_extras.h"

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
#include "ComputeShader.h"
#include "SceneManager.h"
#include "StreamTexture.h"
#include "View.h"
#include "ColorGradientTexture.h"
#include "GLProgram.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "utilities.h"

#include <boost/serialization/version.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

struct TextureDefinition {
	unsigned int textureUnit;
	glm::ivec3 size;
	GLenum internalFormat{ GL_RGBA32F };
	GLenum format{ GL_RGBA };
	GLenum type{ GL_FLOAT };
	GLenum filter{ GL_LINEAR };
	GLenum wrap{ GL_CLAMP_TO_EDGE };
	bool image{ false };
	GLenum imageMode{ GL_READ_WRITE };
	glm::vec4 borderColor{ 0.0f };
};

class AudioTexture
{
public:
	AudioTexture(unsigned int textureUnit, int frameSize, int frameGap, int frequencyBins, int spectrumsInAverage, float domainShift, float peakRadius) :
		textureUnit(textureUnit),
		frameSize(frameSize),
		frameGap(frameGap),
		frequencyBins(frequencyBins),
		numAudioSamples(frameSize * 2),
		spectrumsInAverage(spectrumsInAverage),
		domainShift(domainShift),
		peakRadius(peakRadius),
		analyzer{frameSize},
		peakCurveSize((int)((float)frequencyBins* peakRadius)),
		frequencyTexture{ GL_R32F, frequencyBins, GL_RED, GL_FLOAT, 1, 4, true },
		fAmpControlPoints{ { 0.016f, 0.016f },{ 0.0f, 1.0f } },
		frequencyAmplitudeCurve(fAmpControlPoints, bezierCurveSize, bezierCurveSize),
		peakControlPoints{ { 1.00f, 0.00f },{ 0.0f, 1.00f } },
		peakShapingCurve{ peakControlPoints, peakCurveSize, bezierCurveSize },
		amplitudeFilter{ frequencyAmplitudeCurve.curve1D, frequencyAmplitudeCurve.curve1DSize },
		domainShiftFilter{ domainShift, frequencyBins },
		peakFilter{ peakShapingCurve.curve1D, peakShapingCurve.curve1DSize },
		averageFilter{ spectrumsInAverage },
		audioBuffer{ new float[numAudioSamples]() }
	{
		bind();
		loopback_init();
		float* frequencyPixelBuffer = (float*)frequencyTexture.getPixelBuffer();
		for (int i = 0; i < frequencyTexture.width; i++)
			frequencyPixelBuffer[i] = 0.0f;
	}

	~AudioTexture()
	{
		delete[] audioBuffer;
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_1D, frequencyTexture.textureID);
	}
	unsigned int textureUnit;
	
	const int bezierCurveSize = 4096;
	int frameSize;
	int frameGap;
	int numAudioSamples;
	int frequencyBins;
	int spectrumsInAverage;
	float domainShift;

	float peakRadius;
	int peakCurveSize;
	StreamTexture1D frequencyTexture;
	float * audioBuffer;

	ImVec2 fAmpControlPoints[2];
	ImBezierCurve frequencyAmplitudeCurve;
	
	ImVec2 peakControlPoints[2];
	ImBezierCurve peakShapingCurve;

	SpectrumAnalyzer analyzer;
	AmplitudeFilter amplitudeFilter;
	DomainShiftFilter domainShiftFilter;
	PeakFilter peakFilter;
	AverageFilter averageFilter;

	void update()
	{
		static int newSamples = 0;
		newSamples += loopback_getSound(audioBuffer, numAudioSamples);
		static const FrequencySpectrum* frequencySpectrum = averageFilter.getFrequencySpectrum();
		while (newSamples >= frameGap)
		{
			newSamples -= frameGap;
			float* inputBuffer = analyzer.getFrameInputBuffer();
			int frameSize = analyzer.getFrameSize();
			int frameStart = frameSize - newSamples;
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
		float* frequencyPixelBuffer = (float*)frequencyTexture.getPixelBuffer();
		for (int i = 0; i < frequencySpectrum->size; i++)
			frequencyPixelBuffer[i] = frequencyData[i];
		frequencyTexture.unmapPixelBuffer();
	}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		int frameSize;
		int frameGap;
		int numAudioSamples;
		int frequencyBins;
		int spectrumsInAverage;
		float domainShift;
	}

	void menu()
	{
		if(ImGui::TreeNode("Audio Visualizer"))
		{
			int ival = analyzer.getFrameSize();
			if (ImGui::expArrowButtons("audio frame size: %d", &ival, 2, 65536))
			{
				analyzer.setFrameSize(ival);
				numAudioSamples = ival * 2;
				delete[] audioBuffer;
				audioBuffer = new float[numAudioSamples]();
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

			ival = averageFilter.getNumSpectrumsInAverage();
			if (ImGui::SliderInt("audio frames used", &ival, 1, 40))
				averageFilter.setNumSpectrumsInAverage(utl::clamp(ival, 1, 200));
			ImGui::SameLine(); ImGui::ShowHelpMarker("The final displayed frequency spectrum is an average of this many spectrums.\nRaise to increase smoothness.");

			int totalSamples = analyzer.getFrameSize() + frameGap * averageFilter.getNumSpectrumsInAverage();
			ImGui::Text("time of utilized audio: %.3f sec", (float)totalSamples / (float)loopback_samplesPerSec());

			float fval = domainShiftFilter.getDomainShiftFactor();
			ImGui::SliderFloat("log domain shift factor", &fval, 1.0f, 10.0f);
			domainShiftFilter.setDomainShiftFactor(fval);
			ImGui::SameLine(); ImGui::ShowHelpMarker("Shifts frequency domain onto a logarithmic scale.\
				\n1.0: all frequency bins are spaced evenly.\
				\n10.0: frequency bins are spaced at powers of 10\
				\nIt looks good to use values greater than 10.0 with very large frame sizes. CTRL + CLICK the slider to enter a value manually.");

			bool changed = false;
			if (ImGui::TreeNode("Frequency Amplitude Curve"))
			{
				changed = ImGui::Bezier("", frequencyAmplitudeCurve.controlPoints);
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
				changed |= ImGui::Bezier("", peakShapingCurve.controlPoints);
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

class Texture3D
{
public:
	unsigned int ID;
	TextureDefinition defn{};
	
	Texture3D(TextureDefinition defn) : defn(defn)
	{
		glGenTextures(1, &ID);
		configure();
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + defn.textureUnit);
		glBindTexture(GL_TEXTURE_3D, ID);
		if (defn.image)
			glBindImageTexture(defn.textureUnit, ID, 0, GL_TRUE, 0, defn.imageMode, defn.internalFormat);
	}

	void configure()
	{
		bind();
		glTexImage3D(GL_TEXTURE_3D, 0, defn.internalFormat, defn.size.x, defn.size.y, defn.size.z, 0, defn.format, defn.type, NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, defn.filter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, defn.filter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, defn.wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, defn.wrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, defn.wrap);
		if (defn.wrap == GL_CLAMP_TO_BORDER)
			glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, (float*)&defn.borderColor);
	}

	void setSize(glm::vec3 newSize)
	{
		defn.size = newSize;
		bind();
		glTexImage3D(GL_TEXTURE_3D, 0, defn.internalFormat, defn.size.x, defn.size.y, defn.size.z, 0, defn.format, defn.type, NULL);
	}

	void setImageMode(GLenum imageMode)
	{
		defn.imageMode = imageMode;
		bind();
	}

	void setFormat(GLenum internalFormat, GLenum format, GLenum type)
	{
		defn.internalFormat = internalFormat;
		defn.format = format;
		defn.type = type;
		bind();
		glTexImage3D(GL_TEXTURE_3D, 0, defn.internalFormat, defn.size.x, defn.size.y, defn.size.z, 0, defn.format, defn.type, NULL);
	}
};

class SlabTexture3D
{
private:
	Texture3D source, dest;
public:
	TextureDefinition defn{};

	SlabTexture3D(TextureDefinition defn) : defn(defn), source(defn), dest(defn)
	{
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + defn.textureUnit);
		glBindTexture(GL_TEXTURE_3D, source.ID);
		glBindImageTexture(defn.textureUnit, dest.ID, 0, GL_TRUE, 0, defn.imageMode, defn.internalFormat);
	}

	void swap()
	{
		std::swap(source, dest);
		bind();
	}

	void configure()
	{
		source.configure();
		dest.configure();
	}

	void setSize(glm::vec3 newSize)
	{
		defn.size = newSize;
		source.setSize(newSize);
		dest.setSize(newSize);
	}

	void setFormat(GLenum internalFormat, GLenum format, GLenum type)
	{
		defn.internalFormat = internalFormat;
		defn.format = format;
		defn.type = type;
		source.setFormat(internalFormat, format, type);
		dest.setFormat(internalFormat, format, type);
	}

	void setImageMode(GLenum imageMode)
	{
		defn.imageMode = imageMode;
		source.setImageMode(imageMode);
		dest.setImageMode(imageMode);
	}
};

class GLFeedbackProgram : public GLProgram {
public:
	struct FluidSplat {
		float radius, velocity, pressure, density;
		glm::quat rotation;
		glm::mat3 rotationMatrix;
		
		void bindGlobalUniforms(std::string uniformName) 
		{
			Shader::bindGlobalUniform(uniformName + ".radius", &radius);
			Shader::bindGlobalUniform(uniformName + ".velocity", &velocity);
			Shader::bindGlobalUniform(uniformName + ".pressure", &pressure);
			Shader::bindGlobalUniform(uniformName + ".density", &density);
			Shader::bindGlobalUniform(uniformName + ".rotation", &rotationMatrix);
		}
		void Menu(const char * name)
		{
			if (ImGui::TreeNode(name))
			{
				ImGui::SliderFloat("radius", &radius, 1.0f, 150.0f);
				ImGui::SliderFloat("velocity add scalar", &velocity, 0.00f, 15.0f);
				if (ImGui::gizmo3D("velocity rotation", rotation))
					rotationMatrix = glm::mat3(rotation);
				ImGui::SliderFloat("pressure add scalar", &pressure, 0.00f, 100.0f);
				ImGui::SliderFloat("density add scalar", &density, 0.0f, 15.0f);
				ImGui::TreePop();
			}
		}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			if (version > 0)
				ar & rotation;
			ar & radius & velocity & pressure & density;
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

		glm::vec3 scattering{6.25f, 12.5f, 25.0f};
		glm::vec3 absorption{0.75f, 0.5f, 0.0f};
		glm::vec3 directionalLightLuminance{1.0f, 1.0f, 1.0f};
		glm::vec3 directionalLightDirection{-1.0, -1.0, -1.0};

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

		bool singleComputeShader = false;
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			if (version > 1)
				ar & scattering & absorption & directionalLightLuminance & phaseMode & mieMode & mieMultiLobe
					& mieG1 & mieG2 & mieLobeMix;
			if (version > 0)
				ar & gridSize & backgroundColor & rayStepSize & shadowStepSize & maxShadowingLength
					& maxDensity & directionalLightExtinction & ambientLightExtinction;
			ar & numPressureIterations & velocityDissipation & densityDissipation & vorticityEnabled
				& vorticityScalar & timeScale & mouseFluidSplat & externalFluidSplat & fluidGradient;
		}
	} settings;
	
	View view;
	Shader raymarch;
	ComputeShader compute;
	ComputeShader shadowMap;
	ComputeShader advection, curl, vorticity, divergence, pressure, subtractPressureGradient;
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

	SlabTexture3D fluidTexture{
		{ 0, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	SlabTexture3D pressureTexture{
		{ 1, settings.gridSize, GL_RG32F, GL_RG, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	Texture3D curlTexture{
		{ 2, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	SlabTexture3D densityTexture{
		{ 3, settings.gridSize, GL_R32F, GL_RED, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};
	Texture3D shadowMapTexture{
		{ 5, settings.gridSize, GL_RGBA32F, GL_RGBA, GL_FLOAT,
			GL_LINEAR, GL_CLAMP_TO_BORDER, true, GL_WRITE_ONLY, {0.0, 0.0, 0.0, 0.0}}
	};

	AudioTexture audioTexture{ 7, 1024, 128, 1024, 8, 5.0f, 0.005f };
	
	unsigned int quadVAO{};

	#define MAX_SCATTERING_OCTAVES 8
	ScatteringData scatteringData[MAX_SCATTERING_OCTAVES];
	
	GLFeedbackProgram() :
		GLProgram(4, 4, true, 1600, 900, "Fluid sim", true),
		raymarch("shaders/raymarch/raymarch.vert", "shaders/raymarch/feedback.frag"),
		compute("shaders/compute/feedback.comp"),
		shadowMap("shaders/compute/shadowMap.comp"),
		advection("shaders/compute/advection.comp"),
		curl("shaders/compute/curl.comp"),
		vorticity("shaders/compute/vorticity.comp"),
		divergence("shaders/compute/divergence.comp"),
		pressure("shaders/compute/pressure.comp"),
		subtractPressureGradient("shaders/compute/subtractPressureGradient.comp"),
		presetMenu(settings, "fluidSettings.txt")
		//fluidGradientTexture(4, settings.fluidGradient)
	{
		settings.directionalLightDirection = glm::normalize(settings.directionalLightDirection);
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
		
		initializeScatteringData();
		
		bindGlobalUniforms();
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

	void updateImageModes()
	{
		if (settings.singleComputeShader)
		{
			fluidTexture.setImageMode(GL_READ_WRITE);
			curlTexture.setImageMode(GL_READ_WRITE);
			densityTexture.setImageMode(GL_READ_WRITE);
			shadowMapTexture.setImageMode(GL_WRITE_ONLY);
			return;
		}
		fluidTexture.setImageMode(GL_WRITE_ONLY);
		curlTexture.setImageMode(GL_WRITE_ONLY);
		densityTexture.setImageMode(GL_WRITE_ONLY);
		shadowMapTexture.setImageMode(GL_WRITE_ONLY);
	}


	void update() override
	{
		menu();

		auto fluidStep = [](auto& computeShader, glm::ivec3& gridSize)
		{
			computeShader.update();
			computeShader.use();
			glDispatchCompute(gridSize.x / 8, gridSize.y / 8, gridSize.z / 8);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
		};

		audioTexture.update();

		fluidStep(advection, settings.gridSize);
		fluidTexture.swap();
		densityTexture.swap();
		pressureTexture.swap();
		
		fluidStep(curl, settings.gridSize);
		fluidStep(vorticity, settings.gridSize);
		fluidTexture.swap();
		
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
		const float aspect = screenSize.y != 0.0f ? screenSize.x / screenSize.y : 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);

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
				updateImageModes();
			}
				
			if (ImGui::TreeNode("Fluid Behavior"))
			{
				//if (ImGui::Checkbox("single compute shader", &settings.singleComputeShader))
					//updateImageModes();
				
				if (ImGui::TreeNode("Texture bits"))
				{
					static int velocityBitMode = 2, pressureBitMode = 2, curlBitMode = 2, densityBitMode = 2, shadowBitMode = 2;
					const char* bitModes[] = { "8 bit", "16 bit", "32 bit" };
					if (ImGui::Combo("velocity format", &velocityBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(fluidTexture, "FLUID", 4, velocityBitMode, true);
					if (ImGui::Combo("pressure format", &pressureBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(pressureTexture, "PRESSURE", 2, pressureBitMode,true);
					if (ImGui::Combo("curl format", &curlBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(curlTexture, "CURL", 4, curlBitMode, true);
					if (ImGui::Combo("density format", &densityBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(densityTexture, "DENSITY", 1, densityBitMode, false);
					if (ImGui::Combo("shadow format", &shadowBitMode, bitModes, IM_ARRAYSIZE(bitModes)))
						configureFluidTexture(shadowMapTexture, "SHADOWMAP", 4, shadowBitMode, false);
					ImGui::TreePop();
				}
				
				static glm::ivec3 gridSizeTemp = settings.gridSize;
				ImGui::SliderInt3("fluid size", (int*)&gridSizeTemp, 8, 256);
				const glm::ivec3 localWorkGroupSize(8, 8, 8);
				gridSizeTemp = (gridSizeTemp / localWorkGroupSize) * localWorkGroupSize;
				if (ImGui::Button("Set Fluid Size"))
				{
					settings.gridSize = gridSizeTemp;
					resizeFluidTextures();
				}
				ImGui::SliderInt("pressure iterations", &settings.numPressureIterations, 1, 200);
				ImGui::SliderFloat("timeScale", &settings.timeScale, 0.01f, 5.0f);
				ImGui::SliderFloat("velocity dissipation", &settings.velocityDissipation, 0.9f, 1.0f);
				ImGui::SliderFloat("density dissipation", &settings.densityDissipation, 0.9f, 1.0f);
				ImGui::Checkbox("vorticity enabled", &settings.vorticityEnabled);
				ImGui::SliderFloat("vorticity", &settings.vorticityScalar, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			settings.externalFluidSplat.Menu("External Forces");
			settings.mouseFluidSplat.Menu("Mouse Interaction");

			if (ImGui::TreeNode("Rendering"))
			{
				//fluidGradientTexture.Menu("Color Gradient", settings.fluidGradient);
				ImGui::ColorEdit3("background color", (float*)&settings.backgroundColor);
				
				ImGui::SliderFloat("ray step size", &settings.rayStepSize, 0.1f, 4.0f);
				ImGui::SliderFloat("shadow step size", &settings.shadowStepSize, 0.1f, 4.0f);
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
				ImGui::gizmo3D("light dir", settings.directionalLightDirection);
				settings.directionalLightDirection = glm::normalize(settings.directionalLightDirection);

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
				ImGui::TreePop();
			}

			audioTexture.menu();
			
			static int lastError = 0;
			const int currentError = glGetError();
			if (currentError)
				lastError = currentError;
			ImGui::Text("Current OpenGL Error Code: %d", currentError);
			ImGui::Text("Last OpenGL Error Code: %d", lastError);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		//bool showDemoWindow = false;
		//ImGui::ShowDemoWindow(&showDemoWindow);
	}
};

BOOST_CLASS_VERSION(GLFeedbackProgram::Settings, 2)
BOOST_CLASS_VERSION(GLFeedbackProgram::FluidSplat, 1)


