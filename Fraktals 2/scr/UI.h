#pragma once
#include "..\res\imgui\imgui.h"
#include "..\res\imgui\imgui_impl_glfw.h"
#include "..\res\imgui\imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

class UI
{
public:
	UI(const char* glsl_version, GLFWwindow* window);
	~UI();

	void Render();
	void NewFrame();
	//UIs
	void Mandelbrot(float& zoomSpeed,float& offsetChangeX, float& offsetChangeY);


private:
	
};

