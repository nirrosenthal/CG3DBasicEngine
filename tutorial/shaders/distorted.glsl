#version 330
#define PI 3.14159265
#define lod 4.0 // Level of detail
#define distortGrid 1.0
#define animSpeed 0.002;
#define blueLineThickness 0.2
in vec3 position0;

uniform float time;
uniform vec4 mouse;
uniform vec4 resolution;
uniform float transparency;

// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))
    * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}


void main( void ) {
    /*
        This works by distorting the space which a grid occupies.
    */
    vec2 fragCoord = position0.xy;
    vec2 uv = fragCoord/resolution.xy;
    float dt = time * animSpeed;
    uv -= 1.0;
    uv.x *= resolution.x / resolution.y;
    dt += noise(uv*0.5 * lod + dt + uv.yx * lod * 0.5 + dot(uv,uv));
    float s = sin(dt);
    float c = cos(dt);
    mat2 rot = mat2(c,s,-s,c);

    //Grid distortion "function" in the mathematical sense.
    float gridDensity =
    (distortGrid) *
    (
    //noise layer
    (noise(uv*lod - dt + uv*rot) + noise(uv*lod - 0.5) * 0.5 + noise(uv*lod - 0.75 + uv*rot) * 0.25)
    //Space Distortion
    * abs(sin(uv.x + uv.y + noise(noise(uv*lod+dt)+uv.yx*lod + dt)*PI + dt)) * pow((1.0-blueLineThickness)/noise(uv), 2.)


    )
    + (10.0 * (1.0 - distortGrid));

    //Based on the current scaling of the grid,
    //this mathematically determines which grid cell we're in
    vec2 id = floor(uv * gridDensity);

    vec3 col = vec3(abs(id/gridDensity*2.0).xy + length((id - noise(id.xy*lod) * id)/gridDensity - cos(dt + length(uv-noise(uv*lod + dt )))*2.),0.0);


    gl_FragColor = vec4(noise((col.yz * 0.5 - col.xz + col.xy)*lod + dt) * col.zyx*0.15, 1.0);
//    lowp float grayscaleComponent = gl_FragColor.x*(1.0/3.0) + gl_FragColor.y*(1.0/3.0) + gl_FragColor.z*(1.0/3.0);
//    gl_FragColor =  vec4(.0, .0, .0, grayscaleComponent);
    gl_FragColor.w = transparency;
}