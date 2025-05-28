#version 460 core
//Schader Code modified from https://www.shadertoy.com/view/7lsGDr

#define EPSILON 0.000001
#define mult matrixCompMult

layout(rgba32f, binding = 0) uniform image2D outputImage;
layout(rgba32f, binding = 1) uniform image2D inputImage;

layout(local_size_x = 20, local_size_y = 20) in;

uniform int Run;

const float samplingDist = 2;

const ivec4 iv0 = ivec4(0);
const ivec4 iv1 = ivec4(1);
const ivec4 iv2 = ivec4(2);
const ivec4 iv3 = ivec4(3);
const vec4 v0 = vec4(0.);
const vec4 v1 = vec4(1.);
const mat4 m0 = mat4(v0, v0, v0, v0);
const mat4 m1 = mat4(v1, v1, v1, v1);

const float base_noise = 0.175;
const float R = 12.;  // space resolution = kernel radius
const float T = 2.;  // time resolution = number of divisions per unit time
const mat4    betaLen = mat4( 1., 1., 2., 2., 1., 2., 1., 1., 1., 2., 2., 2., 1., 2., 1., v0 );  // kernel ring number
const mat4      beta0 = mat4( 1., 1., 1., 0., 1., 5./6., 1., 1., 1., 11./12., 3./4., 1., 1., 1./6., 1., v0 );  // kernel ring heights
const mat4      beta1 = mat4( 0., 0., 1./4., 1., 0., 1., 0., 0., 0., 1., 1., 11./12., 0., 1., 0., v0 );
const mat4      beta2 = mat4( 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., v0 );
const mat4         mu = mat4( 0.242, 0.375, 0.194, 0.122, 0.413, 0.221, 0.192, 0.492, 0.426, 0.361, 0.464, 0.361, 0.235, 0.381, 0.216, v0 );  // growth center
const mat4      sigma = mat4( 0.061, 0.1553, 0.0361, 0.0531, 0.0774, 0.0365, 0.0649, 0.1219, 0.1759, 0.1381, 0.1044, 0.0686, 0.0924, 0.1118, 0.0748, v1 );  // growth width
const mat4        eta = mat4( 0.144, 0.506, 0.332, 0.3, 0.502, 0.58, 0.344, 0.268, 0.582, 0.326, 0.418, 0.642, 0.39, 0.378, 0.294, v0 );  // growth strength
const mat4       relR = mat4( 0.98, 0.59, 0.5, 0.93, 0.73, 0.88, 0.93, 0.61, 0.84, 0.7, 0.57, 0.73, 0.74, 0.87, 0.72, v1 );  // relative kernel radius
const mat4        src = mat4( 0., 0., 0., 1., 1., 1., 2., 2., 2., 0., 0., 1., 1., 2., 2., v0 );  // source channels
const mat4        dst = mat4( 0., 0., 0., 1., 1., 1., 2., 2., 2., 1., 2., 0., 2., 0., 1., v0 );  // destination channels

const int intR = int(ceil(R));
const float dt = 1./T;       // time step

const vec4 kmv = vec4(0.5);    // kernel ring center
const mat4 kmu = mat4(kmv, kmv, kmv, kmv);
const vec4 ksv = vec4(0.15);    // kernel ring width
const mat4 ksigma = mat4(ksv, ksv, ksv, ksv);

const ivec4 src0 = ivec4(src[0]), src1 = ivec4(src[1]), src2 = ivec4(src[2]), src3 = ivec4(src[3]);
const ivec4 dst0 = ivec4(dst[0]), dst1 = ivec4(dst[1]), dst2 = ivec4(dst[2]), dst3 = ivec4(dst[3]);


// Perlin noise (same as before)
vec2 randomGradient(vec2 co)
{
    float random = fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453) * 6.2831853;
    return vec2(cos(random), sin(random));
}

float interpolate(float a0, float a1, float x)
{
    float g = (3.0 - x * 2.0) * x * x;
    return (a1 - a0) * g + a0;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
    vec2 grad = randomGradient(vec2(ix, iy));
    float dx = x - float(ix);
    float dy = y - float(iy);
    return dx * grad.x + dy * grad.y;
}

float noise(vec2 p)
{
    int x0 = int(floor(p.x));
    int x1 = x0 + 1;
    int y0 = int(floor(p.y));
    int y1 = y0 + 1;

    float sx = p.x - float(x0);
    float sy = p.y - float(y0);

    float n0 = dotGridGradient(x0, y0, p.x, p.y);
    float n1 = dotGridGradient(x1, y0, p.x, p.y);
    float ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, p.x, p.y);
    n1 = dotGridGradient(x1, y1, p.x, p.y);
    float ix1 = interpolate(n0, n1, sx);

    return interpolate(ix0, ix1, sy);
}
vec4 getSrc( vec3 v,  ivec4 srcv)
{
    return
        v.r * vec4(equal(srcv, iv0)) + 
        v.g * vec4(equal(srcv, iv1)) +
        v.b * vec4(equal(srcv, iv2));
}

