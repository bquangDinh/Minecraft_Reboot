#include "Shader.h"

Shader::Shader(const char* shaderFile, GLenum shaderType) {
	std::string shaderCodeStr = FileUtils::readFile(shaderFile);

	const char* shaderCode = shaderCodeStr.c_str();

	// Compile shader
	ID = glCreateShader(shaderType);
	glShaderSource(ID, 1, &shaderCode, NULL);
	glCompileShader(ID);

	// Check if compiling succeed
	int success;
	const int LOG_BUFFER = 512;
	char infoLog[LOG_BUFFER];

	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(ID, LOG_BUFFER, NULL, infoLog);

		cout << "ERROR::SHADER::COMPILING::" << infoLog << endl;
	}
	else {
		cout << "SHADER::COMPILING::SUCCESS" << endl;
	}
}

Shader::Shader() {
	ID = 0;
}

Shader::~Shader() {
	glDeleteShader(ID);
}