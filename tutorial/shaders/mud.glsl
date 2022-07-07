// taken from: https://glslsandbox.com/e#36539.0
#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform float global_time;
uniform vec4 mouse;
uniform vec4 resolution;
vec2 uv = vec2(0);
float hash(float n) { return fract(sin(n)*12345.0); }
float old_noise(vec3 x, float c1, float c2) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f       = f*f*(3.0-2.0*f);

    float h2 = c1;
    float h1 = c2;
    #define h3 (h2 + h1)

    float n = p.x + p.y*h1+ h2*p.z;
    return mix(mix(	mix( hash(n+0.0), hash(n+1.0),f.x),
    mix( hash(n+h1), hash(n+h1+1.0),f.x),f.y),
    mix(	mix( hash(n+h2), hash(n+h2+1.0),f.x),
    mix( hash(n+h3), hash(n+h3+1.0),f.x),f.y),f.z);

}
// I want to mark that this is created by accident
// awesome fucking shit this noise is at least useful now
// afl_ext 2016
float supernoise3d(vec3 p){

    float a =  old_noise(p, 883.0, 971.0);
    float b =  old_noise(p + 0.5, 113.0, 157.0);
    return (a + b) * 0.5;
}

vec3 normalx(vec3 p, float e){
    float ca = supernoise3d(p) * 3.0;
    float r = supernoise3d(p + vec3(0.0, e, 0.0)) * 3.0;
    float u = supernoise3d(p + vec3(0.0, 0.0, e)) * 3.0;
    vec3 a = vec3(0.0, ca, 0.0);
    vec3 b = vec3(0.0 - e, r, 0.0);
    vec3 c = vec3(0.0, u, 0.0 + e);
    vec3 normal = (cross(normalize(a-b), normalize(a-c)));
    return normalize(normal);
}
vec3 tonemap(vec3 xa){
    vec3 a = xa * 0.2;
    vec3 x = max(vec3(0.0),a-0.004);
    vec3 retColor = (x*(6.2*x+.5))/(x*(6.2*x+1.7)+0.06);
    vec3 gscale = vec3(retColor.r * 0.7 + retColor.g * 0.25 + retColor.b * 0.2) * 0.6;
    return mix(gscale, retColor, 1.0);//min(1.0, length(retColor)));
    //return retColor;
}
void main(){
    float time = global_time / 6000;
    vec2 uv = position0.xy;
    vec3 sun = normalize(vec3(mouse.x * 2.0 - 1.0, 1.0, mouse.y* 2.0 - 1.0));
    vec3 n = normalx(vec3(time, uv.x * 20.0, uv.y * 20.0), 0.3);
    float dt = max(0.0, dot(sun, n));
    float dt2 =  max(0.0, 0.5 + 0.5 * dot(sun, vec3(-n.x, n.y, -n.z)));
    vec3 diff = vec3(0.3, 0.1, 0.0) * (0.5 * dt2 + 0.9 * dt );
    vec3 phong = vec3(1) * pow(dt, 64.0);
    gl_FragColor = vec4(tonemap(diff + phong), 1.0);
}