mat4 getVal(ivec2 xy)
{
   
    vec3 val = imageLoad(inputImage, xy).xyz;
    return mat4( getSrc(val, src0), getSrc(val, src1), getSrc(val, src2), getSrc(val, src3) );
}
float getDst(in mat4 m, in ivec4 ch)
{
    return 
        dot(m[0], vec4(equal(dst0, ch))) + 
        dot(m[1], vec4(equal(dst1, ch))) + 
        dot(m[2], vec4(equal(dst2, ch))) + 
        dot(m[3], vec4(equal(dst3, ch)));
}
mat4 bell(in mat4 x, in mat4 m, in mat4 s)
{
    mat4 v = -mult(x-m, x-m) / s / s / 2.;
    return mat4( exp(v[0]), exp(v[1]), exp(v[2]), exp(v[3]) );
}

mat4 delta(mat4 n, mat4 mu, mat4 sigma)
{
    return 2. * bell(n, mu, sigma) - 1.;
}

mat4 Kernel(in float r, in mat4 relR)
{
    mat4 Br = betaLen / relR * r;
    ivec4 Br0 = ivec4(Br[0]), Br1 = ivec4(Br[1]), Br2 = ivec4(Br[2]), Br3 = ivec4(Br[3]);

    // (Br==0 ? beta0 : 0) + (Br==1 ? beta1 : 0) + (Br==2 ? beta2 : 0)
    mat4 height = mat4(
        beta0[0] * vec4(equal(Br0, iv0)) + beta1[0] * vec4(equal(Br0, iv1)) + beta2[0] * vec4(equal(Br0, iv2)),
        beta0[1] * vec4(equal(Br1, iv0)) + beta1[1] * vec4(equal(Br1, iv1)) + beta2[1] * vec4(equal(Br1, iv2)),
        beta0[2] * vec4(equal(Br2, iv0)) + beta1[2] * vec4(equal(Br2, iv1)) + beta2[2] * vec4(equal(Br2, iv2)),
        beta0[3] * vec4(equal(Br3, iv0)) + beta1[3] * vec4(equal(Br3, iv1)) + beta2[3] * vec4(equal(Br3, iv2)) );
    mat4 mod1 = mat4( mod(Br[0], 1.), mod(Br[1], 1.), mod(Br[2], 1.), mod(Br[3], 1.) );
    return mult(height, bell(mod1, kmu, ksigma));
}


void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 texSize = imageSize(outputImage);

    if (pixelCoord.x >= texSize.x || pixelCoord.y >= texSize.y)
        return;

    vec2 normalizedCoord = vec2(pixelCoord) / vec2(texSize);
    vec4 O;

    if (Run == 0)
    {
        float Col = base_noise + noise(normalizedCoord * 50.0);
        O = vec4(base_noise + noise(normalizedCoord * 50.0/samplingDist), base_noise + noise(normalizedCoord * 49.0/samplingDist), base_noise + noise(normalizedCoord * 44.0/samplingDist), 1.0);
    }
    else if (Run == 1)
    {
       mat4 sum = mat4(0.), total = mat4(0.);
       mat4 valSrc = mat4(0.);
       mat4 weight= mat4(0.);
        for (int i = -intR; i <= intR; ++i)
        {
            for (int j = -intR; j <= intR; ++j)
            {
                vec2 offset = vec2(i, j);
                ivec2 wrappedCoord = ivec2(mod(vec2(pixelCoord + offset), vec2(texSize)));
                float r = length(offset)/R;
                 if (r <= 1.) {
                    weight = Kernel(r, relR);
                    sum += mult( getVal(wrappedCoord + ivec2(offset*samplingDist)), weight); total += weight;
                
                 }
            }
        }
         mat4 avg = sum / (total + EPSILON);
       



        vec3 val = imageLoad(inputImage, pixelCoord).xyz;
         mat4 growth = mult(eta, delta(avg, mu, sigma));
         vec3 growthDst = vec3( getDst(growth, iv0), getDst(growth, iv1), getDst(growth, iv2) );
        
        vec3 rgb = clamp(dt * growthDst + val, 0., 1.);
        O = vec4(rgb, 1.0);
    }

    imageStore(outputImage, pixelCoord, O);
}