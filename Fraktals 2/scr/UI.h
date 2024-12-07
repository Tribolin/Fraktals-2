#pragma once
#include <GLFW/glfw3.h>

#include "..\res\imgui\imgui.h"
#include "..\res\imgui\imgui_impl_glfw.h"
#include "..\res\imgui\imgui_impl_opengl3.h"



class UI
{
public:
	UI(const char* glsl_version, GLFWwindow* window);
	~UI();

	void Render();
	void NewFrame();
	//UIs
	void Manager(int& currentProgrammIndex);
	void DemoWindow(bool& show);
	void Mandelbrot(float& zoomSpeed, float& offsetChangeX, float& offsetChangeY,int& iter, bool& AntiAli, int& anti_aliasing_iterations);
	void Julia4D(float& C, float& rotationSpeed);

private:
	
};

