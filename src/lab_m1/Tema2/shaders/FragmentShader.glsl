#version 330

// Input
in vec3 color;

// Uniforms
uniform int hp;
uniform int obj_type;

// Output
layout(location = 0) out vec4 out_color;

vec3 color_displacement(vec3 color, float hp)
{
	float dlt = log(hp / 100);

	return vec3(color.x + dlt, color.y + dlt, color.z + dlt);
}

void main()
{
	vec3 delta_color = vec3(0, 0, 0);

    if (obj_type == 2 || obj_type == 3)
	{
		delta_color = color_displacement(color, hp);
	}

    out_color = vec4(color + delta_color, 1);

}
