#version 330
in vec3 color_frag;
out vec4 outColor;
uniform float transparency;



void main()
  {
    outColor = vec4(color_frag, 1.0);
    outColor.w = transparency;
  }