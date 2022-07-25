#version 330


in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
uniform float transparency;




uniform vec4 rgb;

void main()
{
	gl_FragColor = rgb;
	gl_FragColor.w = transparency;
}
