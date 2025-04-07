#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>

struct ShaderProgrammSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLoctionCache;
public:
	Shader(const std::string& filepath);

	~Shader();
	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform1i(const std::string& name, int v0);

	void SetUniform1f(const std::string& name, float v0);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4);
private:
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
	ShaderProgrammSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};
class ComputeShader
{
public:
	ComputeShader(const std::filesystem::path& path);
	~ComputeShader();
	void Bind();
	void Unbind();
	void Dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ);
	void WaitForCompletion();

private:
	unsigned int m_RendererID;
};

