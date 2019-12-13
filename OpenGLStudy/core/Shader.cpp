/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#include "Shader.h"

using namespace std;

std::vector<Shader *> Shader::allShaders;
std::unordered_map<std::string, void *> Shader::globalUniformBindings;

void Shader::setGlobalUniform(const std::string & name, void * data)
{
	for (Shader * shader : allShaders)
		if (shader->hasActiveUniform(name) > 0)
		{
			shader->use();
			shader->setUniform(name, data);
		}
}

void Shader::bindGlobalUniform(const std::string & name, void * data)
{
	if (globalUniformBindings.count(name) > 0)
	{
		globalUniformBindings[name] = data;
		return;
	}
	globalUniformBindings[name] = data;
	for (Shader * shader : allShaders)
		if (shader->hasActiveUniform(name))
			shader->bindUniform(name, data);
}

void Shader::unbindGlobalUniform(const std::string & name)
{
	if (!globalUniformBindings.count(name))
	{
		cout << "ERROR::SHADER::UNIFORM::INVALID_GLOBAL_UNBIND\n" <<
			"Uniform name: " << name <<
		endl;
	}
	globalUniformBindings.erase(name);
	for (Shader * shader : allShaders)
		if (shader->getBoundUniform(name))
			shader->unbindUniform(name);
}

Shader::Shader(const char * vertexPath, const char * fragmentPath) :
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode = Shadinclude::load(vertexPath);
	string fragmentCode = Shadinclude::load(fragmentPath);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);
	
	// Make the shader program
	makeProgram(vertexCode.c_str() , fragmentCode.c_str(), ID);

	// Get shader uniforms
	getActiveUniforms();

	// Add to vector of all shaders
	allShaders.push_back(this);

	// Bind global uniforms
	bindGlobalUniforms();
}

Shader::~Shader()
{
	allShaders.erase(find(allShaders.begin(), allShaders.end(), this));
	for (Uniform * uniform : uniformBindings)
		delete uniform;
	for (auto & pair : activeUniforms)
		delete pair.second;
	glDeleteProgram(ID);
}

bool Shader::update()
{
	// Exit function if vertex and fragment shaders have not been modified
	long long int newVTime = getModificationTime(vertexPath);
	long long int newFTime = getModificationTime(fragmentPath);
	if (newVTime == vertexModifiedTime && newFTime == fragmentModifiedTime)
		return false;
	
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode = Shadinclude::load(vertexPath);
	string fragmentCode = Shadinclude::load(fragmentPath);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);

	// Make the shader program
	unsigned int newProgramID;
	if (makeProgram(vertexCode.c_str(), fragmentCode.c_str(), newProgramID))
	{
		glDeleteProgram(ID);
		ID = newProgramID;
		for (Uniform * uniform : uniformBindings)
			delete uniform;
		uniformBindings.clear();
		for (auto & pair : activeUniforms)
			delete pair.second;
		activeUniforms.clear();
		getActiveUniforms();
		bindGlobalUniforms();
		cout << "Shader " << vertexPath << " " << fragmentPath << " recompiled." << endl;
		return true;
	}
	glDeleteProgram(newProgramID);
	return false;
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

void Shader::getActiveUniforms()
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
		Uniform * uniform = new Uniform;
		uniform->location = glGetUniformLocation(ID, name);
		uniform->name = std::string(name);
		uniform->size = size;
		uniform->type = type;
		activeUniforms[uniform->name] = uniform;
	}
}

long long int Shader::getModificationTime(const char * filePath)
{
	struct stat result;
	if (stat(filePath, &result) != 0)
		cout << "ERROR::SHADER::CANT_READ_FILE_STATUS::" << filePath << endl;
	return result.st_mtime;
}

void Shader::use()
{
	glUseProgram(ID);
	setBoundUniforms();
}

bool Shader::hasActiveUniform(const std::string & name)
{
	return activeUniforms.count(name) > 0;
}

Shader::Uniform * Shader::getBoundUniform(const std::string & name)
{
	for (auto it = uniformBindings.begin(); it != uniformBindings.end(); it++)
	{
		Uniform * uniform = *it;
		if (uniform->name == name)
			return uniform;
	}
	return nullptr;
}

void Shader::bindGlobalUniforms()
{
	for (auto& pair : globalUniformBindings)
		if (hasActiveUniform(pair.first) > 0 && !getBoundUniform(pair.first))
			bindUniform(pair.first, pair.second);
}

void Shader::setBoundUniforms()
{
	for (Uniform * uniform : uniformBindings)
		setUniform(uniform);
}

void Shader::bindUniform(const std::string & name, void * data)
{
	// 3 cases: Already bound, not bound and active, not bound and not active (error)
	Uniform * alreadyBoundUniform = getBoundUniform(name);
	if (alreadyBoundUniform)
	{
		alreadyBoundUniform->data = data;
	}
	else if (hasActiveUniform(name))
	{
		Uniform * newBoundUniform = new Uniform;
		Uniform * activeUniform = activeUniforms[name];
		*newBoundUniform = *activeUniform;
		newBoundUniform->data = data;
		uniformBindings.push_back(newBoundUniform);
	}
	else
	{
		cout << "ERROR::SHADER::UNIFORM::INVALID_BIND\n" << 
			vertexPath << "\n" <<
			fragmentPath << "\n" <<
			"Uniform name: " << name <<
		endl;
	}
}

