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
float magQ2(vec4 q1)
{
	return q1.x*q1.x +q1.y*q1.y +q1.z*q1.z +q1.w*q1.w;
}
vec4 multiplyQ(vec4 q1,vec4 q2)
{
	float r = q1.x;
	float i = q1.y;
	float j = q1.z;
	float k = q1.w;
	
	float br = q1.x;
	float bi = q1.y;
	float bj = q1.z;
	float bk = q1.w;

	return vec4(
		((r * br) - (i * bi) - (j * bj) - (k * bk)),
		((r * bi) + (i * br) + (j * bk) - (k * bj)),
		((r * bj) - (i * bk) + (j * br) + (k * bi)),
		((r * bk) + (i * bj) - (j * bi) + (k * br)));


}


vec3 Normalize(vec3 Vect)
{
	return Vect/sqrt(Vect.x*Vect.x+Vect.y*Vect.y+Vect.z*Vect.z);
}

float DistanceFunk(vec3 Position)
{
	vec3 DistanceVec = Sqherepos-Position;
	vec4 Q = vec4(DistanceVec.x,DistanceVec.y,DistanceVec.z,0);

	vec4 qD = vec4(1.0,0.0,0.0,0.0);
	vec4 c = vec4(-0.291,-0.399,0.339,0.437);

	float md2 = 1.0;
	float mz2 = dot(Q,Q);
	float n = 1.0;
	for(int i = 0;i<1024;i++)
	{
		 md2 *= 4.0*mz2;
        // z  -> z^2 + c
        Q = multiplyQ(Q,Q) + c;  

        

        mz2 = magQ2(Q);
        if(mz2>4.0) break;
        n += 1.0;
	}

	float distance =  0.25*sqrt(mz2/md2)*log(mz2);
	return distance;
}

void main()
{
	Camerapos = vec3(0,0,-5);
	
	vec3 Pixelpos = vec3((v_TexCoord.x-0.5)*Screensize,v_TexCoord.y-0.5,-3); 
	
	vec3 RayDir = Normalize(Pixelpos-Camerapos);
	vec3 Position = Camerapos;


	float distance = 0.0;
	
	vec4 texColor;
	float iterations = 0;
	while(distance < 100)
	{
		iterations++;
		float currentdistance =  DistanceFunk(Position);
		Position= Position+ RayDir*currentdistance;
		distance= distance + currentdistance;
		if(currentdistance<0.00001)
		{
			texColor = vec4(distance/10,0,0,1);
			break;
		}
		else
		{
			texColor =  vec4(0,0,0,1);
		}
	}
	
	color = texColor;
};
