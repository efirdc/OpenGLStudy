#include "BaseShader.h"

using namespace std;

std::vector<BaseShader *> BaseShader::allShaders;
std::unordered_map<std::string, void *> BaseShader::globalUniformBindings;
bool BaseShader::inactiveUniformWarnings = false;

BaseShader::BaseShader(std::vector<std::string> extraCode)
	: extraCode(extraCode)
{
	allShaders.push_back(this);
};


BaseShader::~BaseShader()
{
	allShaders.erase(find(allShaders.begin(), allShaders.end(), this));
	for (Uniform * uniform : uniformBindings)
		delete uniform;
	for (auto & pair : activeUniforms)
		delete pair.second;
	glDeleteProgram(ID);
}

void BaseShader::setExtraCode(std::vector<std::string> & newExtraCode)
{
	extraCode = newExtraCode;
	update();
}

std::vector<std::string> BaseShader::getExtraCode()
{
	return extraCode;
}

std::string BaseShader::loadShaderCode(std::string path)
{
	std::string code = Shadinclude::load(path);
	std::istringstream iss(code);
	std::string result;

	int i = 0;
	for (std::string line; std::getline(iss, line);)
	{
		result += line;
		if (i == 1)
			for (const std::string& extraLine : extraCode)
				result += extraLine + '\n';
		i++;
	}
}

void BaseShader::setGlobalUniform(const std::string & name, void * data)
{
	for (BaseShader * shader : allShaders)
		if (shader->hasActiveUniform(name) > 0)
		{
			shader->use();
			shader->setUniform(name, data);
		}
}

void BaseShader::bindGlobalUniform(const std::string & name, void * data)
{
	if (globalUniformBindings.count(name) > 0)
	{
		globalUniformBindings[name] = data;
		return;
	}
	globalUniformBindings[name] = data;
	for (BaseShader * shader : allShaders)
		if (shader->hasActiveUniform(name))
			shader->bindUniform(name, data);
}

void BaseShader::unbindGlobalUniform(const std::string & name)
{
	if (!globalUniformBindings.count(name))
	{
		cout << "ERROR::SHADER::UNIFORM::INVALID_GLOBAL_UNBIND\n" <<
			"Uniform name: " << name <<
			endl;
	}
	globalUniformBindings.erase(name);
	for (BaseShader * shader : allShaders)
		if (shader->getBoundUniform(name))
			shader->unbindUniform(name);
}

void BaseShader::getActiveUniforms()
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

long long int BaseShader::getModificationTime(const char * filePath)
{
	struct stat result;
	if (stat(filePath, &result) != 0)
		cout << "ERROR::SHADER::CANT_READ_FILE_STATUS::" << filePath << endl;
	return result.st_mtime;
}

void BaseShader::use()
{
	glUseProgram(ID);
	setBoundUniforms();
}

bool BaseShader::hasActiveUniform(const std::string & name)
{
	return activeUniforms.count(name) > 0;
}

BaseShader::Uniform * BaseShader::getBoundUniform(const std::string & name)
{
	for (auto it = uniformBindings.begin(); it != uniformBindings.end(); it++)
	{
		Uniform * uniform = *it;
		if (uniform->name == name)
			return uniform;
	}
	return nullptr;
}

void BaseShader::bindGlobalUniforms()
{
	for (auto& pair : globalUniformBindings)
		if (hasActiveUniform(pair.first) > 0 && !getBoundUniform(pair.first))
			bindUniform(pair.first, pair.second);
}

void BaseShader::setBoundUniforms()
{
	for (Uniform * uniform : uniformBindings)
		setUniform(uniform);
}

void BaseShader::bindUniform(const std::string & name, void * data)
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
			ID << "\n" <<
			"Uniform name: " << name <<
			endl;
	}
}

void BaseShader::unbindUniform(const std::string & name)
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
		ID << "\n" <<
		"Uniform name: " << name <<
		endl;
}

void BaseShader::setUniform(const std::string & name, void * data)
{
	if (setUniformErrorCheck(name)) { return; }
	Uniform * activeUniform = activeUniforms[name];
	setUniform(activeUniform->location, activeUniform->type, data);
}

void BaseShader::setUniform(Uniform * uniform)
{
	setUniform(uniform->location, uniform->type, uniform->data);
}

void BaseShader::setUniform(int location, GLenum type, void * data)
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
	case GL_IMAGE_1D:
	case GL_IMAGE_2D:
	case GL_IMAGE_3D:
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
	case GL_INT_VEC2:
		setUniform(location, *(glm::ivec2 *)data);
		break;
	case GL_INT_VEC3:
		setUniform(location, *(glm::ivec3 *)data);
		break;
	case GL_INT_VEC4:
		setUniform(location, *(glm::ivec4 *)data);
		break;
	default:
		cout << "ERROR::SHADER::UNIFORM::UNSUPPORTED_TYPE\n" <<
			ID << "\n" <<
			"Unsupported type: " << hex << type <<
			endl;
	}
}

bool BaseShader::setUniformErrorCheck(const std::string & name)
{
	if (hasActiveUniform(name))
	{
		return false;
	}
	else
	{
		if (inactiveUniformWarnings)
		{
			cout << "ERROR::SHADER::UNIFORM::INACTIVE_SET_UNIFORM\n" <<
				ID << "\n" <<
				"Uniform name: " << name <<
				endl;
		}
		return true;
	}
}

void BaseShader::setUniform(int location, bool value) {
	glUniform1i(location, (int)value);
}
void BaseShader::setUniform(int location, int value) {
	glUniform1i(location, value);
}
void BaseShader::setUniform(int location, unsigned int value) {
	glUniform1i(location, value);
}
void BaseShader::setUniform(int location, float value) {
	glUniform1f(location, value);
}
void BaseShader::setUniform(int location, const glm::vec2 & value) {
	glUniform2fv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, float x, float y) {
	glUniform2f(location, x, y);
}
void BaseShader::setUniform(int location, const glm::vec3 & value) {
	glUniform3fv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}
void BaseShader::setUniform(int location, const glm::vec4 & value) {
	glUniform4fv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, float x, float y, float z, float w) {
	glUniform4f(location, x, y, z, w);
}
void BaseShader::setUniform(int location, const glm::mat2 & mat) {
	glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}
void BaseShader::setUniform(int location, const glm::mat3 & mat) {
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}
void BaseShader::setUniform(int location, const glm::mat4 & mat) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
void BaseShader::setUniform(int location, const glm::ivec2& value) {
	glUniform2iv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, int x, int y) {
	glUniform2i(location, x, y);
}
void BaseShader::setUniform(int location, const glm::ivec3& value) {
	glUniform3iv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, int x, int y, int z) {
	glUniform3i(location, x, y, z);
}
void BaseShader::setUniform(int location, const glm::ivec4& value) {
	glUniform4iv(location, 1, &value[0]);
}
void BaseShader::setUniform(int location, int x, int y, int z, int w) {
	glUniform4i(location, x, y, z, w);
}