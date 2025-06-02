#pragma once

#include "core.h"
#include <string>

class Shader {
public:

	Shader(const char* vertexPath, const char* fragmentPath);
	
	std::string loadShader(const std::string& filePath);

	~Shader();

	void begin(); 

	void end(); 

	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setFloatArray(const std::string& name, float* value, int count);

	void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, const float* values);
	void setVector3(const std::string& name, const glm::vec3 value);

	void setMatrix4x4(const std::string& name, glm::mat4 value);
	void setMatrix4x4Array(const std::string& name, glm::mat4* value, int count);
	void setMatrix3x3(const std::string& name, glm::mat3 value);

private:

	void checkShaderErrors(GLuint target, std::string type);

private:
	GLuint mProgram{ 0 };
};