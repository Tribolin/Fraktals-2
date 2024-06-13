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

uniform float Screensize;

in vec2 v_TexCoord;
vec3 Camerapos;
vec3 Sqherepos;
float focallenght;
float radius;

float mag(vec3 Vect)
{
	return sqrt(Vect.x*Vect.x + Vect.y*Vect.y + Vect.z*Vect.z);
}
vec3 Normalize(vec3 Vect)
{
	return Vect/sqrt(Vect.x*Vect.x+Vect.y*Vect.y+Vect.z*Vect.z);
}
float Dot(vec3 Vect1,vec3 Vect2)
{
	return Vect1.x*Vect2.x + Vect1.y*Vect2.y + Vect1.z*Vect2.z;
}
float DistanceFunk(vec3 Position)
{
	vec3 DistanceVec = Sqherepos-Position;
	float magnitude = sqrt(mag(DistanceVec));
	return (magnitude-radius);
}

void main()
{
	Camerapos = vec3(0,0,-5);
	Sqherepos = vec3(0,0,5);
	vec3 Pixelpos = vec3((v_TexCoord.x-0.5)*Screensize,v_TexCoord.y-0.5,0); 
	radius = 0.7;
	vec3 RayDir = Pixelpos-Camerapos;
	
	float distance = 0.0;
	vec3 Position = Camerapos;
	RayDir = Normalize(RayDir);
	vec4 texColor;
	int iterations = 0;
	while(distance < 100)
	{
		
		float currentdistance =  DistanceFunk(Position);
		Position= Position+ RayDir*currentdistance;
		distance= distance + currentdistance;
		iterations++;
		if(currentdistance < 0.000001)
		{
			vec3 noraml = Position-Sqherepos;
			noraml = Normalize(noraml);
			vec3 LightVec = Normalize(vec3(0.5,-0.5,1));
			
			float angel = acos(Dot(noraml,LightVec)/mag(noraml));

			float Brightness = angel/3.14;
			texColor =  vec4(Brightness,0,pow(0.9,1-Brightness),1);
			break;
		}
		else
		{
			texColor =  vec4(0,0,0,1);
		}
		
	}
	
	color = texColor;
};
