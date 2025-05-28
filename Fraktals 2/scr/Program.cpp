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
	rotation = 0;
}
Julia4D::~Julia4D()
{

}

void Julia4D::render()
{

	shader.Bind();
	 float SpeedRotation = 0;
	 float UpChange = 0.0;

	//shader.SetUniform1f("focallenght", 1.0 );
	int height;
	int width;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	static float c1  = -0.5;
	static float c2 = 0.5;
	static float c3  = 0.5;
	static float c4  = -0.5;
	static float cut = 0;


	static CubeMap sky1 = CubeMap({
			"res/textures/skybox4/px.png",
			"res/textures/skybox4/nx.png",

			"res/textures/skybox4/nz.png",
			"res/textures/skybox4/pz.png",
			

			"res/textures/skybox4/py.png",
			"res/textures/skybox4/ny.png",


			
		}); 
	sky1.Bind(0);
	shader.SetUniform1i("Texture1", 0);

	ui->Julia4D(c1,c2,c3,c4,SpeedRotation,UpChange,cut);
	float ratio = (float)width / (float)height;
	shader.SetUniform1f("Screensize", ratio);
	shader.SetUniform4f("C", c1, c2,c3, c4);
	shader.SetUniform1f("cut", cut);
	
	static float UpAngle = 0;
	
	UpAngle += UpChange/100;
	rotation+=  SpeedRotation;
	float radius = 2;
	
	Math::Vec3 base = Math::Vec3(cos(rotation)* radius, sin(rotation)* radius, 0);
	Math::Vec3 RotationAxis = Math::cross(base,Math::Vec3(0,0,1));
	Math::Quaternion RotAxis = Math::Quaternion(cos(UpAngle), RotationAxis*sin(UpAngle));
	Math::Quaternion InverseAxis = Math::Quaternion(cos(-1*UpAngle), RotationAxis * sin(-1*UpAngle));
	Math::Quaternion qBase = Math::Quaternion(0,base);
	Math::Quaternion Pos = RotAxis * qBase * InverseAxis;

	shader.SetUniform3f("Camerapos",Pos.i, Pos.j, Pos.k);

	Frame.Bind();
	renderer->Draw(Frame.va, Frame.ib, shader);

}


//Fluid Simulation
Partikle::Partikle(Math::Vec2 position)
	:lastPosition(position),position(position)
{
}

Partikle::~Partikle()
{
}
FluidSim::FluidSim(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation)
	: shader(Shader(ShaderLocation)), window(window), PartikelCount(1)
{
	
	renderer = ren;
	ui = Ui;
}
void FluidSim::setup()
{
	Time = clock();
}
void FluidSim::render()
{
	
	double deltaT = double(clock()-Time)/100000;
	Time = clock();
	static float deltaY = 0;
	static float y = 0;
	deltaY = deltaY - deltaT * 9.81;
	
	if (y < -0.5)
	{
		deltaY *=-1;
	}
	y += deltaY;
	shader.Bind();
	Box2D Box = Box2D({0,y},0.1);
	

	shader.SetUniform4f("u_Color", 1.0f, 0.0f, 1.0f, 1.f);
	Box.Bind();
	renderer->Draw(Box.va, Box.ib, shader);
	shader.Unbind();
	Box.Unbind();
	
}

//Wormhole
Wormhole::Wormhole(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation )
	:shader(ShaderLocation),window(window)
{
	renderer = ren;
	ui = Ui;
}
void Wormhole::render()
{
	shader.Bind();
	Frame.Bind();
	static float a = 0.1;
	static float M = 0.1;
	static float l = -5;
	static float zoom = 1;
	static float angle = 0;
	static float x_Offset;
	ui->Wormhole(a,M, zoom,angle,x_Offset,l);

	static CubeMap sky1 = CubeMap({ 
		"res/textures/skybox4/px.png",
			"res/textures/skybox4/nx.png",

			"res/textures/skybox4/nz.png",
			"res/textures/skybox4/pz.png",


			"res/textures/skybox4/py.png",
			"res/textures/skybox4/ny.png",
		});
	/*static CubeMap sky2 = CubeMap(
		{
			
			"res/textures/skybox3/Left_Tex.png",
			"res/textures/skybox3/Right_Tex.png",

			"res/textures/skybox3/Up_Tex.png",
			"res/textures/skybox3/Down_Tex.png",
			

			"res/textures/skybox3/Front_Tex.png",
			"res/textures/skybox3/Back_Tex.png",
		
		});*/
	shader.SetUniform1i("Texture1",1);
	shader.SetUniform1i("Texture2", 1);
	shader.SetUniform1f("a", a);
	shader.SetUniform1f("M", M);
	shader.SetUniform1f("zoom", zoom);
	shader.SetUniform1f("angle", angle);
	shader.SetUniform1f("x_Offset", x_Offset);
	shader.SetUniform1f("CamLenght", l);
	sky1.Bind(0);
	sky1.Bind(1);

	
	renderer->Draw(Frame.va, Frame.ib, shader);
}

