#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 roots_real;
uniform vec4 roots_img;
uniform vec4 coefficients;
uniform float zoom_ratio;
uniform int num_of_iterations;
uniform float x_offset;
uniform float y_offset;

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

float complex_distance(vec2 num1, vec2 num2)
{
	vec2 diff = num1 - num2;
	return sqrt(pow(diff.x, 2) + pow(diff.y, 2));
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

vec2[3] get_roots()
{
	vec2 root1 = complex(roots_real[0], roots_img[0]);
	vec2 root2 = complex(roots_real[1], roots_img[1]);
	vec2 root3 = complex(roots_real[2], roots_img[2]);

	return vec2[3](root1, root2, root3);
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
	float scaled_x = pixel_position.x*zoom_ratio + x_offset;
	float scaled_y = pixel_position.y*zoom_ratio + y_offset;

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
