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
float Threshold = 1/(10000*brightness*brightness);

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
vec4 CheckPsi(vec3 Pos, vec3 ray, float dx, float CameraR)
{
    vec2 result = vec2(0,0);
    float sum = 0;
    for(float i = 0;i<2*length(CameraR);i+=dx)
	{
		
		Pos+= ray*dx;
		float R = length(Pos);
		float theta = acos(Pos.z/R);
		float phi = atan2(Pos.x,Pos.y);
		result = psi_hydrogen(N, L, M, R, theta, phi);
        
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
        if(sum > Threshold)
        {
            return vec4(Pos,1/(i*dx));
        }
	}
    return vec4(0,0,0,0);
}
vec3 calcNormal(vec3 p)
{
    const vec2 div = vec2(0.001,0.0);
    vec3 a=vec3(p+div.xyy); 
    vec3 b=vec3(p-div.xyy); 
    vec3 c=vec3(p+div.yxy); 
    vec3 d=vec3(p-div.yxy); 
    vec3 e=vec3(p+div.yyx); 
    vec3 f=vec3(p-div.yyx); 

    //vec3(R,Phi,Theta)
    vec3 ka = vec3(length(a), atan2(a.x, a.y), acos(a.z / length(a)));
    vec3 kb = vec3(length(b), atan2(b.x, b.y), acos(b.z / length(b)));
    vec3 kc = vec3(length(c), atan2(c.x, c.y), acos(c.z / length(c)));
    vec3 kd = vec3(length(d), atan2(d.x, d.y), acos(d.z / length(d)));
    vec3 ke = vec3(length(e), atan2(e.x, e.y), acos(e.z / length(e)));
    vec3 kf = vec3(length(f), atan2(f.x, f.y), acos(f.z / length(f)));



    
    if(real== 0||M==0)
    {
        float T = probability_density(psi_hydrogen(N, L, M, length(p), atan2(p.x, p.y), acos(p.z / length(p))));
	    vec3 va = (Threshold - probability_density(psi_hydrogen(N, L, M, ka.x, ka.z, ka.y))) * div.xyy;
        vec3 vb = (Threshold - probability_density(psi_hydrogen(N, L, M, kb.x, kb.z, kb.y))) * -div.xyy;
        vec3 vc = (Threshold - probability_density(psi_hydrogen(N, L, M, kc.x, kc.z, kc.y))) * div.yxy;
        vec3 vd = (Threshold - probability_density(psi_hydrogen(N, L, M, kd.x, kd.z, kd.y))) * -div.yxy;
        vec3 ve = (Threshold - probability_density(psi_hydrogen(N, L, M, ke.x, ke.z, ke.y))) * div.yyx;
        vec3 vf = (Threshold - probability_density(psi_hydrogen(N, L, M, kf.x, kf.z, kf.y))) * -div.yyx;


        vec3 normal = va + vb + vc + vd + ve + vf;
        return normal;
        
          
    }
    else
    {
       if(M<0)
       {
            vec3 va = (Threshold - 2 * psi_hydrogen(N, L, M, ka.x, ka.z, ka.y)[1] * psi_hydrogen(N, L, M, ka.x, ka.z, ka.y)[1]) * div.xyy;
            vec3 vb = (Threshold - 2 * psi_hydrogen(N, L, M, kb.x, kb.z, kb.y)[1] * psi_hydrogen(N, L, M, kb.x, kb.z, kb.y)[1]) * -div.xyy;
            vec3 vc = (Threshold - 2 * psi_hydrogen(N, L, M, kc.x, kc.z, kc.y)[1] * psi_hydrogen(N, L, M, kc.x, kc.z, kc.y)[1]) * div.yxy;
            vec3 vd = (Threshold - 2 * psi_hydrogen(N, L, M, kd.x, kd.z, kd.y)[1] * psi_hydrogen(N, L, M, kd.x, kd.z, kd.y)[1]) * -div.yxy;
            vec3 ve = (Threshold - 2 * psi_hydrogen(N, L, M, ke.x, ke.z, ke.y)[1] * psi_hydrogen(N, L, M, ke.x, ke.z, ke.y)[1]) * div.yyx;
            vec3 vf = (Threshold - 2 * psi_hydrogen(N, L, M, kf.x, kf.z, kf.y)[1] * psi_hydrogen(N, L, M, kf.x, kf.z, kf.y)[1]) * -div.yyx;
            vec3 normal = va + vb + vc + vd + ve + vf;
            return normal;

       }
       else
       {
                vec3 va = (Threshold - 2 * psi_hydrogen(N, L, M, ka.x, ka.z, ka.y)[0] * psi_hydrogen(N, L, M, ka.x, ka.z, ka.y)[0]) * div.xyy;
                vec3 vb = (Threshold - 2 * psi_hydrogen(N, L, M, kb.x, kb.z, kb.y)[0] * psi_hydrogen(N, L, M, kb.x, kb.z, kb.y)[0]) * -div.xyy;
                vec3 vc = (Threshold - 2 * psi_hydrogen(N, L, M, kc.x, kc.z, kc.y)[0] * psi_hydrogen(N, L, M, kc.x, kc.z, kc.y)[0]) * div.yxy;
                vec3 vd = (Threshold - 2 * psi_hydrogen(N, L, M, kd.x, kd.z, kd.y)[0] * psi_hydrogen(N, L, M, kd.x, kd.z, kd.y)[0]) * -div.yxy;
                vec3 ve = (Threshold - 2 * psi_hydrogen(N, L, M, ke.x, ke.z, ke.y)[0] * psi_hydrogen(N, L, M, ke.x, ke.z, ke.y)[0]) * div.yyx;
                vec3 vf = (Threshold - 2 * psi_hydrogen(N, L, M, kf.x, kf.z, kf.y)[0] * psi_hydrogen(N, L, M, kf.x, kf.z, kf.y)[0]) * -div.yyx;


                vec3 normal = va + vb + vc + vd + ve + vf;
                return normal;
       }
    }
    
  	return vec3(0,0,0);
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
	float dx = CameraR/200;
	
	float a = 1;
	vec3 Pos = Campos;

   
    vec4 result = CheckPsi(Pos,ray,dx,CameraR);
    float distanceEstimate = result.w; 
    Pos = result.xyz;

    vec3 normal = calcNormal(Pos);
    if(normal==vec3(0,0,0))
    {
        texColor=vec4(0,0,0,0);
    }
    else{
            vec3 light = normalize(vec3(0,0,10));
            float angle = acos(dot(light,normalize(normal)));
    
	        //texColor =  vec4(distanceEstimate*CameraR/10,distanceEstimate*distanceEstimate*CameraR/20,distanceEstimate*CameraR/30,1);
            texColor=vec4(normalize(normal),1);
        }
    

	color = texColor;
};
