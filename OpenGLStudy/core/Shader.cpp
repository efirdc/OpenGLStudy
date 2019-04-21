/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#include "Shader.h"

using namespace std;

std::vector<Shader *> Shader::allShaders;
std::unordered_map<std::string, void *> Shader::globalUniforms;

void Shader::setGlobalUniform(const std::string & name, void * data)
{
	if (globalUniforms.count(name))
	{
		globalUniforms[name] = data;
		return;
	}
	globalUniforms[name] = data;
	for (Shader * shader : allShaders)
		shader->bindGlobalUniforms();
}

void Shader::deleteGlobalUniform(const std::string & name)
{
	globalUniforms.erase(name);
	for (Shader * shader : allShaders)
		if (shader->boundUniforms.count(name) > 0)
			shader->boundUniforms.erase(name);
}

Shader::Shader(const char * vertexPath, const char * fragmentPath) :
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode;
	string fragmentCode;
	const char * cVertexCode = getCode(vertexPath, vertexCode);
	const char * cFragmentCode = getCode(fragmentPath, fragmentCode);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);
	
	// Make the shader program
	makeProgram(cVertexCode, cFragmentCode, ID);

	// Get shader uniforms
	getUniforms();

	// Add to vector of all shaders
	allShaders.push_back(this);

	// Bind global uniforms
	bindGlobalUniforms();
}

bool Shader::update()
{
	// Exit function if vertex and fragment shaders have not been modified
	long long int newVTime = getModificationTime(vertexPath);
	long long int newFTime = getModificationTime(fragmentPath);
	if (newVTime == vertexModifiedTime && newFTime == fragmentModifiedTime)
		return false;
	
	// Update modification times
	vertexModifiedTime = newVTime;
	fragmentModifiedTime = newFTime;

	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode;
	string fragmentCode;
	const char * cVertexCode = getCode(vertexPath, vertexCode);
	const char * cFragmentCode = getCode(fragmentPath, fragmentCode);

	// Make the shader program
	unsigned int newProgramID;
	if (makeProgram(cVertexCode, cFragmentCode, newProgramID))
	{
		glDeleteProgram(ID);
		ID = newProgramID;
	}
	else
		glDeleteProgram(newProgramID);

	uniforms.clear();
	getUniforms();

	cout << "Shader " << vertexPath << " " << fragmentPath << " recompiled." << endl;
	return true;
}

const char * Shader::getCode(const char * codePath, string & code)
{
	// Declare the shaderfile and make sure it can throw errors
	ifstream shaderFile;
	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	// Open the file, read the file buffer contents, close the file, and then convert the stream into a string
	try
	{
		shaderFile.open(codePath);
		stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << codePath << endl;
	}
	
	// Return as a c string
	return code.c_str();
}

bool Shader::makeProgram(const char * vShaderCode, const char * fShaderCode, unsigned int & newProgramID)
{
	// Declarations
	unsigned int vertex, fragment;
	int vSuccess, fSuccess, pSuccess;
	char infoLog[512];

	// Create anmd compile the vertex shader.
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED::" << vertexPath << "\n" << infoLog << std::endl;
	}

	// Create anmd compile the fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED::" << fragmentPath << "\n" << infoLog << endl;
	}

	// Link the vertex and fragment shader in a shader program
	newProgramID = glCreateProgram();
	glAttachShader(newProgramID, vertex);
	glAttachShader(newProgramID, fragment);
	glLinkProgram(newProgramID);
	glGetProgramiv(newProgramID, GL_LINK_STATUS, &pSuccess);
	if (!pSuccess)
	{
		glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED::" << vertexPath << "\n" << infoLog << endl;
	}

	// Clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Return true if both shaders and the program successfully compiled
	return vSuccess && fSuccess && pSuccess;
}

void Shader::getUniforms()
{
	int numUniforms;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);
	for (int i = 0; i < numUniforms; i++)
	{
		GLenum type;
		int size;
		const int maxNameSize = 64;
		char name[maxNameSize];
		glGetActiveUniform(ID, i, maxNameSize, NULL, &size, &type, name);
		Uniform newUniform;
		newUniform.location = glGetUniformLocation(ID, name);
		newUniform.name = std::string(name);
		newUniform.size = size;
		newUniform.type = type;
		uniforms[newUniform.name] = newUniform;
	}
}

