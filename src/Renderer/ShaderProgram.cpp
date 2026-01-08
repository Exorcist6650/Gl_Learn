#include "ShaderProgram.h"

#include <iostream>

namespace Renderer {

	ShaderProgram::ShaderProgram(const std::string vertexShaderCode, const std::string fragmentShaderCode)
	{
		// Shaders compiling
		GLuint vertexShaderID, fragmentShaderID;
		if (!createShader(vertexShaderCode, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "ERROR::SHADER::VERTEX::COMPILE_TIME_ERROR" << std::endl;
			return;
		}
		if (!createShader(fragmentShaderCode, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILE_TIME_ERROR" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		// Shader program generation
		program_ID = glCreateProgram();
		glAttachShader(program_ID, vertexShaderID);
		glAttachShader(program_ID, fragmentShaderID);
		glLinkProgram(program_ID);

		GLint sucsess;
		glGetProgramiv(program_ID, GL_LINK_STATUS, &sucsess);
		if (!sucsess)
		{
			char infoLog[1024];
			glGetProgramInfoLog(program_ID, 1024, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		else program_is_compiled = true;

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(program_ID);
	}

	void ShaderProgram::use() const 
	{
		glUseProgram(program_ID); 
	}

	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		// Creating and compiling shader
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint sucsess;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &sucsess);
		if (!sucsess)
		{
			char infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::COMPILE_TIME_ERROR\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}

}
