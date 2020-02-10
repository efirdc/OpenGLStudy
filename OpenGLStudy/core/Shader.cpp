#include "Shader.h"

using namespace std;

std::vector<Shader *> Shader::allShaders;
std::unordered_map<std::string, void *> Shader::globalUniformBindings;
std::map<std::string, std::string> Shader::globalDefines;
bool Shader::inactiveUniformWarnings = false;

Shader::Shader(std::vector<Stage> stages) :
	stages(stages)
{
	allShaders.push_back(this);

	createProgram();
};

Shader::~Shader()
{
	allShaders.erase(find(allShaders.begin(), allShaders.end(), this));
	for (Uniform * uniform : uniformBindings)
		delete uniform;
	for (auto & pair : activeUniforms)
		delete pair.second;
	glDeleteProgram(ID);
}

void Shader::setExtraCode(std::vector<std::string> & newExtraCode)
{
	shouldUpdate = true;
	extraCode = newExtraCode;
}

std::vector<std::string> Shader::getExtraCode()
{
	return extraCode;
}

void Shader::setDefinition(const std::string& name, const std::string& definition)
{
	shouldUpdate = true;
	defines[name] = definition;
}

void Shader::loadShaderCode(Stage& stage)
{
	std::string code = Shadinclude::load(stage.path);

	// Replace #define's
	auto definesCopy = defines;
	for (auto const & x : globalDefines)
		definesCopy[x.first] = x.second;
	std::vector<std::string> replacedDefines;
	for (auto const & x : definesCopy)
	{
		std::string reName;
		for (auto c : x.first)
		{
			if (c == '(' || c == ')')
				reName += '\\';
			reName += c;
		}
		std::regex re("#define " + reName + ".+");
		if (std::regex_search(code, re))
		{
			code = std::regex_replace(code, re, "#define " + x.first + " " + x.second);
			replacedDefines.push_back(x.first);
		}
	}
	for (const std::string define : replacedDefines)
		definesCopy.erase(define);
	
	std::istringstream iss(code);
	stage.code = "";

	int i = 0;
	for (std::string line; std::getline(iss, line);)
	{
		stage.code += line + "\n";
		if (i == 1)
		{
			for (auto const& x : definesCopy)
				stage.code += "#define " + x.first + " " + x.second + "\n";
			for (const std::string& extraLine : extraCode)
				stage.code += extraLine + '\n';
		}
		i++;
	}
}

bool Shader::createProgram()
{
	for (Stage& stage : stages)
	{
		loadShaderCode(stage);
		compile(stage);
	}

	for (Stage& stage : stages)
		if (!stage.compiled)
		{
			deleteAllShaderStages();
			return false;
		}
			
	unsigned int newID = glCreateProgram();
	for (Stage& stage : stages)
		glAttachShader(newID, stage.ID);
	glLinkProgram(newID);
	
	char infoLog[512];
	int success;
	glGetProgramiv(newID, GL_LINK_STATUS, &success);
	if (!success)
	{
		compiled = false;
		glGetProgramInfoLog(newID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED::"
		<< stages[0].path << "\n" << infoLog << std::endl;
		glDeleteProgram(newID);
		deleteAllShaderStages();
		return false;
	}
	
	if (compiled)
	{
		glDeleteProgram(ID);
		for (Uniform* uniform : uniformBindings)
			delete uniform;
		uniformBindings.clear();
		for (auto& pair : activeUniforms)
			delete pair.second;
		activeUniforms.clear();
		cout << "Shader " << stages[0].path << " recompiled." << endl;
	}
	
	deleteAllShaderStages();
	compiled = true;
	ID = newID;
	getActiveUniforms();
	bindGlobalUniforms();
	return true;
}

void Shader::use()
{
	glUseProgram(ID);
	setBoundUniforms();
}

bool Shader::update()
{
	bool somethingModified = false;
	for (Stage& stage : stages)
	{
		if (isModified(stage))
		{
			somethingModified = true;
			break;
		}
	}

	if (!somethingModified && !shouldUpdate)
		return false;
	shouldUpdate = false;

	return createProgram();
}

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

void Shader::setGlobalDefinition(const std::string& name, const std::string& definition)
{
	for (Shader* shader : allShaders)
		shader->shouldUpdate = true;
	globalDefines[name] = definition;
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
			ID << "\n" <<
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
		ID << "\n" <<
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
	case GL_INT:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_IMAGE_1D:
	case GL_IMAGE_2D:
	case GL_IMAGE_3D:
	case GL_SAMPLER_CUBE:
		setUniform(location, *(int*)data);
		/*
		cout << "ERROR::SHADER::UNIFORM::UNSUPPORTED_TYPE\n" <<
			ID << "\n" <<
			"Unsupported type: " << hex << type <<
			endl;*/
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
void Shader::setUniform(int location, const glm::ivec2& value) {
	glUniform2iv(location, 1, &value[0]);
}
void Shader::setUniform(int location, int x, int y) {
	glUniform2i(location, x, y);
}
void Shader::setUniform(int location, const glm::ivec3& value) {
	glUniform3iv(location, 1, &value[0]);
}
void Shader::setUniform(int location, int x, int y, int z) {
	glUniform3i(location, x, y, z);
}
void Shader::setUniform(int location, const glm::ivec4& value) {
	glUniform4iv(location, 1, &value[0]);
}
void Shader::setUniform(int location, int x, int y, int z, int w) {
	glUniform4i(location, x, y, z, w);
}

void Shader::printComputeShaderInfo()
{
	int workGroupCounts[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCounts[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCounts[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCounts[2]);
	printf("max global (total) work group size x:%i y:%i z:%i\n",
		workGroupCounts[0], workGroupCounts[1], workGroupCounts[2]);
	int workGroupSize[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);
	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		workGroupSize[0], workGroupSize[1], workGroupSize[2]);
	int workGroupInvocations;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workGroupInvocations);
	printf("max local work group invocations %i\n", workGroupInvocations);
}