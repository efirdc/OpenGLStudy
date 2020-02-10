#ifndef BASE_SHADER_H
#define BASE_SHADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <map>
#include <regex>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
//#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shadinclude/Shadinclude.hpp"

class Shader
{
public:
	static void setGlobalUniform(const std::string& name, void* data);
	static void bindGlobalUniform(const std::string& name, void* data);
	static void unbindGlobalUniform(const std::string& name);

	static void setGlobalDefinition(const std::string& name, const std::string& definition);

	static bool inactiveUniformWarnings;

	class Stage
	{
	public:
		Stage(GLenum shaderType, std::string path) : shaderType(shaderType), path(path) {};

		GLenum shaderType;
		std::string path;
		long long int lastModifiedTime;
		std::string code;
		unsigned int ID;
		bool compiled = false;
	};


	Shader(std::vector<Stage> stages);

	Shader(std::string vertexPath, std::string fragmentPath) :
		Shader({
			{GL_VERTEX_SHADER, vertexPath},
			{GL_FRAGMENT_SHADER, fragmentPath}
		})
	{};

	Shader(std::string vertexPath, std::string geometryPath, std::string fragmentPath) :
		Shader({
			{GL_VERTEX_SHADER, vertexPath},
			{GL_GEOMETRY_SHADER, geometryPath},
			{GL_FRAGMENT_SHADER, fragmentPath}
		})
	{};

	Shader(GLenum shaderType, std::string path) :
		Shader({
			{shaderType, path}
		})
	{};
	
	~Shader();

	void setExtraCode(std::vector<std::string> & newExtraCode);
	std::vector<std::string> getExtraCode();
	
	void setDefinition(const std::string& name, const std::string& definition);
	
	bool update();
	/*
	* Recompiles the shader program if there are any changes to the vertex or fragment shader files
	* Pre:
	*	none. can be called every frame
	* Post:
	*	If the files at vertexPath or fragmentPath have been updated, then the shader program will be recompiled.
	*	If recompilation is successful, then the old shader program will be destroyed and the ID overwritten. Returns true
	*	This will reset any shader uniforms. Make sure to set uniforms after calling update()
	*	Setting a texture uniform has a large performance impact, so only do this if update returns true.
	*	If recompilation fails, the new shader program gets destroyed and the shader ID is not changed. Returns false
	*/

	void use();
	/*
	* binds the shader program with glUseProgram()
	*/

	void setUniform(const std::string & name, void * data);
	void bindUniform(const std::string & name, void * data);
	void unbindUniform(const std::string & name);

	template<typename... Args>
	void setUniform(const std::string& name, Args... args)
	{
		if (setUniformErrorCheck(name)) { return; }
		setUniform(activeUniforms[name]->location, args...);
	}
	template<typename... Args>
	void setUniform(const char* name, Args... args)
	{
		if (setUniformErrorCheck(name)) { return; }
		setUniform(activeUniforms[name]->location, args...);
	}
	/*
	void setUniform(const std::string & name, bool value);
	void setUniform(const std::string & name, int value);
	void setUniform(const std::string & name, unsigned int value);
	void setUniform(const std::string & name, float value);
	void setUniform(const std::string & name, const glm::vec2 & value);
	void setUniform(const std::string & name, const glm::vec3 & value);
	void setUniform(const std::string & name, const glm::vec4 & value);
	void setUniform(const std::string & name, float x, float y);
	void setUniform(const std::string & name, float x, float y, float z);
	void setUniform(const std::string & name, float x, float y, float z, float w);
	void setUniform(const std::string & name, const glm::mat2 & mat);
	void setUniform(const std::string & name, const glm::mat3 & mat);
	void setUniform(const std::string & name, const glm::mat4 & mat);
	*/
	

protected:
	static std::map<std::string, std::string> globalDefines;

	std::vector<Stage> stages;

