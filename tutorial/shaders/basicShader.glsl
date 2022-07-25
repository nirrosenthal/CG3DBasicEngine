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
//	int kernelSize = 5;
//	int kernelSizeDiv = kernelSize/2;
//	int numOfNeigh = 0;
//	vec4 neighColor;
//	float posScales0[5] = float[5]( 0.003765,	0.015019,	0.023792,	0.015019,	0.003765);
//	float posScales1[5] = float[5]( 0.015019,	0.059912,	0.094907,	0.059912,	0.015019);
//	float posScales2[5] = float[5]( 0.023792,	0.094907,	0.0150342,	0.094907,	0.023792);
//	float posScales3[5] = float[5]( 0.015019,	0.059912,	0.094907,	0.059912,	0.015019);
//	float posScales4[5] = float[5]( 0.003765,	0.015019,	0.023792,	0.015019,	0.003765);
//
//	for (int yCo = -kernelSizeDiv; yCo < kernelSizeDiv; yCo ++){
//		float posScale[5];
//		if (yCo + kernelSizeDiv == 0)
//			posScale = posScales0;
//		if (yCo + kernelSizeDiv == 1)
//			posScale = posScales1;
//		if (yCo + kernelSizeDiv == 2)
//			posScale = posScales2;
//		if (yCo + kernelSizeDiv == 3)
//			posScale = posScales3;
//		if (yCo + kernelSizeDiv == 4)
//			posScale = posScales4;
//
//		for (int xCo = -kernelSizeDiv; xCo < kernelSizeDiv; xCo ++){
//			float x = texCoord0.x * xCo;
//			float y = texCoord0.y * yCo;
//			numOfNeigh++;
//			neighColor += texture2D(sampler1, vec2(x,y)) * vec4(color0,0) * posScale[xCo + kernelSizeDiv];
//		}
//	}
//	Color = neighColor;
//

	// reset the calculations
//	Color = texture2D(sampler1, vec2(position0.x,position0.y))* vec4(color0,1.0) - neighColor/numOfNeigh; //you must have gl_FragColor
//	Color = texture2D(sampler1, texCoord0)* vec4(color0,1.0) - neighColor/numOfNeigh; //you must have gl_FragColor
	Color = texture2D(sampler1, texCoord0)* vec4(color0,1.0) ; //you must have gl_FragColor
	Color.w = transparency;
}
