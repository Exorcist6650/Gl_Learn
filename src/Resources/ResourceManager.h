#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
	class ShaderProgram;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	std::shared_ptr <Renderer::ShaderProgram> loadShaders(const std::string shaderProgramName, 
		const std::string& vertexPath, const std::string& fragmentPath);

	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderProgramName);

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
private:
	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map <std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap; // Map type define
	ShaderProgramsMap shader_programs; // Map of shader programs
	std::string b_path;
};
