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
void main()
{
	float real = (v_TexCoord.x/0.4)/zoom - 2/zoom;
	float img = (v_TexCoord.y/0.5)/zoom- 1/zoom;
	float nreal = 0.0;
	float nimg = 0.0;


	int counter = 0;
	while(mag2(nreal,nimg))
	{
		float bufreal = nreal;
		nreal = (nreal*nreal)-(nimg*nimg)+real;
		nimg = (2*bufreal*nimg )+img;
		counter++;
		if(counter > 254)
		{
			break;
		}
	}

	vec4 texColor =  vec4(0,counter/255,0,1);
	color = texColor;
};