	void compile(Stage& stage)
	{
		stage.lastModifiedTime = getModificationTime(stage.path);

		if (stage.compiled)
			glDeleteShader(stage.ID);
		
		char infoLog[512];
		int success;
		
		const char* codeC = stage.code.c_str();

		stage.ID = glCreateShader(stage.shaderType);
		glShaderSource(stage.ID, 1, &codeC, NULL);
		glCompileShader(stage.ID);
		glGetShaderiv(stage.ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			stage.compiled = false;
			glGetShaderInfoLog(stage.ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << name(stage)
				<< "::COMPILATION_FAILED::" << stage.path << "\n" << infoLog << std::endl;
			glDeleteShader(stage.ID);
			return;
		}
		stage.compiled = true;
	}

	void deleteAllShaderStages()
	{
		for (Stage& stage : stages)
		{
			if (stage.compiled)
				glDeleteShader(stage.ID);
			stage.compiled = false;
		}
	}

	bool isModified(Stage& stage)
	{
		return getModificationTime(stage.path) != stage.lastModifiedTime;
	}

	long long int getModificationTime(std::string& path)
	{
		struct stat result;
		if (stat(path.c_str(), &result) != 0)
			std::cout << "ERROR::SHADER::CANT_READ_FILE_STATUS::" << path << std::endl;
		return result.st_mtime;
	}

	const char* name(Stage& stage)
	{
		if (stage.shaderType == GL_FRAGMENT_SHADER) return "FRAGMENT";
		if (stage.shaderType == GL_VERTEX_SHADER) return "VERTEX";
		if (stage.shaderType == GL_GEOMETRY_SHADER) return "GEOMETRY";
		if (stage.shaderType == GL_COMPUTE_SHADER) return "COMPUTE";
		if (stage.shaderType == GL_TESS_CONTROL_SHADER) return "TESS_CONTROL";
		if (stage.shaderType == GL_TESS_EVALUATION_SHADER) return "TESS_EVALUATION";
		return "UNKNOWN_SHADER_TYPE";
	}

	void loadShaderCode(Stage& stage);
	bool createProgram();

	std::vector<std::string> extraCode;
	std::map<std::string, std::string> defines;
	bool shouldUpdate = false;

	struct Uniform
	{
		std::string name;
		int location;
		GLenum type;
		int size;
		void * data;
	};

	std::unordered_map<std::string, Uniform *> activeUniforms;
	std::vector<Uniform *> uniformBindings;

	unsigned int ID;
	bool compiled = false;
	static std::vector<Shader *> allShaders;
	static std::unordered_map<std::string, void *> globalUniformBindings;

	Uniform * getBoundUniform(const std::string & name);
	void bindGlobalUniforms();
	void setBoundUniforms();

	void setUniform(Uniform * uniform);
	void setUniform(int location, GLenum type, void * data);

	bool hasActiveUniform(const std::string & name);
	bool setUniformErrorCheck(const std::string & name);

	void getActiveUniforms();
	/*
	* Populates uniforms with this shaders uniform data
	* Pre:
	*	uniforms is empty
	* Post:
	*	uniforms is populated
	*/

	/*
	* Returns time the file was last modified.
	*/

	void setUniform(int location, bool value);
	void setUniform(int location, int value);
	void setUniform(int location, unsigned int value);
	void setUniform(int location, float value);
	void setUniform(int location, const glm::vec2 & value);
	void setUniform(int location, const glm::vec3 & value);
	void setUniform(int location, const glm::vec4 & value);
	void setUniform(int location, float x, float y);
	void setUniform(int location, float x, float y, float z);
	void setUniform(int location, float x, float y, float z, float w);
	void setUniform(int location, const glm::mat2 & mat);
	void setUniform(int location, const glm::mat3 & mat);
	void setUniform(int location, const glm::mat4 & mat);
	void setUniform(int location, int x, int y);
	void setUniform(int location, int x, int y, int z);
	void setUniform(int location, int x, int y, int z, int w);
	void setUniform(int location, const glm::ivec2& value);
	void setUniform(int location, const glm::ivec3& value);
	void setUniform(int location, const glm::ivec4& value);

	void printComputeShaderInfo();
};

#endif