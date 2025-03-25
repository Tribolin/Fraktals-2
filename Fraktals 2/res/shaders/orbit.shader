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
float pi =3.14159265359;
in vec2 v_TexCoord;
uniform float CR;
uniform float CTheta;
uniform float CPhi; 
uniform int N;
uniform int L;
uniform int M;
uniform float brightness;
uniform int real;
float widthComp = 1500/800;


vec4 texColor;

const float a0 = 1.0; 


float factorial(int n) {
    if (n <= 1) return 1.0;
    float result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= float(i);
    }
    return result;
}
float atan2(float x,float y)
{
    if(x>0)
    {
        return atan(y/x);
    }
    if(x<0 && y>0)
    {
        return atan(y/x)+pi;
    }
    if(x<0&&y== 0)
    {
        return pi;
    }
    if(x<0&&y<0)
    {
        return atan(y/x)-pi;
    }
    if(x==0&&y>0)
    {
        return pi/2;
    }
    return -pi/2;
}

float normalization_factor(int l, int m) {
    float num = (2.0 * float(l) + 1.0) * factorial(l - abs(m));
    float denom = 4.0 * 3.14159265359 * factorial(l + abs(m));
    return sqrt(num / denom);
}

vec2 complex_exponential(int m, float phi) {
    return vec2(cos(float(m) * phi), sin(float(m) * phi));
}

float associated_legendre(int l, int m, float x) {
    int absM = abs(m);
    
    if (absM > l) {
        return 0.0; 
    }

  
    float pmm = 1.0;
    if (absM > 0) {
        float sign = (absM % 2 == 0) ? 1.0 : -1.0;
        pmm = sign * factorial(2 * absM - 1) * pow(1.0 - x * x, float(absM) / 2.0);
    }

    if (l == absM) {
        return pmm;
    }

   
    float pmm1 = x * (2.0 * float(absM) + 1.0) * pmm;

    if (l == absM + 1) {
        return pmm1;
    }

    float pll = 0.0;
    for (int n = absM + 2; n <= l; n++) {
        pll = ((2.0 * float(n - 1) + 1.0) * x * pmm1 - (float(n - 1 + absM)) * pmm) / float(n - absM);
        pmm = pmm1;
        pmm1 = pll;
    }

   
    // if (m < 0) {
    //     pll *= pow(-1.0, float(m)) * factorial(l - absM) / factorial(l + absM);
    // }

    return pll;
}
vec2 spherical_harmonic(int l, int m, float theta, float phi) {
    float norm = normalization_factor(l, m);
    float legendre = associated_legendre(l, m, cos(theta));
    vec2 phase = complex_exponential(m, phi);
    return norm * legendre * phase;
}

float laguerre(int k, int alpha, float x) {
    if (k == 0) return 1.0;
    if (k == 1) return 1.0 + float(alpha) - x;
    float L0 = 1.0, L1 = 1.0 + float(alpha) - x, Lk;
    for (int i = 2; i <= k; ++i) {
        Lk = ((2.0 * float(i) + float(alpha) - 1.0 - x) * L1 - (float(i) + float(alpha) - 1.0) * L0) / float(i);
        L0 = L1;
        L1 = Lk;
    }
    return Lk;
}

vec2 psi_hydrogen(int n, int l, int m, float r, float theta, float phi) {
    if (n <= 0 || l < 0 || l >= n || abs(m) > l || r < 0.0) return vec2(0.0, 0.0);
    
    float rho = 2.0 * r / (float(n) * a0);
    
    float norm_factor = sqrt(pow(2.0 / (float(n) * a0), 3.0) * factorial(n - l - 1) / (2.0 * float(n) * factorial(n + l)));
    
    float radial_part = exp(-rho / 2.0) * pow(rho, float(l)) * laguerre(n - l - 1, 2 * l + 1, rho);
    
    vec2 angular_part = spherical_harmonic(l, m, theta, phi);
    
    return norm_factor * radial_part * angular_part;
}

float probability_density(vec2 wavefunction) {
    return dot(wavefunction, wavefunction);
}
vec2 CalcPsiOnPath(vec3 Pos, vec3 ray, float dx, float CameraR)
{
    vec2 sum = vec2(0,0);
    for(float i = 0;i<2*length(CameraR);i+=dx)
	{
		
		Pos+= ray*dx;
		float R = length(Pos);
		float theta = acos(Pos.z/R);
		float phi = atan2(Pos.x,Pos.y);
		sum += psi_hydrogen(N, L, M, R, theta, phi)*dx;
		

	}
    return sum;
}

void main()
{
	float CameraR = CR;
	float CameraTheta = CTheta;
    float CameraPhi = CPhi; 
	float focal = 1;
	vec3 Up = vec3(0,0,1);
	vec3 Campos = vec3(
		CameraR*sin(CameraTheta)*cos(CameraPhi),
		CameraR*sin(CameraTheta)*sin(CameraPhi),
		CameraR*cos(CameraTheta)
		);
	vec3 side = normalize(cross(Up,Campos))*(v_TexCoord.x-0.5)*2*widthComp;
	vec3 CamUp = normalize(cross(side,Campos));
	if(CamUp.z<0)
	{
		CamUp*=-1;
		}
	CamUp*=(v_TexCoord.y-0.5);
	vec3 ray =normalize( -normalize(Campos)*focal + side + CamUp);
	float dx = CameraR/100;
	vec2 result = vec2(0,0);
	float a = 1;
	vec3 Pos = Campos;

    float sum = 0;
    result = CalcPsiOnPath(Pos,ray,dx,CameraR);
    if(real== 0||M==0)
    {
	    
        sum = probability_density(result);
    }
    else{
        if(M<0)
        {
           sum = 2*result[1]*result[1];
          
        }
        else
        {
            sum = 2*result[0]*result[0];
        }
    }
    float enhancementFaktor= N*brightness*brightness;
    
	texColor =  vec4(sum*10*enhancementFaktor,sum*5*enhancementFaktor,0,1);

	color = texColor;
};
