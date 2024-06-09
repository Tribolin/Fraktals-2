#shad#shader vertex
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
vec3 Camerapos;
vec3 Sqherepos;
float focallenght;
float radius;


void main()
{
	Camerapos = vec3(0,0,-1);
	Sqherepos = vec3(0,0,10);
	vec3 Pixelpos = vec3(v_TexCoord.x,v_TexCoord.x,0); 
	radius = 1.0;
	vec3 RayDir = Pixelpos-Camerapos;
	float distance = 0;
	while(distance < 11)
	{
		distance++;
	}
	vec4 texColor =  vec4(v_TexCoord.y,0,0,1);
	color = texColor;
};