long long int Shader::getModificationTime(const char * filePath)
{
	struct stat result;
	if (stat(filePath, &result) != 0)
		cout << "ERROR::SHADER::CANT_READ_FILE_STATUS::" << filePath << endl;
	return result.st_mtime;
}

void Shader::bindGlobalUniforms()
{
	for (const auto &pair : globalUniforms)
		if (uniforms.count(pair.first) > 0)
			boundUniforms[pair.first] = pair.second;
}

void Shader::setBoundUniforms()
{
	for (const auto &pair : boundUniforms)
		setUniform(pair.first, pair.second);
}

void Shader::setUniform(const std::string & name, void * data)
{
	switch (uniforms[name].type)
	{
	case GL_BOOL:
		setUniform(name, *(bool *)data);
		break;
	case GL_INT:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_SAMPLER_CUBE:
		setUniform(name, *(int *)data);
		break;
	case GL_UNSIGNED_INT:
		setUniform(name, *(unsigned int *)data);
		break;
	case GL_FLOAT:
		setUniform(name, *(float *)data);
		break;
	case GL_FLOAT_VEC2:
		setUniform(name, *(glm::vec2 *)data);
		break;
	case GL_FLOAT_VEC3:
		setUniform(name, *(glm::vec3 *)data);
		break;
	case GL_FLOAT_VEC4:
		setUniform(name, *(glm::vec4 *)data);
		break;
	case GL_FLOAT_MAT2:
		setUniform(name, *(glm::mat2 *)data);
		break;
	case GL_FLOAT_MAT3:
		setUniform(name, *(glm::mat3 *)data);
		break;
	case GL_FLOAT_MAT4:
		setUniform(name, *(glm::mat4 *)data);
		break;
	default:
		std::cout << "ERROR::SHADER::UNIFORM::UNSUPPORTED_TYPE\n" <<
			vertexPath << "\n" <<
			fragmentPath << "\n" <<
			"Uniform name: " << name << "\n" <<
			"Unsupported type: " << std::hex << uniforms[name].type <<
		std::endl;
	}
}

void Shader::use()
{
	glUseProgram(ID);
	setBoundUniforms();
}

void Shader::setUniform(const std::string & name, bool value)
{
	glUniform1i(uniforms[name].location, (int)value);
}
void Shader::setUniform(const std::string & name, int value)
{
	glUniform1i(uniforms[name].location, value);
}
void Shader::setUniform(const std::string & name, unsigned int value)
{
	glUniform1i(uniforms[name].location, value);
}
void Shader::setUniform(const std::string & name, float value)
{
	glUniform1f(uniforms[name].location, value);
}
void Shader::setUniform(const std::string & name, const glm::vec2 & value)
{
	glUniform2fv(uniforms[name].location, 1, &value[0]);
}
void Shader::setUniform(const std::string & name, float x, float y)
{
	glUniform2f(uniforms[name].location, x, y);
}
void Shader::setUniform(const std::string & name, const glm::vec3 & value)
{
	glUniform3fv(uniforms[name].location, 1, &value[0]);
}
void Shader::setUniform(const std::string & name, float x, float y, float z)
{
	glUniform3f(uniforms[name].location, x, y, z);
}
void Shader::setUniform(const std::string & name, const glm::vec4 & value)
{
	glUniform4fv(uniforms[name].location, 1, &value[0]);
}
void Shader::setUniform(const std::string & name, float x, float y, float z, float w)
{
	glUniform4f(uniforms[name].location, x, y, z, w);
}
void Shader::setUniform(const std::string & name, const glm::mat2 & mat)
{
	glUniformMatrix2fv(uniforms[name].location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(const std::string & name, const glm::mat3 & mat)
{
	glUniformMatrix3fv(uniforms[name].location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(const std::string & name, const glm::mat4 & mat)
{
	glUniformMatrix4fv(uniforms[name].location, 1, GL_FALSE, &mat[0][0]);
}
