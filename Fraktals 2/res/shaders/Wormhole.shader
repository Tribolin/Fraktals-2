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
uniform samplerCube Texture1;
uniform samplerCube Texture2;

#define pi 3.1415926538

uniform float a;
uniform float M;
float dt = -.01;
int maxSteps = 1000;

uniform float CamLenght ;
uniform float zoom  = 1;
uniform float angle = 1;
uniform float x_Offset;
vec2 iResulution = vec2(1,1);

float b;
float B2;



//Wormhole funktion
float LtoR(float l)
{
	float rho = 1;
	if(abs(l) < a)
	{
		return rho;
	}
	float x = 2*(abs(l)-a)/(pi*M);
	return rho+M*(x*atan(x)-.5*log(1+x*x));
}
//derivative
float LtoDR(float l)
{
	if(abs(l) < a)
	{
		return 0;
	}
	float x = 2*(abs(l)-a)/(pi*M);
	return 2*atan(x)*sign(l)/pi;
}
vec4 multiplyQ(vec4 q1,vec4 q2)
{
	float r = q1.x;
	float i = q1.y;
	float j = q1.z;
	float k = q1.w;
	
	float br = q2.x;
	float bi = q2.y;
	float bj = q2.z;
	float bk = q2.w;

	return vec4(
		((r * br) - (i * bi) - (j * bj) - (k * bk)),
		((r * bi) + (i * br) + (j * bk) - (k * bj)),
		((r * bj) - (i * bk) + (j * br) + (k * bi)),
		((r * bk) + (i * bj) - (j * bi) + (k * br)));


}


void main()
{
	vec2 uv = (2*v_TexCoord-iResulution.xy)/iResulution.x;
	float l= CamLenght;
	float theta = -pi/2;
	float phi = x_Offset;
	//vec3 N = vec3(sin(uv.x)*cos(uv.y),sin(uv.x)*sin(uv.y),cos(uv.x));
	//vec3 n = vec3(-N.x,-N.y,N.z);
	vec3 N =  normalize(vec3(zoom, uv));
	float nL = -N.x;
	float nPhi = -N.y;
	float nTheta = N.z;
	float r = LtoR(l);
	
	float pL = nL;
	float pTheta = r*nTheta;
	float pPhi = r * sin(theta)*nPhi;

	b = pPhi;
	B2 = r*r*(nTheta*nTheta+nPhi*nPhi);

	float steps = 0;
	float epsilon = 1e-6;
	

	while(abs(l)< abs(2*CamLenght) && steps < maxSteps)
	{

		
		float dl = dt*pL;
		

		r = LtoR(l);
		

		float dTheta = pTheta/(r*r +epsilon) *dt;
		
		
		float dPhi = b/(r*r*sin(theta)*sin(theta)+epsilon) *dt;
		
		
		float dpL= B2 * LtoDR(l)/(pow(r,3))  *dt;
		float dpTheta = (b*b*cos(theta))/(r*r*sin(theta)*sin(theta)*sin(theta)+epsilon) *dt;

		pL+= dpL;
		pTheta+= dpTheta;
		l+=dl;
		theta+=dTheta;
		phi += dPhi;

		steps++;
		
		
	}
 

	
	vec3 direction = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
	
	if(l < 0)
	{
		//color = vec4(theta/(2*pi),theta/(2*pi),theta/(2*pi),1);
		color = texture(Texture1,direction);
	}
	else{
		color = texture(Texture2,direction);
		}
};



// vec2 uv = (2*v_TexCoord-iResulution.xy)/iResulution.x;
// 	vec3 vel = normalize(vec3(-zoom,uv));
	

// 	// vec3 RotationVec = vec3(0,0,1);
// 	// vec4 ToRotate = vec4(0,vel);
// 	// vec4 RotationHelper1 = vec4(cos(angle),RotationVec*sin(angle));
// 	// vec4 RotationHelper2 = vec4(cos(-1*angle),RotationVec*sin(-1*angle));
// 	// vec4 mid = multiplyQ(RotationHelper1,ToRotate);
// 	// vel = multiplyQ(mid,RotationHelper2).yzw;
	
	
//     vec2 beta = normalize(vel.yz);

// 	float l = CamLenght;
// 	float r = LtoR(l);
// 	float dl = 1;
// 	float H = r* length(vel.yz);
// 	float phi = 0;

// 	float dr;

// 	int steps = 0;
// 	while(abs(l) < max(abs(CamLenght)*2,a+2) && steps < maxSteps)
// 	{
// 		dr = LtoDR(l);
// 		r = LtoR(l);
// 		l+= dl*dt;
// 		phi += H/r/r*dt;
// 		dl += H*H*dr/r/r/r*dt;
// 		steps++;
// 	}
	
// 	float dx = dl*dr*cos(phi)-H/r*sin(phi);
// 	float dy = dl*dr*sin(phi)+H/r*cos(phi);

// 	vec3 vec = normalize(vec3( dx,dy*beta));
// 	vec3 CubeVec = vec3(-vec.x,vec.z,-vec.y);

// 	if(l > 0.)
// 	{
// 		color = texture(Texture1,CubeVec);
// 	}
// 	else
// 	{
		
// 		color = texture(Texture2,CubeVec);
// 	}
	