#version 410

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform vec4 resolution;
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

vec2 NewtonCubicRoot(vec2 num)
{
	vec2 root = num;
	const int iter = 100;
	for (int k = 0; k < iter; k++)
	{
		root = complex_div((2.0/3.0)*complex_pow(root,3) + num, complex_pow(root,2));
	}
	return root;
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



vec2[3] FindRootsOfReduceEquation(vec2[3] reduceCoeffs)
{
	vec2[3] roots = vec2[](complex(0),complex(0),complex(0));
	vec2 sqroot = complex_root(complex_pow(reduceCoeffs[0], 3)/27.0f + complex_pow(reduceCoeffs[0], 2)/4.0f);
	vec2 p = NewtonCubicRoot(reduceCoeffs[1]/2.0f + sqroot);
	vec2 n = NewtonCubicRoot(reduceCoeffs[1]/2.0f - sqroot);
	roots[0] = p + n;
	roots[1] = complex_mult(p, complex(acos(2.0f*3.14159f/3.0f),asin(2*3.14159f/3.0f))) -
		complex_mult(n ,complex(acos(1.0f*3.14159f/3.0f),asin(1*3.14159f/3.0f)));
	roots[2] = complex_mult(-p, complex(acos(1.0f*3.14159f/3.0f),asin(1*3.14159f/3.0f))) +
		complex_mult(n, complex(acos(2.0f*3.14159f/3.0f),asin(2*3.14159f/3.0f)));
	return roots;
}



vec2[3] FindCubicRoots()
{
	vec2[3] reduceCoeffs = vec2[](complex(0), complex(0), complex(0));
	vec2[3] roots;
	vec2 bOver3a = complex((b/a)/3.0f);
	reduceCoeffs[0] = c/a - 3.0f*complex_pow(bOver3a,2);
	reduceCoeffs[1] = c/a*bOver3a - d/a - 2.0f*complex_pow(bOver3a,3);
	// std::cout<<"reduced\n"<<reduceCoeffs<<std::endl;
	if (abs(reduceCoeffs[1].x-reduceCoeffs[0].x)>0.000001)
	{
		roots = FindRootsOfReduceEquation(reduceCoeffs);
		roots[0] -= bOver3a;
		roots[1] -= bOver3a;
		roots[2] -= bOver3a;
	}
	else
	{
		roots[0] = -1.0f*bOver3a;
		roots[1] = complex_mult(complex(acos(3.14159f/3.0f), asin(3.14159f/3.0f)), bOver3a);
		roots[2] = complex_mult(complex(acos(2.0f*3.14159f/3.0f), asin(2*3.14159f/3.0f)), bOver3a);
	}

	return roots;
}
vec2 func(vec2 z)
{
	return a * complex_pow(z,3) + b * complex_pow(z,2) + c*z + complex(d);
}

vec2 der(vec2 z)
{
	return 3*a*complex_pow(z,2) + 2*b*z + complex(c);
}
void main()
{
	vec2[3] roots = FindCubicRoots();

	vec2 coordinates = ((gl_FragCoord.xy) - 0.5) / resolution.xy;

	vec2 z = coordinates;
//	vec2 res = NewtonCubicRoot(coordinates);
//	bool found_d = false;
//	float min_d = 0;
	vec3[3] colors = vec3[3](vec3(3, 138, 138)/255, vec3(138, 3, 138)/255 , vec3(138, 138, 3)/255);

	const int num_of_iterations = 1000;
	const double tolerance = 0.000000000001;
	for(int iteration=0; iteration < num_of_iterations; iteration++){
		vec2 step = complex_div(func(z), der(z));
		z = z - a * step;

		for(int i = 0; i < 3; i++){
			vec2 diff = z - roots[i];

			//If the current iteration is close enough to a root, color the pixel.
			if (sqrt(pow(diff.x, 2) + pow(diff.y, 2)) < tolerance){
				gl_FragColor = vec4(colors[i], 1);
				return;
			}
		}
	}

}
