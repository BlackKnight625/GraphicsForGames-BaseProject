#version 330 core

uniform vec4 MarbleColor, MortarColor;
uniform vec2 MarbleSize, MarblePct;

in float exIntensity;
in vec3 exPosition;
out vec4 FragmentColor;

void main(void) {
  vec4 color;
  vec3 position;
  vec2 use_marble;
  position = exPosition / vec3(MarbleSize, 0);

  if(fract(position.y * 0.5) > 0.5) {
  	position.x += 0.5;
  }

  position = fract(position);
  use_marble = step(vec2(position), MarblePct);

  color = mix(MortarColor, MarbleColor, use_marble.x * use_marble.y);
  color *= exIntensity;

  FragmentColor = color;
}
