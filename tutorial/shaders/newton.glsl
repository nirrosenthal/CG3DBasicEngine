#version 330
#define PI 3.1415926538

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 coefficients;
uniform float zoom_ratio;
uniform int num_of_iterations;
uniform float x_offset_in_fractal;
uniform float y_offset_in_fractal;

vec2 complex(float num)
{
	return vec2(num, 0);
}

vec2 complex(float num1, float num2)
{
	return vec2(num1, num2);
}

vec2 complex_add(vec2 num1, vec2 num2)
{
	return num1 + num2;
}


vec2 complex_add(float num1, vec2 num2)
{
	return complex_add(complex(num1), num2);
}

vec2 complex_add(vec2 num1, float num2)
{
	return complex_add(num1, complex(num2));
}
vec2 complex_add(float num1, float num2)
{
	return complex_add(complex(num1), complex(num2));
}

vec2 complex_mult(vec2 num1, vec2 num2)
{
	float a = num1.x;
	float b = num1.y;
	float c = num2.x;
	float d = num2.y;

	return complex((a*c-b*d),(a*d+b*c));
}
vec2 complex_mult(float num1, vec2 num2)
{
	return complex_mult(complex(num1), num2);
}
vec2 complex_mult(vec2 num1, float num2)
{
	return complex_mult(num1, complex(num2));
}
vec2 complex_mult(float num1, float num2)
{
	return complex(num1*num2);
}

float complex_distance(vec2 num1, vec2 num2)
{
	vec2 diff = num1 - num2;
	return sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}
float complex_distance(float num1, vec2 num2)
{
	return complex_distance(complex(num1), num2);
}
float complex_distance(vec2 num1, float num2)
{
	return complex_distance(num1, complex(num2));
}
float complex_distance(float num1, float num2)
{
	return complex_distance(complex(num1), complex(num2));
}
float complex_abs(vec2 num)
{
	return complex_distance(num, 0);
}
float complex_abs(float num)
{
	return abs(num);
}

vec2 complex_pow(vec2 num, int pow)
{
	vec2 base = complex(1);
	for(int i=0; i<pow; i++) {
		base = complex_mult(base, num);
	}
	return base;
}

vec2 complex_pow(float num, int pow)
{
	return complex_pow(complex(num), pow);
}

vec2 complex_div(vec2 num1, vec2 num2)
{
	float a = num1.x;
	float b = num1.y;
	float c = num2.x;
	float d = num2.y;

	return complex((a*c+b*d)/(c*c+d*d), (b*c-a*d)/(c*c+d*d));
}
vec2 complex_div(float num1, vec2 num2)
{
	return complex_div(complex(num1), num2);
}
vec2 complex_div(vec2 num1, float num2)
{
	return complex_div(num1, complex(num2));
}
vec2 complex_div(float num1, float num2)
{
	return complex(num1/num2);
}
vec2 complex_sqrt(vec2 num)
{
	float sqrtReal = sqrt((complex_abs(num) + num[0])/2.0);
	float sign = 0;
	if(num[1] < 0) {
		sign = -1;
	} else if (num[1] > 0) {
		sign = 1;
	}
	float sqrtImg = sign * sqrt((complex_abs(num)-num[0])/2.0);
	return complex(sqrtReal, sqrtImg);
}
float complex_norm(vec2[2] vec) {
	return sqrt(pow(complex_abs(vec[0]), 2) + pow(complex_abs(vec[1]), 2));
}



vec2 NewtonCubicRoot(vec2 num)
{
	vec2 root = num;
	const int iter = 9;
	if(complex_abs(num) < 1 / pow(10, 9)) {
		return num;
	} // else
	if(complex_abs(num) < 0.9) {
		root = complex(1);
	}
	for (int k = 0; k < iter; k++)
	{
		root = complex_div((2.0f*complex_pow(root,3) + num), (complex_pow(root, 2)/3.0f));
	}
	return root;

}

