#include "Program.h"



Program::Program()
{
}
Program::Program(Renderer* renderer, UI* ui)
	:ui(ui), renderer(renderer)
{

}
void Program::setup()
{
}
void Program::render()
{
}
void Program::end()
{
}


//Mandelbrot Program

Mandelbrot::Mandelbrot(Renderer* ren, UI* Ui, std::string ShaderLocation )
	:shader(Shader(ShaderLocation)), zoomSpeed(0.0f), offsetChangeX(0.0f), offsetChangeY(0.0f), zoom(0.1f),
	anti_aliasing(false),iterations(5000),anti_aliasing_iterations{5}
{
	
	ui = Ui;
	renderer = ren;
	Frame.Unbind();
	shader.Unbind();
}

void Mandelbrot::setup()
{

}
void Mandelbrot::render()
{
	

	ui->Mandelbrot(zoomSpeed, offsetChangeX, offsetChangeY,iterations,anti_aliasing, anti_aliasing_iterations);

	shader.Bind();

	shader.SetUniform1f("zoom", zoom);
	shader.SetUniform2f("positionOffset", positionOffset[0], positionOffset[1]);
	shader.SetUniform1i("iterations",iterations);
	if(anti_aliasing)
		shader.SetUniform1i("antialy", anti_aliasing_iterations);
	else
		shader.SetUniform1i("antialy", 0);

	Frame.Bind();


	renderer->Draw(Frame.va, Frame.ib, shader);
	zoom += zoomSpeed / 100.f * zoom;
	positionOffset[0] += offsetChangeX / 100.0f / zoom;
	positionOffset[1] += offsetChangeY / 100.0f / zoom;
	offsetChangeX = 0;
	offsetChangeY = 0;
}
void Mandelbrot::end()
{
}

Koch_Schneeflocke::Koch_Schneeflocke(Renderer* renderer, UI* ui, std::string ShaderLocation)
	:shader(Shader(ShaderLocation))
{
	std::vector<Math::Vec2> Positions;

	Positions.push_back({ - 0.5, -0.5});
	Positions.push_back({0.5,-0.5});
	Positions.push_back({ 0,0.5 });
	Positions.push_back({ -0.5, -0.5 });

	for (size_t i = 0; i < 5; i++)
	{
		std::vector<Math::Vec2> PositionsBuffer;
		for (size_t i = 0; i < Positions.size()-1; i++)
		{
			Math::Vec2 Direktion{ Positions[i + 1]- Positions[i]   };
			float lenght = (Direktion ).magnitude();
			Direktion = Direktion/3;


			PositionsBuffer.push_back(Positions[i]);
			Math::Vec2 Pos = Positions[i] + Direktion;
			PositionsBuffer.push_back(Pos);

			float angle = -60 * Degree_to_Radian;
			Direktion = { (cos(angle) * Direktion.x - sin(angle) * Direktion.y),((sin(angle) * Direktion.x + cos(angle) * Direktion.y)) };

			Pos = Pos + Direktion;
			PositionsBuffer.push_back(Pos);

			 angle = 120 * Degree_to_Radian;
			Direktion = { (cos(angle) * Direktion.x - sin(angle) * Direktion.y),((sin(angle) * Direktion.x + cos(angle) * Direktion.y)) };

			Pos = Pos + Direktion;
			PositionsBuffer.push_back(Pos);

			
			PositionsBuffer.push_back(Positions[i+1]);





		}
		Positions = PositionsBuffer;
	}

	line.AddData(Positions);

	shader.Unbind();
	line.Unbind();
}
Koch_Schneeflocke::~Koch_Schneeflocke()
{
}
void Koch_Schneeflocke::render()
{
	shader.Bind();
	

	shader.SetUniform4f("u_Color", 0.0f,0.0f,1.0f,1.f);
	line.Bind();
	renderer->Draw(line.va,line.ib, shader);
	shader.Unbind();
	line.Unbind();
}

Ray_Marching_Test::Ray_Marching_Test(GLFWwindow* window,Renderer* ren, UI* Ui, std::string ShaderLocation)
	:shader(Shader(ShaderLocation)), kw(1),window(window)
{
	renderer = ren;
	ui = Ui;
}

Ray_Marching_Test::~Ray_Marching_Test()
{
}

void Ray_Marching_Test::render()
{
	shader.Bind();

	//shader.SetUniform1f("focallenght", 1.0 );
	int height;
	int width;
	glfwGetWindowSize(window,&width,&height);

	float ratio = (float)width / (float)height;
	shader.SetUniform1f("Screensize", ratio);

	Frame.Bind();
	renderer->Draw(Frame.va, Frame.ib, shader);
	
}

Julia4D::Julia4D(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation)
	: shader(Shader(ShaderLocation)), kw(1), window(window)
{
	renderer = ren;
	ui = Ui;
}
Julia4D::~Julia4D()
{

}

void Julia4D::render()
{
	shader.Bind();

	//shader.SetUniform1f("focallenght", 1.0 );
	int height;
	int width;
	glfwGetWindowSize(window, &width, &height);

	float ratio = (float)width / (float)height;
	shader.SetUniform1f("Screensize", ratio);

	Frame.Bind();
	renderer->Draw(Frame.va, Frame.ib, shader);

}
