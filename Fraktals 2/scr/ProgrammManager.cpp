#include "ProgrammManager.h"


ProgrammManager::ProgrammManager(GLFWwindow* window, const char* glsl_version, Renderer* renderer, UI* ui)
	:window{ window }, renderer{ renderer }, ui{ ui }
{
	program = ProgramList.at(currentProgramIndex);

}

ProgrammManager::~ProgrammManager()
{
}

bool ProgrammManager::run()
{
	static bool show_demo_window = true;

	while (!glfwWindowShouldClose(window))
	{
		renderer->Clear();
		ui->NewFrame();
		ui->DemoWindow(show_demo_window);


		int lastProgamIndex = currentProgramIndex;

		ui->Manager(currentProgramIndex);

		if (lastProgamIndex!= currentProgramIndex)
		{
			program->end();
			program = ProgramList.at(currentProgramIndex);
			program->setup();
		}
		
		//Render
		program->render();
		
		ui->Render();



		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
	}
	return true;
}