vec2[3] FindRootsOfReduceEquation(vec2[2] reduceCoeffs)
{
	vec2[3] roots;
	roots[0] = complex(0);
	roots[1] = complex(0);
	roots[2] = complex(0);

	vec2 sqroot = complex_sqrt(complex_pow(reduceCoeffs[0], 3)/27.0 + complex_pow(reduceCoeffs[1], 2)/4.0);
	vec2 p = NewtonCubicRoot(reduceCoeffs[1]/2.0 + sqroot);
	vec2 n = NewtonCubicRoot(reduceCoeffs[1]/2.0 - sqroot);

	roots[0] = p + n;
	roots[1] = complex_mult(p, complex(cos(2.0*PI/3.0), sin(2.0*PI/3.0))) - complex_mult(n, complex(cos(PI/3.0), PI/3.0));
	roots[2] = complex_mult(-p, complex(cos(PI/3.0), sin(PI/3.0))) + complex_mult(n, complex(cos(2.0*PI/3.0), sin(2.0*PI/3.0)));

	return roots;
}

vec2[3] get_roots()
{
	vec2[2] reduceCoeffs;
	reduceCoeffs[0] = complex(0);
	reduceCoeffs[1] = complex(0);
	vec2[3] roots;
	vec2 bOver3a = complex_div(coefficients[1], coefficients[0]) / 3.0;
	reduceCoeffs[0] = coefficients[2] - coefficients[1] - 3.0 * complex_pow(bOver3a, 2);
	reduceCoeffs[1] = complex_mult(complex_div(coefficients[2], coefficients[0]), bOver3a) - 2.0 * (complex_pow(bOver3a, 3));
	if(complex_norm(reduceCoeffs) > 0.000001) {
		roots = FindRootsOfReduceEquation(reduceCoeffs);
		roots[0] -= bOver3a;
		roots[1] -= bOver3a;
		roots[2] -= bOver3a;
	} else {
		roots[0] = -1.0*bOver3a;
		roots[1] = complex_mult(complex(cos(PI/3.0f), sin(PI/3.0f)), bOver3a);
		roots[2] = complex_mult(complex(cos(2.0*PI/3.0), sin(2.0*PI/3.0)), bOver3a);
	}

	return roots;
}


vec2 func(vec2 z)
{
	float a = coefficients[0];
	float b = coefficients[1];
	float c = coefficients[2];
	float d = coefficients[3];
	return a * complex_pow(z,3) + b * complex_pow(z,2) + c * z + complex(d);
}

vec2 der(vec2 z)
{
	float a = coefficients[0];
	float b = coefficients[1];
	float c = coefficients[2];
	float d = coefficients[3];
	return 3*a*complex_pow(z,2) + 2*b*z + complex(c);
}

vec2 scaled_position(vec2 pixel_position)
{
	float scaled_x = pixel_position.x*zoom_ratio + x_offset_in_fractal;
	float scaled_y = pixel_position.y*zoom_ratio + y_offset_in_fractal;

	return vec2(scaled_x, scaled_y);
}


void main()
{
	const vec3[3] colors = vec3[3](
		vec3(255, 0, 0) / 255, 	  // RED
		vec3(0, 255, 0) / 255, 	 // GREEN
		vec3(0, 0, 255) / 255 	// BLUE
	);

	vec2[3] roots = get_roots();
	vec2 z = scaled_position(position0.xy);

	for(int i=0; i < num_of_iterations; i++)
	{
		vec2 step = complex_div(func(z), der(z));
		z = z - coefficients[0] * step;
	}

	vec3 chosen_color;
	float dist_0 = complex_distance(z, roots[0]);
	float dist_1 = complex_distance(z, roots[1]);
	float dist_2 = complex_distance(z, roots[2]);
	float min_dist = min(min(dist_0, dist_1), dist_2);

	if(min_dist == dist_0)
		chosen_color = colors[0];
	else if(min_dist == dist_1)
		chosen_color = colors[1];
	else
		chosen_color = colors[2];

	gl_FragColor = vec4(chosen_color, 0);
}
