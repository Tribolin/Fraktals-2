#pragma once
#include "Program.h"
#include "UI.h"
//

class ProgrammManager
{
public:
	ProgrammManager(GLFWwindow* window,const char* glsl_version,Renderer* renderer, UI* ui);
	~ProgrammManager();
	bool run();
private:


	GLFWwindow* window;
	Renderer* renderer;
	UI* ui;

	int currentProgramIndex = 0;

	//Programms
	Mandelbrot mandelbrot = Mandelbrot(renderer,ui);
	Koch_Schneeflocke Flocke = Koch_Schneeflocke(renderer,ui);
	Ray_Marching_Test ray_marching_test = Ray_Marching_Test(window,renderer,ui);
	Julia4D julia4d = Julia4D(window, renderer, ui);
	FluidSim fluid = FluidSim(window,renderer,ui);
	Wormhole wormhole = Wormhole(window,renderer,ui);
	Orbit orbit = Orbit(window,renderer,ui);
	Doppelspalt doppel = Doppelspalt(window, renderer, ui);

	std::vector<Program*> ProgramList = { &mandelbrot,&Flocke, &ray_marching_test,&julia4d,&fluid,&wormhole,&orbit,&doppel};
	Program* program;
};

