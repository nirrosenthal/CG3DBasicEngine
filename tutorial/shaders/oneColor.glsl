#version 330
#define PI 3.1415926538

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform float r;
uniform float g;
uniform float b;

void main()
{
	gl_FragColor = vec4(r, g, b, 0) / 255.0;
}
