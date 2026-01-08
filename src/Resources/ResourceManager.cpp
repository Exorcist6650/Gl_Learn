#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	b_path = executablePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string shaderProgramName, 
	const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexPath.empty()) {
		std::cerr << "ERROR::SHADER::VERTEX::FILE_IS_EMPTY" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentPath.empty()) {
		std::cerr << "ERROR::SHADER::FRAGMENT::FILE_IS_EMPTY" << std::endl;
		return nullptr;
	}

	// Append shader program into map
	std::shared_ptr<Renderer::ShaderProgram>& newShader = shader_programs.emplace(shaderProgramName,
		std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (newShader->isCompiled()) return newShader; // Shader ruturn (sucsessfully)

	// Error output
	std::cerr << "ERROR::SHADER::PROGRAM::LOAD_FAILED\n" 
		<< "Vertex path: " << vertexPath << "\nFragmentPath: " << fragmentPath << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderProgramName)
{
	auto it = shader_programs.find(shaderProgramName);
	if (it != shader_programs.end()) return it->second;

	// Error output
	std::cout << "ERROR::RESOURSE_MANAGER::GET_SHADER_PROGRAM" << std::endl;
	return nullptr;
}


std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
	// File open and stream into string
	std::ifstream shaderFile;
	shaderFile.open(b_path + "/" + relativeFilePath.c_str(), std::ios::in, std::ios::binary);
	if (!shaderFile.is_open())
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_OPEN\n" << relativeFilePath << std::endl;
		return std::string();
	}

	std::stringstream buffer; // String stream
	buffer << shaderFile.rdbuf();
	return buffer.str();
}
