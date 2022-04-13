#version 410

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform vec4 resolution;
uniform vec4 roots_real;
uniform vec4 roots_img;
uniform float a;
uniform float b;
uniform float c;
uniform float d;

uniform int num_of_iterations;

vec2 complex(float num)
{
	return vec2(num, 0);
}

vec2 complex(float num1, float num2)
{
	return vec2(num1, num2);
}

vec2 complex_mult(vec2 num1, vec2 num2)
{
	float a = num1.x;
	float b = num1.y;
	float c = num2.x;
	float d = num2.y;

	return complex((a*c-b*d),(a*d+b*c));
}

vec2 complex_pow(vec2 num, int pow)
{
	vec2 base = complex(1);
	for(int i=0; i<pow; i++) {
		base = complex_mult(base, num);
	}
	return base;
}

vec2 complex_div(vec2 num1, vec2 num2)
{
	float a = num1.x;
	float b = num1.y;
	float c = num2.x;
	float d = num2.y;

	return complex((a*c+b*d)/(c*c+d*d), (b*c-a*d)/(c*c+d*d));
}

vec2 complex_root(float x)
{
	if(x >= 0)
		return complex(sqrt(x));
	return complex(0, sqrt(-x));
}

vec2 complex_root(vec2 x)
{
	float a = x[0];
	float b = x[1];
	int sign = 1;
	if(b<0)
		sign = -1;
	float size = sqrt(a*a + b*b);
	return complex(sqrt((size+a)/2), sign * b * sqrt((size-a)/2));
}

vec2[3] get_roots()
{
	vec2 root1 = complex(roots_real[0], roots_img[0]);
	vec2 root2 = complex(roots_real[1], roots_img[1]);
	vec2 root3 = complex(roots_real[2], roots_img[2]);

	return vec2[3](root1, root2, root3);
}


vec2 func(vec2 z)
{
	return a * complex_pow(z,3) + b * complex_pow(z,2) + c * z + complex(d);
}

vec2 der(vec2 z)
{
	return 3*a*complex_pow(z,2) + 2*b*z + complex(c);
}

void main()
{
	const vec3[3] colors = vec3[3](vec3(255, 0, 0) / 255, vec3(0, 255, 0) / 255 , vec3(0, 0, 255) / 255);
	vec2[3] roots = get_roots();
	vec2 z = position0.xy;

	for(int i=0; i < num_of_iterations; i++)
	{
		vec2 step = complex_div(func(z), der(z));
		z = z - a * step;
	}

	int closest_root = 0;
	vec2 diff = z - roots[0];
	float min_dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2));

	for(int i = 1; i < 3; i++)
	{
		vec2 diff = z - roots[i];
		float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
		if(dist < min_dist)
		{
			closest_root = i;
			min_dist = dist;
		}
	}

	gl_FragColor = vec4(colors[closest_root], 0);

}
