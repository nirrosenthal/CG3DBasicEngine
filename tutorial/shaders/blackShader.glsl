#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform float transparency;



out vec4 Color;
void main()
{
	gl_FragColor = vec4(0,0,0,0); //you must have gl_FragColor
	gl_FragColor.w = transparency;
}
