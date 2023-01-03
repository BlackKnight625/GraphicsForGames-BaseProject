#version 330 core

uniform vec4 MarbleColor, MortarColor;
uniform vec2 MarbleSize, MarblePct;

in float exIntensity;
in vec2 exPosition;
out vec4 FragmentColor;

#define Integral(x,p,notp) ((floor(x)*(p))+max(fract(x)-notp),0.0))

void main(void) {
  vec4 color;
  vec2 position, use_marble;
  position = exPosition / MarbleSize;

  if(fract(position.y * 0.5) > 0.5) {
  	position.x += 0.5;
  }

  position = fract(position);
  use_marble = step(position, MarblePct);

  color = mix(MortarColor, MarbleColor, use_marble.x * use_marble.y);
  color *= exIntensity;

  FragmentColor = color;
}
