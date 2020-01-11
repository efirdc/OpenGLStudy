#include "ComputeShader.h"

using namespace std;

void ComputeShader::printSizes()
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

ComputeShader::ComputeShader(const char * path, std::vector<std::string> extraCode) :
	BaseShader(extraCode),
	path(path)
{
	// Retrieve the vertex/fragment source code from the filepaths
	string code = loadShaderCode(path);

	// Save the last modified times of the vertex and fragment shader
	modifiedTime = getModificationTime(path);

	// Make the shader program
	makeProgram(code.c_str(), ID);

	// Get shader uniforms
	getActiveUniforms();
	
	// Bind global uniforms
	bindGlobalUniforms();
}

bool ComputeShader::update()
{
	long long int newTime = getModificationTime(path);
	if (newTime == modifiedTime && !shouldUpdate)
		return false;
	shouldUpdate = false;
	
	string code = loadShaderCode(path);

	modifiedTime = getModificationTime(path);

	// Make the shader program
	unsigned int newProgramID;
	if (makeProgram(code.c_str(), newProgramID))
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
		cout << "Shader " << path << " recompiled." << endl;
		return true;
	}
	glDeleteProgram(newProgramID);
	return false;
}

bool ComputeShader::makeProgram(const char * code, unsigned int & newProgramID)
{
	unsigned int compute;
	int cSuccess, pSuccess;
	char infoLog[512];

	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &code, NULL);
	glCompileShader(compute);
	glGetShaderiv(compute, GL_COMPILE_STATUS, &cSuccess);
	if (!cSuccess)
	{
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED::" << path << "\n" << infoLog << std::endl;
	}

	newProgramID = glCreateProgram();
	glAttachShader(newProgramID, compute);
	glLinkProgram(newProgramID);
	glGetProgramiv(newProgramID, GL_LINK_STATUS, &pSuccess);
	if (!pSuccess)
	{
		glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED::" << path << "\n" << infoLog << endl;
	}

	glDeleteShader(compute);

	return cSuccess && pSuccess;
}