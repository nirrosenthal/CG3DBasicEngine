// taken from: https://codepen.io/p3xx/pen/EJGvwr
#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 iResolution;
uniform float iGlobalTime;
#define PI 3.14159265
uniform float transparency;




// Hyperboloc functions from https://machinesdontcare.wordpress.com/2008/03/10/glsl-cosh-sinh-tanh/

/// COSH Function (Hyperbolic Cosine)
float cosh(float val)
{
    float tmp = exp(val);
    float cosH = (tmp + 1.0 / tmp) / 2.0;
    return cosH;
}

// TANH Function (Hyperbolic Tangent)
float tanh(float val)
{
    float tmp = exp(val);
    float tanH = (tmp - 1.0 / tmp) / (tmp + 1.0 / tmp);
    return tanH;
}

// SINH Function (Hyperbolic Sine)
float sinh(float val)
{
    float tmp = exp(val);
    float sinH = (tmp - 1.0 / tmp) / 2.0;
    return sinH;
}
// Complex Number math by julesb
// https://github.com/julesb/glsl-util



vec2 cx_mul(vec2 a, vec2 b) { return vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);}
vec2 cx_div(vec2 a, vec2 b) { return vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));}
float cx_modulus(vec2 a){ return length(a);}
vec2 cx_conj(vec2 a){ return vec2(a.x,-a.y);}
float cx_arg(vec2 a) { return atan(a.y,a.x);}
vec2 cx_sin(vec2 a) { return vec2(sin(a.x) * cosh(a.y), cos(a.x) * sinh(a.y));}
vec2 cx_cos(vec2 a) { return vec2(cos(a.x) * cosh(a.y), -sin(a.x) * sinh(a.y));}
vec2 cx_sqrt(vec2 a) {
    float r = sqrt(a.x*a.x+a.y*a.y);
    float rpart = sqrt(0.5*(r+a.x));
    float ipart = sqrt(0.5*(r-a.x));
    if (a.y < 0.0) ipart = -ipart;
    return vec2(rpart,ipart);
}

vec2 cx_tan(vec2 a) {
    return cx_div(cx_sin(a), cx_cos(a));
}

vec2 cx_log(vec2 a) {
    float rpart = sqrt((a.x*a.x)+(a.y*a.y));
    float ipart = atan(a.y,a.x);
    if (ipart > PI) ipart=ipart-(2.0*PI);
    return vec2(log(rpart),ipart);
}

vec2 cx_mobius(vec2 a) {
    vec2 c1 = a - vec2(1.0,0.0);
    vec2 c2 = a + vec2(1.0,0.0);
    return cx_div(c1, c2);
}

vec2 cx_z_plus_one_over_z(vec2 a) {
    return a + cx_div(vec2(1.0,0.0), a);
}

vec2 cx_z_squared_plus_c(vec2 z, vec2 c) {
    return cx_mul(z, z) + c;
}

vec2 cx_sin_of_one_over_z(vec2 z) {
    return cx_sin(cx_div(vec2(1.0,0.0), z));
}


////////////////////////////////////////////////////////////
// end Complex Number math by julesb
////////////////////////////////////////////////////////////


// From Stackoveflow
// http://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Johan Karlsson's own additions to complex number math

vec2 cx_sub(vec2 a, vec2 b) {return vec2(a.x - b.x, a.y - b.y);}
vec2 cx_add(vec2 a, vec2 b) {return vec2(a.x + b.x, a.y + b.y);}
float cx_abs(vec2 a) {return sqrt(a.x * a.x + a.y * a.y);}
vec2 cx_to_polar(vec2 a) {
    float phi = atan(a.x, a.y);
    float r = sqrt(a.x * a.x + a.y * a.y);
    return vec2(r, phi);
}


// End utils, here comes the actual fractal

// sin(z) - 2
vec2 f(vec2 z) {
    vec2 sinz = cx_sin(z);
    vec2 sinzminustwo = vec2(sinz.x - 2.0, sinz.y);
    return sinzminustwo;
}

// f(z) derivated
// cos(z)
vec2 fPrim(vec2 z) {
    return cx_cos(z);
}

const int maxIterations = 800;
vec2 one = vec2(1, 0);
vec3 newtonRapson(vec2 z) {
    vec2 oldZ = z;
    float s = 0.0;
    for(int i = 0; i < maxIterations; i++){
        z = cx_sub(z, cx_div(f(z), fPrim(z)));
if(abs(oldZ.x - z.x) < 0.00001 && abs(oldZ.y - z.y) < 0.00001) {
    break;
}

vec2 w = cx_div(one, cx_sub(oldZ, z));
float wAbs = cx_abs(w);

s += exp(-wAbs);
oldZ = z;
}
return vec3(s, cx_to_polar(z));
}



void main(void)
{
    // I have no clue about what I am doing
    // Trial and error
    // Magic numbers
    // You are welcome
    float time = iGlobalTime / 6000;
    vec2 uv = (position0.xy - 0.5 * iResolution.xy) / min(iResolution.y, iResolution.x);
    vec2 moved = vec2(uv.x - 1.0, uv.y-0.7);
    float cs = cos(time/5.0);
    float sn = sin(time/5.0);
    vec2 rotated = vec2(moved.x * cs - moved.y * sn, moved.x * sn + moved.y * cs);
    vec3 result = newtonRapson(rotated);
    float c = 0.9-result.x/float(maxIterations)*50.0;
    vec3 color = hsv2rgb(vec3(result.z + time/15.0, 1.0, c));
    gl_FragColor = vec4(color, 1.0);
    gl_FragColor.w = transparency;
}