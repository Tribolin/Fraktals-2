#pragma once
#include <string>
#include <unordered_map>

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
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4);
private:
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
	ShaderProgrammSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};
