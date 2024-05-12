
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

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Version 1.0", NULL, NULL);


	if (window == NULL)
	{
		std::cout << "ERROR 01: Window could not be created";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	////io.ConfigViewportsNoAutoMerge = true;
	////io.ConfigViewportsNoTaskBarIcon = true;
	//

	//ImGui::StyleColorsDark();
	//io.Fonts->AddFontFromFileTTF("res/Fonts/Roboto-Regular.ttf", 15.0f);

	//

	//ImGuiStyle& style = ImGui::GetStyle();
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	style.WindowRounding = 0.0f;
	//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//}

	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);
	UI ui(glsl_version,window);



	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glewInit();
	
	{
		Objekt obj;

		/*float positions[]{
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		unsigned int indecies[] = {
			0,1,2,
			2,3,0
		};*/
		
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		/*VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;

		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		

		IndexBuffer ib(indecies, 6);*/

		Shader shader("res/shaders/Mandelbrot.shader");
		shader.Bind();
		
		//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/cherno.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture",0);

		/*va.Unbind();
		vb.Unbind();
		ib.Unbind();*/
		obj.Unbind();
		shader.Unbind();

		Renderer renderer;

		float zoom = 0.1f;
		float positionOffset[2]{0.f,0.f};

		
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			static float zoomSpeed = 0.0f;
			static float offsetChangeX = 0.0f;
			static float offeetChangeY = 0.0f;
			// Start the Dear ImGui frame
			ui.NewFrame();
			

			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			ui.Mandelbrot(zoomSpeed, offsetChangeX,offeetChangeY);

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.3f, 0.0f, 1.0f);
			shader.SetUniform1f("zoom", zoom);
			shader.SetUniform2f("positionOffset", positionOffset[0],positionOffset[1]);
			obj.Bind();
			/*va.Bind();
			ib.Bind();*/

			

			renderer.Draw(obj.va,obj.ib, shader);
			//renderer.Draw(va,ib, shader);
			
			ImGui::Render();

			ui.Render();
			
			zoom += zoomSpeed/100.f*zoom;
			positionOffset[0] += offsetChangeX/100.0f/zoom;
			positionOffset[1] += offeetChangeY/100.0f/zoom;
			offsetChangeX = 0;
			offeetChangeY = 0;

			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
		
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
