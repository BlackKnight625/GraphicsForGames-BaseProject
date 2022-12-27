#version 330 core

in vec3 exNormal;
out vec4 FragmentColor;

void main(void)
{
	vec3 N = normalize(exNormal);

	vec3 color;
/**/
	color = (N + vec3(1.0)) * 0.5;
/** /
	vec3 pos, neg;
	pos.x = (N.x > 0.0) ? N.x : 0.0;
	neg.x = (N.x < 0.0) ? -N.x : 0.0;
	pos.y = (N.y > 0.0) ? N.y : 0.0;
	neg.y = (N.y < 0.0) ? -N.y : 0.0;
	pos.z = (N.z > 0.0) ? N.z : 0.0;
	neg.z = (N.z < 0.0) ? -N.z : 0.0;

	color.r = pos.x + neg.y + neg.z;
	color.g = pos.y + neg.x + neg.z;
	color.b = pos.z + neg.x + neg.y;
/**/
	FragmentColor = vec4(color, 1.0);
}
