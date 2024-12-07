#pragma once
#include "Shader.h"
#include"Renderer.h"
#include "UI.h"
#include "Objekt.h"

class Program
{
private:
protected:
	Renderer* renderer;
	UI* ui;
public:
	
	Program();
	Program(Renderer* renderer, UI* ui);
	~Program() = default;
	virtual void setup();
	virtual void render();
	virtual void end();
};



class Mandelbrot : public Program
{
private:
	

	float zoomSpeed;
	float offsetChangeX;
	float offsetChangeY;

	float zoom;
	float positionOffset[2];

	int iterations;
	bool anti_aliasing;
	int anti_aliasing_iterations;

	Tex_Fraktal2D Frame;
	Shader shader;
public:
	Mandelbrot(Renderer* renderer, UI* ui , std::string ShaderLocation = "res/shaders/Mandelbrot.shader");
	void setup() override;
	void render()  override;
	void end() override;

};
class Koch_Schneeflocke : public Program
{
private:
	Line line;
	Shader shader;
public:
	Koch_Schneeflocke(Renderer* renderer, UI* ui, std::string ShaderLocation = "res/shaders/Basic.shader");
	~Koch_Schneeflocke();

	virtual void render();

};
class Ray_Marching_Test : public Program
{
private:
	Tex_Fraktal2D Frame;
	Shader shader;
	float kw;
	GLFWwindow* window;

public:
	Ray_Marching_Test(GLFWwindow* window,Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/ray_marching_test.shader");
	~Ray_Marching_Test();
	virtual void render();
};

class Julia4D:public Program
{
private:
	Tex_Fraktal2D Frame;
	Shader shader;
	float kw;
	float rotation ;
	GLFWwindow* window;
public:
	Julia4D(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/Julia4D.shader");
	~Julia4D();
	
	virtual void render();
};
