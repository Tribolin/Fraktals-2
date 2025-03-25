#pragma once
#include <time.h>
#include "Shader.h"
#include"Renderer.h"
#include "UI.h"
#include "Objekt.h"
#include "Math.h"

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


//Julia Frakrals using Quaternions instead of Complex Numbers(Main Part in shader)
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


//Fluid Simulation
class Partikle
{
public:
	Partikle(Math::Vec2 position);
	~Partikle();

private:
	Math::Vec2 position;
	Math::Vec2 lastPosition;
};
class FluidSim :public Program
{
private:
	Shader shader;
	GLFWwindow* window;
	std::vector<Partikle> ParticleManager;
	clock_t Time;
public:
	int PartikelCount;

	FluidSim(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/Basic.shader");
	virtual void render();
	virtual void setup();
};

//Wormhole simulation
class Wormhole :public Program
{
private:
	Tex_Fraktal2D Frame;
	Shader shader;
	GLFWwindow* window;
public:
	Wormhole(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/Wormhole.shader");
	~Wormhole();

	virtual void render();
};

//Orbital simulation
class Orbit :public Program
{
private:
	Tex_Fraktal2D Frame;
	Shader shader;
	Shader shader3d;
	GLFWwindow* window;
	float r = 1;
	float psi = 0;
	float theta = 0.5 * 3.14159265359;
	int n = 1;
	int l = 0;
	int m = 0;
	float brightness = 1;
	int real = 1;
	
	bool Render3D = false;

public:
	Orbit(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/orbit.shader", std::string ShaderLocation2 = "res/shaders/Orbital3D.shader");
	~Orbit();

	virtual void render();
};

class Doppelspalt :public Program
{
private:
	Tex_Fraktal2D Frame;
	Shader shader;
	GLFWwindow* window;
public:
	Doppelspalt(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/Doppelspalt.shader");
	~Doppelspalt();

	virtual void render();
};

class ConwayLive :public Program
{
	private:
		Shader shader;
		GLFWwindow* window;
	public:
		ConwayLive(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation = "res/shaders/Basic.shader");
		~ConwayLive();
		virtual void render();
};