void Shader::unbindUniform(const std::string & name)
{
	for (auto it = uniformBindings.begin(); it != uniformBindings.end(); it++)
	{
		Uniform * uniform = *it;
		if (uniform->name == name)
		{
			uniformBindings.erase(it);
			delete uniform;
			return;
		}
	}
	cout << "ERROR::SHADER::UNIFORM::INVALID_UNBIND\n" <<
		vertexPath << "\n" <<
		fragmentPath << "\n" <<
		"Uniform name: " << name <<
	endl;
}

void Shader::setUniform(const std::string & name, void * data) 
{
	if (setUniformErrorCheck(name)) { return; }
	Uniform * activeUniform = activeUniforms[name];
	setUniform(activeUniform->location, activeUniform->type, data);
}

void Shader::setUniform(Uniform * uniform) 
{
	setUniform(uniform->location, uniform->type, uniform->data);
}

void Shader::setUniform(int location, GLenum type, void * data)
{
	switch (type)
	{
	case GL_BOOL:
		setUniform(location, *(bool *)data);
		break;
	case GL_INT:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_SAMPLER_CUBE:
		setUniform(location, *(int *)data);
		break;
	case GL_UNSIGNED_INT:
		setUniform(location, *(unsigned int *)data);
		break;
	case GL_FLOAT:
		setUniform(location, *(float *)data);
		break;
	case GL_FLOAT_VEC2:
		setUniform(location, *(glm::vec2 *)data);
		break;
	case GL_FLOAT_VEC3:
		setUniform(location, *(glm::vec3 *)data);
		break;
	case GL_FLOAT_VEC4:
		setUniform(location, *(glm::vec4 *)data);
		break;
	case GL_FLOAT_MAT2:
		setUniform(location, *(glm::mat2 *)data);
		break;
	case GL_FLOAT_MAT3:
		setUniform(location, *(glm::mat3 *)data);
		break;
	case GL_FLOAT_MAT4:
		setUniform(location, *(glm::mat4 *)data);
		break;
	default:
		cout << "ERROR::SHADER::UNIFORM::UNSUPPORTED_TYPE\n" <<
			vertexPath << "\n" <<
			fragmentPath << "\n" <<
			"Unsupported type: " << hex << type <<
		endl;
	}
}

bool Shader::setUniformErrorCheck(const std::string & name)
{
	if (hasActiveUniform(name))
	{
		return false;
	}
	else
	{
		cout << "ERROR::SHADER::UNIFORM::INACTIVE_SET_UNIFORM\n" <<
			vertexPath << "\n" <<
			fragmentPath << "\n" <<
			"Uniform name: " << name <<
		endl;
		return true;
	}
}

void Shader::setUniform(const std::string & name, bool value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, int value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, unsigned int value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, float value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, const glm::vec2 & value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, float x, float y) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, x, y);
}
void Shader::setUniform(const std::string & name, const glm::vec3 & value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, float x, float y, float z) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, x, y, z);
}
void Shader::setUniform(const std::string & name, const glm::vec4 & value) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, value);
}
void Shader::setUniform(const std::string & name, float x, float y, float z, float w) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, x, y, z, w);
}
void Shader::setUniform(const std::string & name, const glm::mat2 & mat) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, mat);
}
void Shader::setUniform(const std::string & name, const glm::mat3 & mat) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, mat);
}
void Shader::setUniform(const std::string & name, const glm::mat4 & mat) {
	if (setUniformErrorCheck(name)) { return; }
	setUniform(activeUniforms[name]->location, mat);
}

void Shader::setUniform(int location, bool value) {
	glUniform1i(location, (int)value);
}
void Shader::setUniform(int location, int value) {
	glUniform1i(location, value);
}
void Shader::setUniform(int location, unsigned int value) {
	glUniform1i(location, value);
}
void Shader::setUniform(int location, float value) {
	glUniform1f(location, value);
}
void Shader::setUniform(int location, const glm::vec2 & value) {
	glUniform2fv(location, 1, &value[0]);
}
void Shader::setUniform(int location, float x, float y) {
	glUniform2f(location, x, y);
}
void Shader::setUniform(int location, const glm::vec3 & value) {
	glUniform3fv(location, 1, &value[0]);
}
void Shader::setUniform(int location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}
void Shader::setUniform(int location, const glm::vec4 & value) {
	glUniform4fv(location, 1, &value[0]);
}
void Shader::setUniform(int location, float x, float y, float z, float w) {
	glUniform4f(location, x, y, z, w);
}
void Shader::setUniform(int location, const glm::mat2 & mat) {
	glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(int location, const glm::mat3 & mat) {
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(int location, const glm::mat4 & mat) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}