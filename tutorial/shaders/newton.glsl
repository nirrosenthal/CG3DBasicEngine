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
	return a * complex_pow(z,3) + b * complex_pow(z,2) + c * z + complex(d);
}

vec2 der(vec2 z)
{
	return 3*a*complex_pow(z,2) + 2*b*z + complex(c);
}

vec2 scaled_position(vec2 pixel_position)
{
	float original_x = pixel_position.x+1;
	float original_y = pixel_position.y+1;
	float width = resolution.x;
	float height = resolution.y;
	float scaled_x, scaled_y;

//	scaled_x = original_x * zoom_ratio - 1;
//	scaled_y = original_y * zoom_ratio - 1;

	if(original_x < width/2.0)
	{
		scaled_x = (original_x*zoom_ratio) - zoom_ratio + x_offset*zoom_ratio;
	}
	else
	{
		scaled_x = (original_x/zoom_ratio) - zoom_ratio + x_offset*zoom_ratio;
	}
	if(original_y < height/2.0)
	{
		scaled_y = original_y*zoom_ratio - zoom_ratio + y_offset*zoom_ratio;
	}
	else
	{
		scaled_y = (original_y/zoom_ratio) - zoom_ratio + y_offset*zoom_ratio;
	}

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
		z = z - a * step;
	}

	int closest_root_index;
	int i=0;
	float min_dist;
	do {
		vec2 diff = z - roots[i];
		float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
		if(i==0 || dist < min_dist)
		{
			closest_root_index = i;
			min_dist = dist;
		}
		i++;
	} while(i<3);

	gl_FragColor = vec4(colors[closest_root_index], 0);

}
