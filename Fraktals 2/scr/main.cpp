
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "..\res\imgui\imgui.h"
#include "..\res\imgui\imgui_impl_glfw.h"
#include "..\res\imgui\imgui_impl_opengl3.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Objekt.h"
#include "UI.h"
#include "Program.h"
#include "ProgrammManager.h"

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1500,800, "Fraktals 2", NULL, NULL);
	


	if (window == NULL)
	{
		std::cout << "ERROR 01: Window could not be created";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	
	UI ui(glsl_version,window);



	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glewInit();
	
	{
		

		
		
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));





		Renderer renderer;
		
		ProgrammManager manager(window,glsl_version,&renderer,&ui);
	
		manager.run();
		
		
		
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
