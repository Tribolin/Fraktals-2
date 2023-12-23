
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "..\res\imgui\imgui.h"
#include "..\res\imgui\imgui_impl_glfw.h"
#include "..\res\imgui\imgui_impl_opengl3.h"

#ifdef DEBUG
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))
#else
#define GLCall(x) xS
#endif 
#define ASSERT(x) if (!(x)) __debugbreak();


static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ("<<error<<"): "<<function<<" "<<file<<":"<<line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgrammSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgrammSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
	{
		std::cerr << "ERROR: didn't find shader file";
	}
	enum class ShaderType
	{
		NONE= -1, Vertex =0, Fragment = 1
	};

	std::string line;
	std::stringstream sstream[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream,line))
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

static unsigned int CompileShader(const std::string& source, unsigned int type)
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


static unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
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

int main()
{
	glfwInit();
	
	
	GLFWwindow* window = glfwCreateWindow(640, 480, "Version 1.0", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR 01: Window could not be created";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewInit();
	

	float positions[]{
		-0.5f,-0.5f,
		 0.5f, -0.5f,
		 0.5f,0.5f,
		 -0.5f, 0.5f,
	};

	unsigned int indecies[] = {
		0,1,2,
		2,3,0
	};

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 2* 4 * sizeof(float), positions, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,  6 * sizeof(unsigned int), indecies, GL_STATIC_DRAW));

	ShaderProgrammSource source = ParseShader("res/shaders/Basic.shader");
	
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader , "u_Color"));
	ASSERT(location != -1); 
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
	float r = 0.0f;
	float increment = 0.05f;
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		GLCall(glUniform4f(location,r, 0.3f, 0.0f, 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		
		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0.0f)
		{
			increment = 0.05f;
		}
		r += increment;
		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
	}
	glDeleteProgram(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
