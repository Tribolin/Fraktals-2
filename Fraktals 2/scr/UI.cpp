#include "UI.h"


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
	ImGui::Render();

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

void UI::Manager(int& currentProgramIndex)
{

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	{
		ImGui::Begin("Program Selection");

		const char* items[] = { "Mandelbrot", "Koch Snowflake", "Ray_marching_test", "Julia-4d","fluid-sim","Wormhole-sim","Orbital","Double-Slit","Lenia"};
		static const char* current_item = NULL;

		if (ImGui::BeginCombo("Program", items[currentProgramIndex]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]); 
				if (ImGui::Selectable(items[n], is_selected))
					currentProgramIndex = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();  
			}
			ImGui::EndCombo();
			
		}
		ImGui::End();
	}
}

void UI::DemoWindow(bool& show)
{
	ImGui::ShowDemoWindow(&show);
}

void UI::Mandelbrot(float& zoomSpeed, float& offsetChangeX, float& offsetChangeY, int& iter, bool& AntiAli, int& anti_aliasing_iterations)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{
				
				static int counter = 0;

				ImGui::Begin("Mandelbrot");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is the Mandelbrot set. It is one of the most famous Fraktals.\n Use the controlles below to explore it.");               // Display some text (you can use a format strings too)
				//ImGui::Checkbox("UI Test Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("cpu", &show_another_window);

				ImGui::SliderFloat("zoom speed", &zoomSpeed, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat("offsetX", &offsetChangeX, -1.0f, 1.0f);
				ImGui::SliderFloat("offsetY", &offsetChangeY, -1.0f, 1.0f);

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Text("configure Performance");
				ImGui::SliderInt("Iterations",&iter,0.0f,2000.0f);
				ImGui::Checkbox("anti aliasing",&AntiAli);
				if (AntiAli)
				{
					ImGui::SliderInt("Strenght", &anti_aliasing_iterations,0,20);
				}
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

void UI::Julia4D(float& c1, float& c2, float& c3, float& c4, float& rotationSpeed, float& UpChange, float& cut)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{

		static int counter = 0;

		ImGui::Begin("Julia 4D");           
		ImGui::SliderFloat("C1",&c1,-2,2);
		ImGui::SliderFloat("C2", &c2, -2, 2);
		ImGui::SliderFloat("C3", &c3, -2, 2);
		ImGui::SliderFloat("C4", &c4, -2, 2);

		ImGui::Value("Rotation Speed", rotationSpeed);
		ImGui::SliderFloat("Speed", &rotationSpeed, -1, 1);
		
		ImGui::SliderFloat("Up", &UpChange, -1, 1);

		ImGui::SliderFloat("Cut", &cut, -1, 1);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}
}

void UI::Wormhole(float& a, float& M,float& zoom,float&angle,float&x_Offset,float& l)
{
	ImGui::Begin("Wormhole");

	ImGui::SliderFloat("a", &a,0,10);
	ImGui::SliderFloat("M", &M, 0, 1);
	ImGui::SliderFloat("Camera Zoom", &zoom, 0.1, 10);
	ImGui::SliderFloat("angle", &angle, -2, 2);
	ImGui::SliderFloat("x_Offset", &x_Offset, -3, 3);
	ImGui::SliderFloat("l", &l, -10, 5);
	ImGui::End();
}

void UI::Orbit(float& R, float& psi, float& theta, int& n, int& l, int& m, float& Brightness,int&real, bool& Draw3D)
{
	float pi = 3.14159265359;
	static bool r = true;
	ImGui::Begin("Orbital");

	ImGui::SliderFloat("Camera Distance", &R, 0, 70);
	ImGui::SliderFloat("Psi", &psi, -pi, pi);
	ImGui::SliderFloat("theta", &theta, 0.001, pi);
	ImGui::SliderInt("N",&n, 1, 8);
	ImGui::SliderInt("l", &l, 0, (n-1));
	ImGui::SliderInt("m", &m, -l, l);
	ImGui::SliderFloat("Brightness", &Brightness, 0.5,20);
	ImGui::Checkbox("real",&r);
	real = r;
	ImGui::Checkbox("Draw3D", &Draw3D);
	ImGui::End();
}
