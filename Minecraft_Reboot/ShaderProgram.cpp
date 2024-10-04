#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const GLuint vextexShader, const GLuint fragmentShader) {
	// Attaching and Linking
	ID = glCreateProgram();

	glAttachShader(ID, vextexShader);
	glAttachShader(ID, fragmentShader);

	// Link shaders together
	glLinkProgram(ID);

	checkProgramLinkSuccess(vextexShader, fragmentShader);
}

ShaderProgram::ShaderProgram(const Shader vextexShader, const Shader fragmentShader) {
	// Attaching and Linking
	ID = glCreateProgram();

	glAttachShader(ID, vextexShader.ID);
	glAttachShader(ID, fragmentShader.ID);

	// Link shaders together
	glLinkProgram(ID);

	checkProgramLinkSuccess(vextexShader.ID, fragmentShader.ID);
}

ShaderProgram::ShaderProgram(const GLuint shaderProgram) {
	ID = shaderProgram;
}

ShaderProgram::ShaderProgram(const char* vextexFile, const char* fragmentFile) {
	// Create shader from file source
	Shader vextexShader(vextexFile, GL_VERTEX_SHADER);
	Shader fragmentShader(fragmentFile, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();

	glAttachShader(ID, vextexShader.ID);
	glAttachShader(ID, fragmentShader.ID);

	glLinkProgram(ID);

	checkProgramLinkSuccess(vextexShader.ID, fragmentShader.ID);
}

ShaderProgram::ShaderProgram() {
	ID = 0;
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(ID);
}

void ShaderProgram::Use() {
	glUseProgram(ID);
}

void ShaderProgram::Destroy() {
	glDeleteProgram(ID);
}

void ShaderProgram::SetMatrix4(const char* name, const glm::mat4 matrix, const bool transpose) {
	// Get the location of the attribute
	GLuint location = glGetUniformLocation(ID, name);

	// Set value
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matrix));
}

void ShaderProgram::SetVector2(const char* name, const glm::vec2 vec2) {
	// Get the location of the attribute
	GLuint location = glGetUniformLocation(ID, name);

	// Set value
	glUniform2f(location, vec2.x, vec2.y);
}

void ShaderProgram::SetVector3(const char* name, const glm::vec3 vec3) {
	// Get the location of the attribute
	GLuint location = glGetUniformLocation(ID, name);

	// Set value
	glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::SetFloat(const char* name, const float value) {
	// Get the location of the attribute
	GLuint location = glGetUniformLocation(ID, name);

	// Set value
	glUniform1f(location, value);
}

void ShaderProgram::checkProgramLinkSuccess(const GLuint vextexShader, const GLuint fragmentShader) {
	//check program linking
	int success = 0;
	const int LOG_BUFFER = 512;
	char infoLog[LOG_BUFFER];

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (success) {
		cout << "PROGRAM::LINKING:SUCCESS" << endl;

		// Since the program has now contained the two shaders
		// It is safe to delete these two shaders to free up memory
		glDeleteShader(vextexShader);
		glDeleteShader(fragmentShader);
	}
	else {
		// Show Log
		glGetProgramInfoLog(ID, LOG_BUFFER, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINKING::" << infoLog << endl;
	}
}