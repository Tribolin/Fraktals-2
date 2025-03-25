#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
void main()
{
  gl_Position =  position;
  v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

float pi =3.14159265359;

void main()
{
	float pos = abs(v_TexCoord.x*2-1);
	float lambda = 800e-9;
	float d = 10e-6;
	float b = 3e-6;
	float e = 0.5;
	
	vec2 mag = vec2(0,0);
	for(float i = -0.5*d-0.5*b;i<-0.5*d+0.5*b;i+= b/400)
	{
		float t = sqrt(e*e+(pos-i)*(pos-i));
		mag+= vec2(cos(2*pi/lambda*t),sin(2*pi/lambda*t))/400;
	}
	for(float i = 0.5*d-0.5*b;i<0.5*d+0.5*b;i+= b/400)
	{
		float t = sqrt(e*e+(pos-i)*(pos-i));
		mag+= vec2(cos(2*pi/lambda*t),sin(2*pi/lambda*t))/400;
	}
	float k = length(mag)*length(mag);
	
	color = vec4(k,k/4,k/10,1);
};
