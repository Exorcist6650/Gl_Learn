#pragma once

#include <glad/glad.h>

#include <string>

namespace Renderer {
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string vertexShaderCode, const std::string fragmentShaderCode);
		~ShaderProgram();
		bool isCompiled() const { return program_is_compiled; }
		void use() const;
		GLuint getProgramID() const { return program_ID; }

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram &operator=(const ShaderProgram&) = delete;
	private:
		bool program_is_compiled = false;
		GLuint program_ID;
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	};
}