Wormhole::~Wormhole()
{

}

Orbit::Orbit(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation,std::string ShaderLocation2)
	:shader(ShaderLocation),shader3d(ShaderLocation2), window(window)
{
	renderer = ren;
	ui = Ui;
}
void Orbit::render()
{
	int height;
	int width;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader* shaderPtr;
	if (Render3D)
	{
		shaderPtr = &shader3d;
	}
	else
	{
		shaderPtr = &shader;
	}
	shaderPtr->Bind();
	Frame.Bind();
	ui->Orbit(r,psi,theta,n,l,m,brightness,real,Render3D);
	shaderPtr->SetUniform1f("CR", r);
	shaderPtr->SetUniform1f("CPhi", psi);
	shaderPtr->SetUniform1f("CTheta", theta);
	shaderPtr->SetUniform1f("brightness", brightness);
	shaderPtr->SetUniform1i("N",n);
	shaderPtr->SetUniform1i("L", l);
	shaderPtr->SetUniform1i("M", m);
	shaderPtr->SetUniform1i("real", real);

	renderer->Draw(Frame.va, Frame.ib, *shaderPtr);
}
Orbit::~Orbit()
{

}



Doppelspalt::Doppelspalt(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation)
	:shader(ShaderLocation), window(window)
{
	renderer = ren;
	ui = Ui;
}
void Doppelspalt::render()
{
	
	renderer->Draw(Frame.va, Frame.ib, shader);
}
Doppelspalt::~Doppelspalt()
{

}

ConwayLive::ConwayLive(GLFWwindow* window, Renderer* ren, UI* Ui, std::string ShaderLocation)
	:shader(ShaderLocation), window(window), texture(1, 1), framebuffer(texture), texture2(1, 1), AktiveTexture(false)
{
	int height;
	int width;
	renderer = ren;
	ui = Ui;
	
}
ConwayLive::~ConwayLive()
{
}
void ConwayLive::render()
{


	int height;
	int width;
	glfwGetWindowSize(window, &width, &height);
	shader.Bind();
	if (width != texture.width || height != texture.height)
	{
		texture.Delete();
		texture2.Delete();
		texture = ComputeTexture(width, height);
		texture2 = ComputeTexture(width, height);
		
		
		shader.SetUniform1i("Run", 0);
	}
	else
	{
		
		shader.SetUniform1i("Run", 1);
	}
	if (AktiveTexture)
	{
		framebuffer.AttachTexture(texture);
		GLCall(glBindImageTexture(1, texture2.m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
		AktiveTexture = false;
	}
	else
	{
		framebuffer.AttachTexture(texture2);
		GLCall(glBindImageTexture(1, texture.m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
		AktiveTexture = true;
	}
	GLCall(glBindImageTexture(0, framebuffer.ColorAttachment.m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
	
	

	
	const GLuint workGroupSizeX = 20;
	const GLuint workGroupSizeY = 20;

	GLuint numGroupsX = (width + workGroupSizeX - 1) / workGroupSizeX;
	GLuint numGroupsY = (height + workGroupSizeY - 1) / workGroupSizeY;
	shader.Dispatch(numGroupsX, numGroupsY, 1);
	shader.WaitForCompletion();
	
	framebuffer.BlitFramebufferToSwapchain();
	
	
}	