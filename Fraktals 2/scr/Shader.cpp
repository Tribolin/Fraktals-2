#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{

	ShaderProgrammSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

	
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}




ShaderProgrammSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
	{
		std::cerr << "ERROR: didn't find shader file";
	}
	enum class ShaderType
	{
		NONE = -1, Vertex = 0, Fragment = 1
	};

	std::string line;
	std::stringstream sstream[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::Fragment;
		}
		else
		{
			sstream[(int)type] << line << '\n';
		}
	}
	return{ sstream[0].str(),sstream[1].str() };
}

 unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile Shader!";
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}



void Shader::SetUniform1i(const std::string& name, int v0) {
	GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}




int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLoctionCache.find(name) != m_UniformLoctionCache.end())
		return m_UniformLoctionCache[name];
	
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warnig: uniform " << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLoctionCache[name] = location;
	
	return location;
}


ComputeShader::ComputeShader(const std::filesystem::path& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << path.string() << std::endl;
		m_RendererID = -1;
	}

	std::ostringstream contentStream;
	contentStream << file.rdbuf();
	std::string shaderSource = contentStream.str();

	GLuint shaderHandle = glCreateShader(GL_COMPUTE_SHADER);

	const GLchar* source = (const GLchar*)shaderSource.c_str();
	glShaderSource(shaderHandle, 1, &source, 0);

	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);

		std::cerr << infoLog.data() << std::endl;

		glDeleteShader(shaderHandle);
		m_RendererID = -1;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderHandle);
	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		std::cerr << infoLog.data() << std::endl;

		glDeleteProgram(program);
		glDeleteShader(shaderHandle);

		m_RendererID = -1;
	}

	glDetachShader(program, shaderHandle);
	m_RendererID = program;
}

ComputeShader::~ComputeShader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void ComputeShader::Bind()
{
	GLCall(glUseProgram(m_RendererID));
}

void ComputeShader::Unbind()
{
	GLCall(glUseProgram(0));
}

void ComputeShader::Dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ)
{
	GLCall(glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ));
	
}

void ComputeShader::WaitForCompletion()
{
	GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
}
