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

uniform sampler2D u_Texture;

uniform float zoom;
uniform int iterations;
uniform int antialy;
uniform vec2 positionOffset;

vec4 colors[2]= {
	{0.0,1.0,0.0,1.0},
	{1.0,0.0,0.0,1.0}
	};

bool mag2(float real, float img)
{
	float mag = real * real + img * img;
	if(mag > 6)
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
	iterations -= 2;
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
	
	
	int aly = antialy;
	int counter = clac(real,img,iterations);

	for(int i = 0; i<aly; i+= 1)
	{
		float areal = real+ sin(i*(2*3.14159)/aly)/500/zoom;
		float aimg = img+ cos(i*(2*3.14159)/aly)/1000/zoom;
		counter += clac(areal,aimg,iterations);
	}
	if(aly > 0)
		counter= counter/aly;
	int n = iterations/10;
	float mody = mod(counter, n);
	

	vec4 texColor =  vec4(0,0,mody/n,1);
	color = texColor;
};
