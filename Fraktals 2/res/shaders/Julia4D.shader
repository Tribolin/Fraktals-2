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
uniform vec4 C;
uniform vec3 Camerapos;
in vec2 v_TexCoord;

vec3 Sqherepos;
float focallenght;
float radius;

float mag(vec3 Vect)
{
	return sqrt(Vect.x*Vect.x + Vect.y*Vect.y + Vect.z*Vect.z);
}
float qlength2(vec4 q1)
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
vec4 qsqr( in vec4 a )
{
    return vec4( a.x*a.x - a.y*a.y - a.z*a.z - a.w*a.w,
                 2.0*a.x*a.y,
                 2.0*a.x*a.z,
                 2.0*a.x*a.w );
}


vec3 Normalize(vec3 Vect)
{
	return Vect/sqrt(Vect.x*Vect.x+Vect.y*Vect.y+Vect.z*Vect.z);
}

float DistanceFunk(vec3 Position,vec4 c)
{
	vec3 DistanceVec = Sqherepos-Position;
	vec4 Q = vec4(DistanceVec.x,DistanceVec.y,DistanceVec.z,0);
	// if(Position.y > 0.5)
	// {
	// 	return Position.y-0.5;
	// }
	vec4 qD = vec4(1.0,0.0,0.0,0.0);
	

	float md2 = 1.0;
	float mz2 = dot(Q,Q);
	float n = 1.0;
	for(int i = 0;i<100;i++)
	{
		 md2 *= 4.0*mz2;
        // z  -> z^2 + c
        Q = multiplyQ(Q,Q) + c;  

        

        mz2 = qlength2(Q);
        if(mz2>4.0) break;
        n += 1.0;
	}

	float distance =  0.25*sqrt(mz2/md2)*log(mz2);
	return distance;
}
vec3 calcNormal( in vec3 p, in vec4 c )
{
       const vec2 e = vec2(0.001,0.0);
    vec4 za=vec4(p+e.xyy,0.0); float mz2a=qlength2(za), md2a=1.0;
    vec4 zb=vec4(p-e.xyy,0.0); float mz2b=qlength2(zb), md2b=1.0;
    vec4 zc=vec4(p+e.yxy,0.0); float mz2c=qlength2(zc), md2c=1.0;
    vec4 zd=vec4(p-e.yxy,0.0); float mz2d=qlength2(zd), md2d=1.0;
    vec4 ze=vec4(p+e.yyx,0.0); float mz2e=qlength2(ze), md2e=1.0;
    vec4 zf=vec4(p-e.yyx,0.0); float mz2f=qlength2(zf), md2f=1.0;
  	for(int i=0; i<11; i++)
    {
        md2a *= mz2a; za = qsqr(za) + c; mz2a = qlength2(za);
        md2b *= mz2b; zb = qsqr(zb) + c; mz2b = qlength2(zb);
        md2c *= mz2c; zc = qsqr(zc) + c; mz2c = qlength2(zc);
        md2d *= mz2d; zd = qsqr(zd) + c; mz2d = qlength2(zd);
        md2e *= mz2e; ze = qsqr(ze) + c; mz2e = qlength2(ze);
        md2f *= mz2f; zf = qsqr(zf) + c; mz2f = qlength2(zf);
    }
    float da = sqrt(mz2a/md2a)*log2(mz2a);
    float db = sqrt(mz2b/md2b)*log2(mz2b);
    float dc = sqrt(mz2c/md2c)*log2(mz2c);
    float dd = sqrt(mz2d/md2d)*log2(mz2d);
    float de = sqrt(mz2e/md2e)*log2(mz2e);
    float df = sqrt(mz2f/md2f)*log2(mz2f);
    
    return normalize( vec3(da-db,dc-dd,de-df) );
}
vec4 Ray(vec3 StartPos,vec3 RayDir,float depth)
{
	vec3 Position = StartPos;
	float distance = 0.0;

	while(distance < depth)
	{
		
		float currentdistance =  DistanceFunk(Position, C);
		Position= Position+ RayDir*currentdistance;
		distance= distance + currentdistance;
		if(currentdistance<0.00001)
		{
			return vec4(Position,distance);
		}
		
	}
	return vec4(StartPos,-1);
}

void main()
{
	vec3 Up = vec3(0,0,1);
	vec3 side = Normalize(cross(Up,Camerapos));
	vec3 Pixelpos = vec3(0,(v_TexCoord.x-0.5)*Screensize,v_TexCoord.y-0.5); 
	Pixelpos = side *( v_TexCoord.x-0.5)*2 + Up*(v_TexCoord.y-0.5)*2;
	vec3 RayDir = Normalize(Pixelpos-Camerapos);
	vec3 Position = Camerapos;


	float distance = 0.0;
	
	vec4 texColor;
	
	vec4 RayReturn = Ray(Position,RayDir,100);
	Position = vec3(RayReturn.x,RayReturn.y,RayReturn.z);
	distance = RayReturn.w;

	if(Position != Camerapos)
	{
			vec3 normal = calcNormal( Position,C);
			vec3 Light = vec3(10,10,10);
			
			RayDir= Normalize(Light-Position);
			float angle = acos(dot(normal,RayDir)/(mag(normal)*mag(RayDir)));
			
			float brighness =1/angle;
			vec3 MateriaSettings = vec3(1,1,1);
			
			texColor = vec4(MateriaSettings*brighness,1);
			
			
			
	}
	else
		{
			texColor =  vec4(0,0,0,1);
		}
	
	
	color = texColor;
};
