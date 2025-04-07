#version 460 core

layout(rgba32f, binding = 0) uniform  image2D outputImage;

layout(local_size_x = 16, local_size_y = 16) in;
float rand(float x) {
    return fract(sin(x) * 43758.5453123);
}
vec2 randomGradient(vec2 co){
    float random =  fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453)*2*3.14159265359;
    return vec2(cos(random),sin(random));
}
float interpolate(float a0, float a1, float x)
{
    float g; // Gewicht für die Interpolation
    //g = x; // lineare Interpolation; ergibt stetiges, aber nicht differenzierbares Rauschen
    g = (3.0 - x * 2.0) * x * x; // kubische Interpolation mit dem Polynom 3 * x^2 - 2 * x^3
    //g = ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x); // Interpolation mit dem Polynom 6 * x^5 - 15 * x^4 + 10 * x^3
    return (a1 - a0) * g + a0;
}
float dotGridGradient(int ix, int iy, float x, float y)
{
    vec2 grad = randomGradient(vec2(ix, iy));
    // Berechne den Abstandsvektor:
    float dx = x -  ix;
    float dy = y -  iy;
    return dx * grad.x + dy * grad.y; // Skalarprodukt
}
float noise(vec2 p) {
    float x = p.x;
    float y = p.y;
    int x0 =  int(floor(x));
    int x1 = x0 + 1;
    int y0 =  int(floor(y));
    int y1 = y0 + 1;

    // Bestimme die Abstände von den Gitterpunkten für die Interpolation:
    float sx = x - x0;
    float sy = y - y0;

    // Interpoliere zwischen den Skalarprodukten an den vier Ecken:
    float n0, n1, ix0, ix1;
    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    return interpolate(ix0, ix1, sy);
}
void main()
{
    
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    if (pixelCoord.x >= imageSize(outputImage).x || pixelCoord.y >= imageSize(outputImage).y)
		return;

    ivec2 texSize = imageSize(outputImage);
    vec2 fTexSize = vec2(texSize);
    vec2 normalizedCoord = vec2(pixelCoord) / vec2(texSize);
   
    float Col = noise(normalizedCoord*10);
        
    vec4 O = vec4(Col,Col*0.1,Col*0.6,1);

    vec4 color = vec4(1.0,1.0, 0.0, 1.0);
    imageStore(outputImage, pixelCoord, O);
}