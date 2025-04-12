#version 460 core

layout(rgba32f, binding = 0) uniform image2D outputImage;
layout(rgba32f, binding = 1) uniform image2D inputImage;

layout(local_size_x = 20, local_size_y = 20) in;

uniform int Run;

const int R = 10;
const float T = 10.;
const float dt = 1. / T;

const vec4 beta1 = vec4(1./4., 1., 0., 0.);
const float betaLen1 = 2.;
const float mu1 = 0.16;
const float sigma1 = 0.025;
const float eta1 = 0.666;

const vec4 beta2 = vec4(1., 3./4., 3./4., 0.);
const float betaLen2 = 3.;
const float mu2 = 0.22;
const float sigma2 = 0.042;
const float eta2 = 0.666;

const vec4 beta3 = vec4(1., 0., 0., 0.);
const float betaLen3 = 1.;
const float mu3 = 0.28;
const float sigma3 = 0.025;
const float eta3 = 0.666;

const float rho = 0.5;
const float omega = 0.15;
const float base_noise = 0.15;

float Bell(float x, float mu, float sigma)
{
    return exp(-(x - mu)*(x - mu) / (2. * sigma * sigma));
}

float delta(float n, float mu, float sigma)
{
    return 2. * Bell(n, mu, sigma) - 1.;
}

float Kernel(float r, float betaLen, vec4 beta)
{
    if (r > 1.) return 0.;
    float Br = r * betaLen;
    float height = beta[int(Br)];
    return height * Bell(mod(Br, 1.), rho, omega);
}

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
        O = vec4(Col, Col, Col, 1.0);
    }
    else if (Run == 1)
    {
        float sum1 = 0., KernelSum1 = 0.;
        float sum2 = 0., KernelSum2 = 0.;
        float sum3 = 0., KernelSum3 = 0.;

        for (int i = -R; i <= R; ++i)
        {
            for (int j = -R; j <= R; ++j)
            {
                ivec2 offset = ivec2(i, j);
                ivec2 wrappedCoord = ivec2(mod(vec2(pixelCoord + offset), vec2(texSize)));

                float val = imageLoad(inputImage, wrappedCoord).x;
                float u = length(vec2(i, j)) / float(R);

                float w1 = Kernel(u, betaLen1, beta1);
                float w2 = Kernel(u, betaLen2, beta2);
                float w3 = Kernel(u, betaLen3, beta3);

                sum1 += val * w1; KernelSum1 += w1;
                sum2 += val * w2; KernelSum2 += w2;
                sum3 += val * w3; KernelSum3 += w3;
            }
        }

        float avg1 = sum1 / KernelSum1;
        float avg2 = sum2 / KernelSum2;
        float avg3 = sum3 / KernelSum3;

        float growth1 = delta(avg1, mu1, sigma1);
        float growth2 = delta(avg2, mu2, sigma2);
        float growth3 = delta(avg3, mu3, sigma3);

        float growth = growth1 * eta1 + growth2 * eta2 + growth3 * eta3;

        float val = imageLoad(inputImage, pixelCoord).x;
        float Col = clamp(dt * growth + val, 0.0, 1.0);
        O = vec4(Col, Col, Col, 1.0);
    }

    imageStore(outputImage, pixelCoord, O);
}