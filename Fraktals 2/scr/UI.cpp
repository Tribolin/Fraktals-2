#include "UI.h"
#include <GLFW/glfw3.h>

UI::UI(const char* glsl_version, GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("res/Fonts/Roboto-Regular.ttf", 15.0f);



	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

UI::~UI()
{
}

void UI::Render()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UI::Mandelbrot(float& zoomSpeed, float& offsetChangeX, float& offsetChangeY)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{
				
				static int counter = 0;

				ImGui::Begin("Mandelbrot");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				//ImGui::Checkbox("UI Test Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("cpu", &show_another_window);

				ImGui::SliderFloat("zoom speed", &zoomSpeed, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat("offsetX", &offsetChangeX, -1.0f, 1.0f);
				ImGui::SliderFloat("offsetY", &offsetChangeY, -1.0f, 1.0f);
				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				/*if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);
				*/
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			} 
}
