#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform mat4 colors;
uniform vec4 rootsx;
uniform vec4 rootsy;
uniform vec4 coeffs;
uniform int IterationNum;
uniform float zoom;
uniform vec4 move;
out vec4 Color;


vec2 multScalar (vec2 a, float c){return vec2(a.x*c, a.y*c);}
vec2 mult (vec2 a, vec2 b){return vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);}
vec2 divide(vec2 a, vec2 b) {return vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));}
vec2 add (vec2 a, vec2 b, vec2 c, vec2 d){return vec2(a.x + b.x + c.x + d.x, a.y + b.y + c.y + d.y);}
vec2 add (vec2 a, vec2 b, vec2 c){return vec2(a.x + b.x + c.x, a.y + b.y + c.y);}
vec2 add (vec2 a, vec2 b){return vec2(a.x + b.x, a.y + b.y);}
vec2 sub (vec2 a, vec2 b){return vec2(a.x-b.x, a.y-b.y);}

// get distance from rootsx(i),rootsy(i) to x,y
float dist(int i, float x, float y){
	float distx = x - rootsx[i];
	float disty = y - rootsy[i];
	return sqrt(distx*distx + disty*disty);
	}

// get index of closest root to x,y from roots 
int minDist(float x, float y){
	int minInd = 0;
	float minDist = 100;
	int i = 0;
	while (i != 3){
		float temp = dist(i,x,y);
		if (temp < minDist){
			minInd = i;
			minDist = temp;
		}
		i++;
	}
	return minInd;
}

// get color index 0 <= i <= 3
vec4 getColor(int i){
	return vec4(colors[0][i],colors[1][i],colors[2][i],colors[3][i]) ;
}

vec2 der(vec2 xy){
	vec2 toSecond = multScalar(mult(xy,xy), 3*coeffs[0]);
	vec2 toFirst = multScalar(xy, 2*coeffs[1]);
	vec2 toNone = vec2(coeffs[2],0);
	return add(toSecond,toFirst,toNone);
}
vec2 func(vec2 xy){
	vec2 toThird = multScalar(mult(mult(xy,xy),xy),coeffs[0]);
	vec2 toSecond = multScalar(mult(xy,xy),coeffs[1]);
	vec2 toFirst = multScalar(xy,coeffs[2]);
	vec2 toNone = vec2(coeffs[3],0);
	return add(toThird, toSecond, toFirst, toNone);
}


bool funcNotZero(vec2 xy){
	float d = 0.0001;
	return (abs(func(xy).x) > d || abs(func(xy).y) > d); 
}

// do function IterationNum times
// set color to closest root's color via index
void main()
{
	int i = 0;
	vec2 xy = vec2((position0.x  - move[0]) * zoom,(position0.y  + move[1]) * zoom);
	while (i < IterationNum && funcNotZero(xy)){
		xy = sub(xy, divide(func(xy),der(xy)));
		i++;
	}
	Color = getColor(minDist(xy.x, xy.y));	
}
