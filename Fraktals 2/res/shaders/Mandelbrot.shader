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

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float zoom;
uniform vec2 positionOffset;

bool mag2(float real, float img)
{
	float mag = real * real + img * img;
	if(mag > 4)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int clac(float real, float img, int iterations)
{
	float nreal = 0.0;
	float nimg = 0.0;

	int counter = 0;
	while(mag2(nreal,nimg))
	{
		float bufreal = nreal;
		nreal = (nreal*nreal)-(nimg*nimg)+real;
		nimg = (2*bufreal*nimg )+img;
		counter++;
		if(counter > iterations)
		{
			return counter;
		}
	}
	return counter;
}

void main()
{
	float real = (v_TexCoord.x/0.4)/zoom - 2/zoom + positionOffset.x;
	float img = (v_TexCoord.y/0.5)/zoom- 1/zoom + positionOffset.y;
	
	int iterations = 500;
	int aly = 20;
	int counter = clac(real,img,iterations);

	for(int i = 0; i<aly; i+= 1)
	{
		float areal = real+ sin(i*(2*3.14159)/aly)/1000/zoom;
		float aimg = img+ cos(i*(2*3.14159)/aly)/1000/zoom;
		counter += clac(areal,aimg,iterations);
	}
	counter= counter/(aly+1);

	vec4 texColor =  vec4(0,counter/(iterations+1),0,1);
	color = texColor;
};
